/*******************************************************************************
  Trivial File Transfer Protocol (TFTP) Client

  Summary:
    Module for Microchip TCP/IP Stack
    
  Description:
    -Provides unreliable file upload and download services to other 
     applications which wish to connect to a remote UDP based TFTP 
     server.
    -Reference: RFC 1350
*******************************************************************************/

/*******************************************************************************
FileName:   TFTPc.c
Copyright � 2015 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
#define TCPIP_THIS_MODULE_ID    TCPIP_MODULE_TFTP_CLIENT

#include "tcpip_private.h"

#if defined(TCPIP_STACK_USE_TFTP_CLIENT)
#include "tftpc_private.h"

static const TCPIP_TFTPC_MODULE_CONFIG tftpcConfigDefault =
{
    TCPIP_TFTPC_DEFAULT_IF,             // TFTPC_interface
    3,
};

static TFTP_STATE   _tftpCmdState;
static tcpipSignalHandle tftpcSignalHandle = 0;
static uint8_t tftpcInitCount=0;
static TFTP_CLIENT_VARS gTFTPClientDcpt;
static TCPIP_NET_IF*  pTftpcIf = 0;       // we use only one interface for tftp (for now at least)
static TCPIP_NET_IF*  pTftpDefIf = 0;    // default TFTPC interface
static TCPIP_TFTPC_ACCESS_ERROR      tftpClientError;
static uint32_t     tftpcTimer;
// Tracker variable for the number of TFTP retries
static uint8_t _tftpRetries;

static uint16_t _tftpError;                // Variable to preserve error condition causes for later transmission

// TFTP Static functions
static void TCPIP_TFTPC_Cleanup(void);
static void _TFTPSendFileName(TFTP_OPCODE opcode, uint8_t *fileName);
static bool TFTPOpenFile(const char *fileName, TFTP_FILE_MODE mode);
static TFTP_RESULT TFTPIsGetReady(uint8_t *getData, int *len);
static void _TFTPSendAck(TCPIP_UINT16_VAL blockNumber);
static TFTP_RESULT TFTPIsPutReady(void);

static void TCPIP_TFTPC_Process(void);

static void _TFTPSocketRxSignalHandler(UDP_SOCKET hUDP, TCPIP_NET_HANDLE hNet, TCPIP_UDP_SIGNAL_TYPE sigType, const void* param);

// TFTP status flags
static union
{
    struct
    {
        unsigned int bIsFlushed : 1;
        unsigned int bIsAcked : 1;
        unsigned int bIsClosed : 1;
        unsigned int bIsClosing : 1;
        unsigned int bIsReading : 1;
    } bits;
    uint8_t Val;
} _tftpFlags;

static union
{
    struct
    {
        TCPIP_UINT16_VAL _tftpBlockNumber;
        TCPIP_UINT16_VAL _tftpDuplicateBlock;
        TCPIP_UINT16_VAL _tftpBlockLength;
    } group2;
} MutExVar;     // Mutually Exclusive variable groups to conserve RAM.


static const char vUploadRemoteHost[TCPIP_TFTPC_HOSTNAME_LEN];

bool TCPIP_TFTPC_Initialize(const TCPIP_STACK_MODULE_CTRL* const stackData,
                       const TCPIP_TFTPC_MODULE_CONFIG* pTftpcConfig)
{
    TFTP_CLIENT_VARS*   pClient;
    uint16_t        bufferSize;
    uint16_t         totalBufferSize;

    if(stackData->stackAction == TCPIP_STACK_ACTION_IF_UP)
    {   // interface restart
        return true;
    }

    // stack start up
    if(tftpcInitCount == 0)
    {   // once per service

        if(pTftpcConfig ==0)
        {
            pTftpcConfig = &tftpcConfigDefault;
        }
        pTftpDefIf = (TCPIP_NET_IF*)TCPIP_STACK_NetHandleGet(pTftpcConfig->tftpc_interface);

        pClient = &gTFTPClientDcpt;
        pClient->hSocket = TCPIP_UDP_ClientOpen(IP_ADDRESS_TYPE_IPV4, TCPIP_TFTP_SERVER_PORT, 0);
        if(pClient->hSocket == INVALID_UDP_SOCKET)
        {
            return false;
        }
       
        totalBufferSize = TCPIP_TFTP_CLIENT_MAX_BUFFER_SIZE+TCPIP_TFTPC_FILENAME_LEN+TCPIP_TFTP_CLIENT_OPCODE+TCPIP_TFTP_CLIENT_OCTET+1;
        bufferSize = TCPIP_UDP_TxPutIsReady(pClient->hSocket, totalBufferSize); //
        if(bufferSize < totalBufferSize)
        {
            TCPIP_UDP_OptionsSet(pClient->hSocket, UDP_OPTION_TX_BUFF, (void*)(unsigned int)totalBufferSize);
        }

        TCPIP_UDP_SignalHandlerRegister(pClient->hSocket, TCPIP_UDP_SIGNAL_RX_DATA, _TFTPSocketRxSignalHandler, 0);

        // create the TFTPC timer
        tftpcSignalHandle =_TCPIPStackSignalHandlerRegister(TCPIP_THIS_MODULE_ID, TCPIP_TFTPC_Task, TCPIP_TFTPC_TASK_TICK_RATE);
        if(tftpcSignalHandle == 0)
        {   // cannot create the SNTP timer
            TCPIP_TFTPC_Cleanup();
            return false;
        }
        gTFTPClientDcpt.hSocket = pClient->hSocket;
        gTFTPClientDcpt.netH =  NULL;
        gTFTPClientDcpt.smState = SM_TFTP_END;
        gTFTPClientDcpt.callbackPos = 0;
        gTFTPClientDcpt.fileDescr =  SYS_FS_HANDLE_INVALID;
    }

    tftpcInitCount++;
    return true;
}

static  void _TFTPCReleaseSocket(TFTP_CLIENT_VARS* pClient)
{
    if(pClient->hSocket != INVALID_UDP_SOCKET)
    {
        TCPIP_UDP_Close(pClient->hSocket);
        pClient->hSocket = INVALID_UDP_SOCKET;
    }
}

static void TCPIP_TFTPC_Cleanup(void)
{
    if(tftpcSignalHandle)
    {
        _TCPIPStackSignalHandlerDeregister(tftpcSignalHandle);
        tftpcSignalHandle = 0;
    }
    tftpcInitCount = 0;

    // close the socket -
    _TFTPCReleaseSocket(&gTFTPClientDcpt);
    
}

void TCPIP_TFTPC_Deinitialize(const TCPIP_STACK_MODULE_CTRL* const stackData)
{        
    if(stackData->stackAction == TCPIP_STACK_ACTION_DEINIT)
    {   // stack shut down
        if(tftpcInitCount > 0)
        {   // we're up and running

            if(--tftpcInitCount == 0)
            {   // all closed
                // release resources
                TCPIP_TFTPC_Cleanup();
            }
        }
    }
}

TCPIP_TFTPC_ACCESS_ERROR TCPIP_TFTPC_SetCommandOption(char * hostname,TCPIP_TFTP_CMD_TYPE cmdType,char * fileName )
{
    TCPIP_DNS_RESULT    dnsRes;
    TFTP_CLIENT_VARS*   pClient;
    TFTP_RESULT tftpcRes=TFTP_OK;

    pClient = &gTFTPClientDcpt;
    //TFTP client is in progress. Retry later
    if(pClient->smState != SM_TFTP_END)
    {
        return TFTPC_ERROR_ACCESS_VIOLATION;
    }
    if(strlen(fileName)> (sizeof(pClient->fileName)-1))
    {
        return TFTPC_RES_INVALID_FILE_LENGTH;
    }
    // resolve DNS host name from command prompt
    dnsRes = TCPIP_DNS_Resolve(hostname, DNS_TYPE_A);
    if(dnsRes == DNS_RES_NAME_IS_IPADDRESS)
    {
        // don't do anything here ;
    }
    else if(dnsRes != DNS_RES_OK)
    {
        return TFTPC_ERROR_DNS_RESOLVE_ERR;
    }
    dnsRes = TCPIP_DNS_IsResolved(hostname, &pClient->tftpServerAddr);
    if(dnsRes == DNS_RES_PENDING)
    {   // ongoing operation;
        tftpcRes = TFTP_NOT_READY;
        pClient->smState = SM_TFTP_WAIT_DNS;
    }
    else if(dnsRes < 0)
    {   // some DNS error occurred; retry after waiting a while
        pClient->smState = SM_TFTP_END;
        tftpcRes = TFTP_ERROR;
    }
    else
    {
        pClient->smState = SM_TFTP_DNS_RESOLVED;
    }
    if( tftpcRes != TFTP_ERROR)
    {
        strncpy((char*)vUploadRemoteHost,hostname,sizeof(vUploadRemoteHost)-1);
        if(cmdType == TFTP_CMD_PUT_TYPE)
        {
            pClient->modeType = TFTP_FILE_MODE_WRITE;
        }
        else
        {
            pClient->modeType = TFTP_FILE_MODE_READ;
        }

        strcpy(pClient->fileName,fileName);
    }
    else
    {
        return TFTPC_ERROR_DNS_RESOLVE_ERR;
    }

    return TFTPC_ERROR_NONE;
   
}



static TFTP_RESULT TFTPIsPutReady(void)
{
    TCPIP_UINT16_VAL opCode;
    TCPIP_UINT16_VAL blockNumber;
    bool bTimeOut;
    TFTP_CLIENT_VARS*   pClient;
    int replyPktSize=0;

    pClient = &gTFTPClientDcpt;
    // Check to see if timeout has occurred.
    bTimeOut = false;
   
    pClient->smState = SM_TFTP_WAIT_FOR_ACK;
    switch(pClient->smState)
    {
        case SM_TFTP_WAIT_FOR_ACK:
            replyPktSize = TCPIP_UDP_GetIsReady(pClient->hSocket);
            if ( !replyPktSize )
            {
                if ( SYS_TMR_TickCountGet() - tftpcTimer >= (TCPIP_TFTPC_CMD_PROCESS_TIMEOUT * SYS_TMR_TickCounterFrequencyGet()) )
                {
                    bTimeOut = true;
                    tftpcTimer = SYS_TMR_TickCountGet();
                }
                 // When timeout occurs in this state, application must retry.
                if ( bTimeOut )
                {
                    if ( _tftpRetries++ > (TCPIP_TFTPC_MAX_RETRIES-1) )
                    {
                        // Forget about all previous attempts.
                        _tftpRetries = 1;
                        pClient->smState = SM_TFTP_END;
                        return TFTP_TIMEOUT;
                    }
                    else
                    {
                        pClient->smState = SM_TFTP_WAIT;
                        MutExVar.group2._tftpBlockNumber.Val--;	// Roll back by one so proper block number ID is sent for the next packet
                        pClient->smState = SM_TFTP_FILE_OPEN_AND_SEND_REQUEST;
                        return TFTP_RETRY;
                    }
                }
                break;
            }
            tftpcTimer = SYS_TMR_TickCountGet();
            // Get opCode.
            TCPIP_UDP_Get(pClient->hSocket,&opCode.v[1]);
            TCPIP_UDP_Get(pClient->hSocket,&opCode.v[0]);

            // Get block number.
            TCPIP_UDP_Get(pClient->hSocket,&blockNumber.v[1]);
            TCPIP_UDP_Get(pClient->hSocket,&blockNumber.v[0]);

            // Discard everything else.
            TCPIP_UDP_Discard(pClient->hSocket);

            // This must be ACK or else there is a problem.
            if ( opCode.Val == (uint16_t)TFTP_OPCODE_ACK )
            {
                // Also the block number must match with what we are expecting.
                if ( MutExVar.group2._tftpBlockNumber.Val == blockNumber.Val )
                {
                    // Mark that block we sent previously has been ack'ed.
                    _tftpFlags.bits.bIsAcked = true;

                    // Since we have ack, forget about previous retry count.
                    _tftpRetries = 1;

//                    // If this file is being closed, this must be last ack.
//                    // Declare it as closed.
                    if ( _tftpFlags.bits.bIsClosing )
                    {
                        _tftpFlags.bits.bIsClosed = true;
                        return TFTP_END_OF_FILE;
                    }

                    // Or else, wait for put to become ready so that caller
                    // can transfer more data blocks.
                    pClient->smState = SM_TFTP_WAIT;
                    tftpcTimer = SYS_TMR_TickCountGet();
                }
                else
                {
                    SYS_CONSOLE_PRINT("TFTPIsPutReady(): Unexpected block %d received - droping it...\n",blockNumber.Val);
                    return TFTP_NOT_READY;
                }
            }
            else if ( opCode.Val == (uint16_t)TFTP_OPCODE_ERROR )
            {
                // For error opCode, remember error code so that application
                // can read it later.
                _tftpError = blockNumber.Val;

                // Declare error.
                return TFTP_ERROR;
            }
            else
                break;


        case SM_TFTP_WAIT:
            // Wait for UDP is to be ready to transmit.
            if(TCPIP_UDP_PutIsReady(pClient->hSocket) < TCPIP_TFTP_CLIENT_MAX_BUFFER_SIZE)
            {
                TCPIP_UDP_OptionsSet(pClient->hSocket, UDP_OPTION_TX_BUFF, (void*)(unsigned int)TCPIP_TFTP_CLIENT_MAX_BUFFER_SIZE);
            }

         // Put next block of data.
            MutExVar.group2._tftpBlockNumber.Val++;
            TCPIP_UDP_Put(pClient->hSocket,0);
            TCPIP_UDP_Put(pClient->hSocket,TFTP_OPCODE_DATA);

            TCPIP_UDP_Put(pClient->hSocket,MutExVar.group2._tftpBlockNumber.v[1]);
            TCPIP_UDP_Put(pClient->hSocket,MutExVar.group2._tftpBlockNumber.v[0]);

            // Remember that this block is not yet flushed.
            _tftpFlags.bits.bIsFlushed = false;

            // Remember that this block is not acknowledged.
            _tftpFlags.bits.bIsAcked = false;
            return TFTP_OK;
	// Suppress compiler warnings on unhandled SM_TFTP_WAIT_FOR_DATA,
	// SM_TFTP_DUPLICATE_ACK, SM_TFTP_SEND_ACK, SM_TFTP_SEND_LAST_ACK enum
	// states.
        default:
            break;
    }

    return TFTP_NOT_READY;
}

void TCPIP_TFTPC_Task(void)
{
    TCPIP_MODULE_SIGNAL sigPend;

    sigPend = _TCPIPStackModuleSignalGet(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_MASK_ALL);

    if(sigPend != 0)
    { // regular TMO or RX signal occurred
        TCPIP_TFTPC_Process();
    }

}

// send a signal to the TFTP module that data is available
// no manager alert needed since this normally results as a higher layer (UDP) signal
static void _TFTPSocketRxSignalHandler(UDP_SOCKET hUDP, TCPIP_NET_HANDLE hNet, TCPIP_UDP_SIGNAL_TYPE sigType, const void* param)
{
    if(sigType == TCPIP_UDP_SIGNAL_RX_DATA)
    {
        _TCPIPStackModuleSignalRequest(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_RX_PENDING, true); 
    }
}


static void TCPIP_TFTPC_Process(void)
{
    TCPIP_NET_IF* pNetIf=NULL;
    TFTP_CLIENT_VARS*   pClient;
    TCPIP_DNS_RESULT    dnsRes;
    int32_t wCount, wLen,status;
    uint8_t data[512];
    bool res=true;
    bool bTimeout=false;
    uint32_t    replyPktSize=0;
    
    pClient = &gTFTPClientDcpt;
    pNetIf = _TCPIPStackAnyNetLinked(false);
    if(pNetIf == 0 || _TCPIPStackIsConfig(pNetIf) != 0)
    {   // not yet up and running
        return;
    }

    switch(pClient->smState)
    {
        case SM_TFTP_HOME:            
            dnsRes = TCPIP_DNS_Resolve(vUploadRemoteHost, DNS_TYPE_A);
            if(dnsRes == DNS_RES_NAME_IS_IPADDRESS)
            {
               pClient->smState = SM_TFTP_WAIT_DNS;
            }
            else if(dnsRes != DNS_RES_OK)
            {
                pClient->smState = SM_TFTP_END;
            }
            else
            {
                pClient->smState = SM_TFTP_WAIT_DNS;
            }
            break;

        case SM_TFTP_WAIT_DNS:
            dnsRes = TCPIP_DNS_IsResolved(vUploadRemoteHost, &pClient->tftpServerAddr);
            if(dnsRes == DNS_RES_PENDING)
            {   // ongoing operation;
                break;
            }
            else if(dnsRes < 0)
            {   // some DNS error occurred; retry after waiting a while
                pClient->smState = SM_TFTP_END;
                break;
            }
            else
            {
                pClient->smState++;
            }

        case SM_TFTP_DNS_RESOLVED:
            // select a running interface
            pTftpcIf = pTftpDefIf;
            if(!TCPIP_STACK_NetworkIsLinked(pTftpcIf))
            {
                pTftpcIf = _TCPIPStackAnyNetLinked(true);
            }
            if(pTftpcIf == 0)
            {   // wait some more
                tftpClientError = TFTPC_RES_TFTP_IF_ERR;
                break;
            }
            TCPIP_UDP_RemoteBind(pClient->hSocket, IP_ADDRESS_TYPE_IPV4,TCPIP_TFTP_SERVER_PORT,&pClient->tftpServerAddr);
             // receiving from multiple TFTP servers
            TCPIP_UDP_OptionsSet(pClient->hSocket, UDP_OPTION_STRICT_PORT, (void*)false);
            //TCPIP_UDP_DestinationIPAddressSet(pClient->hSocket, IP_ADDRESS_TYPE_IPV4, &pClient->tftpServerAddr);
            TCPIP_UDP_SocketNetSet(pClient->hSocket, pTftpcIf);
            pClient->smState++;
            tftpcTimer = SYS_TMR_TickCountGet();
            //break;
        case SM_TFTP_UDP_IS_OPENED:
             if(!TCPIP_UDP_TxPutIsReady(pClient->hSocket,(TCPIP_TFTPC_FILENAME_LEN+TCPIP_TFTP_CLIENT_OPCODE+TCPIP_TFTP_CLIENT_OCTET+1)))
             {
                if((SYS_TMR_TickCountGet() - tftpcTimer > TCPIP_TFTPC_ARP_TIMEOUT*SYS_TMR_TickCounterFrequencyGet()))
                {
                    bTimeout = true;
                }
                if(bTimeout)
                {
                    if ( _tftpRetries++ > (TCPIP_TFTPC_MAX_RETRIES-1) )
                    {
                        // Forget about all previous attempts.
                        _tftpRetries = 1;
                        pClient->smState = SM_TFTP_END;
                        return;
                    }
                }
                break;
             }
            _tftpRetries = 1;
            pClient->smState++;
        case SM_TFTP_FILE_OPEN_AND_SEND_REQUEST:
            res = TFTPOpenFile(pClient->fileName, pClient->modeType);
            if(res==false)
            {
                pClient->smState = SM_TFTP_END;
            }
            else
            {
                if(pClient->modeType == TFTP_FILE_MODE_WRITE)
                {
                    pClient->smState = SM_TFTP_PUT_COMMAND;
                }
                else
                {
                    pClient->smState = SM_TFTP_GET_COMMAND;
                    _tftpCmdState = SM_TFTP_WAIT_FOR_DATA;
                }
            }
            break;
        case SM_TFTP_PUT_COMMAND:
            switch(TFTPIsPutReady())
            {
                case TFTP_OK:
                    if(pClient->callbackPos != 0x00u)
                    {// The file was already opened, so load up its ID and seek
                        if(pClient->fileDescr == SYS_FS_HANDLE_INVALID)
                        {// No file handles available, so wait for now
                            pClient->smState = SM_TFTP_END;
                            return;
                        }
                        SYS_FS_FileSeek(pClient->fileDescr,(int32_t)pClient->callbackPos,SYS_FS_SEEK_SET);
                    }
                    wCount = TCPIP_TFTP_CLIENT_MAX_BUFFER_SIZE -5;  // first 512 bytes
                    wLen = SYS_FS_FileRead(pClient->fileDescr,data,wCount);
                    if(wLen == 0)
                    {// If no bytes were read, an EOF was reached
                        SYS_FS_FileClose(pClient->fileDescr);
                        pClient->fileDescr = -1;
                        pClient->callbackPos = 0;
                        pClient->smState = SM_TFTP_END;
                        TCPIP_UDP_Disconnect(pClient->hSocket,false);
                        return ;
                    }
                    else if(wLen < wCount)
                    {
                        TCPIP_UDP_ArrayPut(pClient->hSocket, data, wLen);
                         // flush these 512 bytes
                        TCPIP_UDP_Flush(pClient->hSocket);
                        _tftpFlags.bits.bIsClosing =  true;
                    }
                    else
                    {// Write the bytes to the socket
                        TCPIP_UDP_ArrayPut(pClient->hSocket, data, wLen);
                        wCount -= wLen;

                        // flush these 512 bytes
                        TCPIP_UDP_Flush(pClient->hSocket);
                        // Save the new address and close the file
                        status = SYS_FS_FileTell(pClient->fileDescr);
                        if(status == -1)
                            pClient->callbackPos = 0;
                        else
                            pClient->callbackPos = (uint32_t)status;

            }
            pClient->smState = SM_TFTP_PUT_COMMAND;
            break;
                case TFTP_RETRY:
                    pClient->smState = SM_TFTP_FILE_OPEN_AND_SEND_REQUEST;
                    break;
                case TFTP_NOT_READY:
                    if(pClient->fileDescr != -1)
                    {
                        SYS_FS_FileClose(pClient->fileDescr);
                        pClient->fileDescr = -1;
                    }
                    pClient->callbackPos = 0;
                    pClient->smState = SM_TFTP_END;
                    break;
                case  TFTP_END_OF_FILE:
                    SYS_FS_FileClose(pClient->fileDescr);
                    pClient->fileDescr = -1;
                    pClient->callbackPos = 0;
                    pClient->smState = SM_TFTP_END;
                    TCPIP_UDP_Disconnect(pClient->hSocket,false);
                    break;
                default:
                    break;
            }
            break;
        case SM_TFTP_GET_COMMAND:
            switch(TFTPIsGetReady(data,&wCount))
			{
                case TFTP_OK:
                    // Check if their is any byte need to be written to the FS -
                    if(wCount == 0)
                    {
                        break;
                    }
                    if(pClient->callbackPos != 0x00u)
                    {// The file was already opened, so load up its ID and seek
                        if(pClient->fileDescr == SYS_FS_HANDLE_INVALID)
                        {// No file handles available, so wait for now
                            pClient->smState = SM_TFTP_END;
                            return;
                        }
                        SYS_FS_FileSeek(pClient->fileDescr,(int32_t)pClient->callbackPos,SYS_FS_SEEK_SET);
                    }
                      // first 512 bytes
                    wLen = SYS_FS_FileWrite(pClient->fileDescr,data,wCount);
                    if(wLen == -1)
                    {// If no bytes were read, an EOF was reached
                        SYS_FS_FileClose(pClient->fileDescr);
                        pClient->fileDescr = -1;
                        pClient->callbackPos = 0;
                        pClient->smState = SM_TFTP_END;
                        TCPIP_UDP_Disconnect(pClient->hSocket,false);
                        return ;
                    }                   
                    else
                    {// Save the new address
                        status = SYS_FS_FileTell(pClient->fileDescr);
                        if(status == -1)
                            pClient->callbackPos = 0;
                        else
                            pClient->callbackPos = (uint32_t)status;
                    }
                    break;
                case TFTP_ACK_SEND:
                     pClient->smState = SM_TFTP_GET_COMMAND;
                     break;
                case TFTP_RETRY:
                     pClient->smState = SM_TFTP_FILE_OPEN_AND_SEND_REQUEST;
                    break;
                case TFTP_NOT_READY:
                    if(pClient->fileDescr != -1)
    				{
                        SYS_FS_FileClose(pClient->fileDescr);
                        pClient->fileDescr = -1;
                    }
                    pClient->callbackPos = 0;
                    pClient->smState = SM_TFTP_END;
                    break;
                case  TFTP_END_OF_FILE:
                    SYS_FS_FileClose(pClient->fileDescr);
                    pClient->fileDescr = -1;
                    pClient->callbackPos = 0;
                    pClient->smState = SM_TFTP_END;
                    TCPIP_UDP_Disconnect(pClient->hSocket,false);
                    break;
                default:
                    break;
            }
            break;
        case SM_TFTP_END:
            // remove all other UDP packets from the socket buffer
            // This also removes the last ack from the Socket buffer
            while(1)
            {
                replyPktSize = TCPIP_UDP_GetIsReady(pClient->hSocket);
                if(replyPktSize == 0)
                {
                    break;
                }
                else
                {
                    TCPIP_UDP_Discard(pClient->hSocket);
                }
            }
            break;
        default:
            break;
            
    }
}

static void _TFTPSendFileName(TFTP_OPCODE opcode, uint8_t *fileName)
{
    uint8_t c;
    TFTP_CLIENT_VARS*   pClient;
    pClient = &gTFTPClientDcpt;	

    
    // Write opCode
    TCPIP_UDP_Put(pClient->hSocket,0);
    TCPIP_UDP_Put(pClient->hSocket,opcode);

    // write file name, including NULL.
    do
    {
        c = *fileName++;
        TCPIP_UDP_Put(pClient->hSocket,c);
    } while ( c != '\0' );

    // Write mode - always use octet or binay mode to transmit files.
    TCPIP_UDP_Put(pClient->hSocket,'o');
    TCPIP_UDP_Put(pClient->hSocket,'c');
    TCPIP_UDP_Put(pClient->hSocket,'t');
    TCPIP_UDP_Put(pClient->hSocket,'e');
    TCPIP_UDP_Put(pClient->hSocket,'t');
    TCPIP_UDP_Put(pClient->hSocket,0);

    // Transmit it.
    TCPIP_UDP_Flush(pClient->hSocket);
    
}

static bool TFTPOpenFile(const char *fileName, TFTP_FILE_MODE mode)
{
    int32_t fp;
    TFTP_CLIENT_VARS*   pClient;
    
    pClient = &gTFTPClientDcpt;	
    
    fp = pClient->fileDescr;

    if(mode == TFTP_FILE_MODE_WRITE)
    {
        fp = SYS_FS_FileOpen_Wrapper((const char*)fileName,SYS_FS_FILE_OPEN_READ);
    }
    else
    {
        fp = SYS_FS_FileOpen_Wrapper((const char*)fileName,SYS_FS_FILE_OPEN_WRITE);        
    }

    if(fp == SYS_FS_HANDLE_INVALID)
    {// File not found, so abort
        return false;
    }
    pClient->fileDescr = fp;
    pClient->callbackPos = 0;
    _tftpFlags.bits.bIsClosing = false;

    // Tell remote server about our intention.
    _TFTPSendFileName(mode, (uint8_t *)fileName);

    // Clear all flags.
    _tftpFlags.Val = 0;

    // Remember start tick for this operation.
    tftpcTimer = SYS_TMR_TickCountGet();

    // Depending on mode of operation, remote server will respond with
    // specific block number.
    if ( mode == TFTP_FILE_MODE_READ )
    {
        // Remember that we are reading a file.
        _tftpFlags.bits.bIsReading = true;

        // For read operation, server would respond with data block of 1.
        MutExVar.group2._tftpBlockNumber.Val = 1;

        // Next packet would be the data packet.
        pClient->smState = SM_TFTP_WAIT_FOR_DATA;
    }

    else
    {
        // Remember that we are writing a file.
        _tftpFlags.bits.bIsReading = false;

        // For write operation, server would respond with data block of 0.
        MutExVar.group2._tftpBlockNumber.Val = 0;

        // Next packet would be the ACK packet.
        pClient->smState = SM_TFTP_WAIT_FOR_ACK;
    }

    return true;
}


static TFTP_RESULT TFTPIsGetReady(uint8_t *getData, int *len)
{
    TCPIP_UINT16_VAL opCode;
    TCPIP_UINT16_VAL blockNumber;
    bool bTimeOut;
    TFTP_CLIENT_VARS*   pClient;
    uint32_t    replyPktSize=0;

    pClient = &gTFTPClientDcpt;
    // Check to see if timeout has occurred.
    bTimeOut = false;
    if ( SYS_TMR_TickCountGet() - tftpcTimer >= (TCPIP_TFTPC_CMD_PROCESS_TIMEOUT * SYS_TMR_TickCounterFrequencyGet()) )
    {
        bTimeOut = true;
        tftpcTimer = SYS_TMR_TickCountGet();
    }

    switch(_tftpCmdState)
    {
        case SM_TFTP_WAIT_FOR_DATA:
            // If timeout occurs in this state, it may be because, we have not
            // even received very first data block or some in between block.
            if ( bTimeOut == true )
            {
                bTimeOut = false;
                if ( _tftpRetries++ > (TCPIP_TFTPC_MAX_RETRIES-1) )
                {   // Forget about all previous attempts.
                    _tftpRetries = 1;
                    return TFTP_TIMEOUT;
                }

                // If we have not even received first block, ask application
                // retry.
                if ( MutExVar.group2._tftpBlockNumber.Val == 1u )
                {
                    return TFTP_RETRY;
                }
                else
                {
                    // Block number was already incremented in last ACK attempt,
                    // so decrement it.
                    MutExVar.group2._tftpBlockNumber.Val--;

                    // Do it.
                    _tftpCmdState = SM_TFTP_SEND_ACK;
                    break;
                }
            }
            tftpcTimer = SYS_TMR_TickCountGet();
            replyPktSize = TCPIP_UDP_GetIsReady(pClient->hSocket);
            // For Read operation, server will respond with data block.
            if ( !replyPktSize)
            {
                break;
            }

            // Get opCode
            TCPIP_UDP_Get(pClient->hSocket,&opCode.v[1]);
            TCPIP_UDP_Get(pClient->hSocket,&opCode.v[0]);

            // Get block number.
            TCPIP_UDP_Get(pClient->hSocket,&blockNumber.v[1]);
            TCPIP_UDP_Get(pClient->hSocket,&blockNumber.v[0]);
            
            // get the data array from the socket  
            *len = TCPIP_UDP_ArrayGet(pClient->hSocket,getData,replyPktSize-4);
            
            // In order to read file, this must be data with block number of 0.
            if ( opCode.Val == (uint16_t)TFTP_OPCODE_DATA )
            {
                // Make sure that this is not a duplicate block.
                if ( MutExVar.group2._tftpBlockNumber.Val == blockNumber.Val )
                {
                    // Mark that we have not acked this block.
                    _tftpFlags.bits.bIsAcked = false;

                    // Since we have a packet, forget about previous retry count.
                    _tftpRetries = 1;

                   _tftpCmdState = SM_TFTP_SEND_ACK;
                    if(replyPktSize < (TCPIP_TFTP_CLIENT_MAX_BUFFER_SIZE-1))
                    {
                        _tftpFlags.bits.bIsClosing =  true;
                        _tftpCmdState = SM_TFTP_SEND_LAST_ACK;
                    }
                    return TFTP_OK;
                }

                // If received block has already been received, simply ack it
                // so that Server can "get over" it and send next block.
                else if ( MutExVar.group2._tftpBlockNumber.Val > blockNumber.Val )
                {
                    SYS_CONSOLE_PRINT("TFTPIsGetReady(): Duplicate block %d received - dropping it...\n",blockNumber.Val);
                    MutExVar.group2._tftpDuplicateBlock.Val = blockNumber.Val;
                    _tftpCmdState = SM_TFTP_DUPLICATE_ACK;
                }
            }
            // Discard all unexpected and error blocks.
            TCPIP_UDP_Discard(pClient->hSocket);

            // If this was an error, remember error code for later delivery.
            if ( opCode.Val == (uint16_t)TFTP_OPCODE_ERROR )
            {
                _tftpError = blockNumber.Val;
                return TFTP_ERROR;
            }

            break;

    case SM_TFTP_DUPLICATE_ACK:
        if ( TCPIP_UDP_PutIsReady(pClient->hSocket))
        {
            _TFTPSendAck(MutExVar.group2._tftpDuplicateBlock);
            _tftpCmdState = SM_TFTP_WAIT_FOR_DATA;
        }
        break;

    case SM_TFTP_SEND_LAST_ACK:
            SYS_CONSOLE_PRINT("\r\nTotal Byte received: %d bytes \r\n ",pClient->callbackPos);
    case SM_TFTP_SEND_ACK:
        if ( TCPIP_UDP_PutIsReady(pClient->hSocket) )
        {
            _TFTPSendAck(MutExVar.group2._tftpBlockNumber);

            // This is the next block we are expecting.
            MutExVar.group2._tftpBlockNumber.Val++;

            // Remember that we have already acked current block.
            _tftpFlags.bits.bIsAcked = true;

                if ( _tftpCmdState == SM_TFTP_SEND_LAST_ACK )
                {
                    return TFTP_END_OF_FILE;
                }

            _tftpCmdState = SM_TFTP_WAIT_FOR_DATA;
                SYS_CONSOLE_MESSAGE("#");
            return TFTP_ACK_SEND;

        }
        break;

	// Suppress compiler warnings on unhandled SM_TFTP_WAIT and
	// SM_TFTP_WAIT_FOR_ACK states.
    default:
    	break;
    }
    return TFTP_NOT_READY;
}

static void _TFTPSendAck(TCPIP_UINT16_VAL blockNumber)
{
    TFTP_CLIENT_VARS*   pClient;
    pClient = &gTFTPClientDcpt;
    // Write opCode.
    TCPIP_UDP_Put(pClient->hSocket,0);
    TCPIP_UDP_Put(pClient->hSocket,TFTP_OPCODE_ACK);

    // Write block number for this ack.
    TCPIP_UDP_Put(pClient->hSocket,blockNumber.v[1]);
    TCPIP_UDP_Put(pClient->hSocket,blockNumber.v[0]);

    // Transmit it.
    TCPIP_UDP_Flush(pClient->hSocket);
}


#endif /* TCPIP_STACK_USE_TFTP_CLIENT */

