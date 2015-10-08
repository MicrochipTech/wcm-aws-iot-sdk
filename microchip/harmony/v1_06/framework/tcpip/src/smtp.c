/*******************************************************************************
  Simple Mail Transfer Protocol (SMTP) Client
  Module for Microchip TCP/IP Stack

  Summary:
    ARCFOUR Cryptography Library
    
  Description:
    -Provides ability to send Emails
    -Reference: RFC 2821
*******************************************************************************/

/*******************************************************************************
File Name:  SMTP.c
Copyright © 2012 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
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
#define TCPIP_THIS_MODULE_ID    TCPIP_MODULE_SMTP_CLIENT

#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_STACK_USE_SMTP_CLIENT)


/****************************************************************************
  Section:
	SMTP Client Public Variables
  ***************************************************************************/
// The global set of TCPIP_SMTP_CLIENT_MESSAGE.
// Set these parameters after calling TCPIP_SMTP_UsageBegin successfully.
static TCPIP_SMTP_CLIENT_MESSAGE SMTPClient;	

/****************************************************************************
  Section:
	SMTP Client Internal Variables
  ***************************************************************************/
static IPV4_ADDR SMTPServer;						// IP address of the remote SMTP server
static TCP_SOCKET MySocket = INVALID_SOCKET;	// Socket currently in use by the SMTP client

// State machine for the CR LF Period replacement
// Used by TCPIP_SMTP_Put to transparently replace "\r\n." with "\r\n.."
static union
{
	uint8_t *Pos;
	enum
	{
		CR_PERIOD_SEEK_CR = 0,		// Idle state, waiting for '\r'
		CR_PERIOD_SEEK_LF,			// "\r" has been written, so check next byte for '\n'
		CR_PERIOD_SEEK_PERIOD,		// "\r\n" has been written, so check next byte for '.'
		CR_PERIOD_NEED_INSERTION	// "\r\n." has been written, so an additional '.'
									//   must be written before the next byte.
	} State;
} CRPeriod;

// State of the transport for the SMTP Client
static enum
{
	TRANSPORT_HOME = 0,			// Idle state
	TRANSPORT_BEGIN,			// Preparing to make connection
	TRANSPORT_NAME_RESOLVE,		// Resolving the SMTP server address
	TRANSPORT_OBTAIN_SOCKET,	// Obtaining a socket for the SMTP connection
	TRANSPORT_SOCKET_OBTAINED,	// SMTP connection successful
	TRANSPORT_CLOSE				// STMP socket is closed
} TransportState = TRANSPORT_HOME;

// Message state machine for the SMTP Client
static enum
{
	SMTP_HOME = 0,				// Idle start state for SMTP client (application is preparing message)
	SMTP_HELO,					// HELO is being sent to server
	SMTP_HELO_ACK,				// Received an ACK for the HELO
	SMTP_AUTH_LOGIN,			// Requesting to log in
	SMTP_AUTH_LOGIN_ACK,		// Log in request accepted
	SMTP_AUTH_USERNAME,			// Sending user name
	SMTP_AUTH_USERNAME_ACK,		// User name accepted
	SMTP_AUTH_PASSWORD,			// Sending password
	SMTP_AUTH_PASSWORD_ACK,		// Password was accepted
	SMTP_MAILFROM,				// Sending inital MAIL FROM command
	SMTP_MAILFROM_ACK,			// MAIL FROM was accepted
	SMTP_RCPTTO_INIT,			// Preparing to send RCPT TO
	SMTP_RCPTTO,				// Sending RCPT TO command
	SMTP_RCPTTO_ACK,			// RCPT TO was accepted
	SMTP_RCPTTO_ISDONE,			// Done sending RCPT TO commands
	SMTP_RCPTTOCC_INIT,			// Preparing to send RCPT TO CC commands
	SMTP_RCPTTOCC,				// Sending RCPT TO CC commands
	SMTP_RCPTTOCC_ACK,			// RCPT TO CC was accepted
	SMTP_RCPTTOCC_ISDONE,		// Done sending RCPT TO CC
	SMTP_RCPTTOBCC_INIT,		// Preparing to send RCPT TO BCC commands
	SMTP_RCPTTOBCC,				// Sending RCPT TO BCC commands
	SMTP_RCPTTOBCC_ACK,			// RCPT TO BCC was accepted
	SMTP_RCPTTOBCC_ISDONE,		// Done sending RCPT TO BCC
	SMTP_DATA,					// Sending DATA command
	SMTP_DATA_ACK,				// DATA command accpted
	SMTP_DATA_HEADER,			// Sending message headers
	SMTP_DATA_BODY_INIT,		// Preparing for message body
	SMTP_DATA_BODY,				// Sending message body
	SMTP_DATA_BODY_ACK,			// Message body accepted
	SMTP_QUIT_INIT,				// Sending QUIT command
	SMTP_QUIT					// QUIT accepted, connection closing
} SMTPState;

// State machine for writing the SMTP message headers
static enum
{
	PUTHEADERS_FROM_INIT = 0,	// Preparing to send From header
	PUTHEADERS_FROM,			// Sending From header
	PUTHEADERS_TO_INIT,			// Preparing to send To header
	PUTHEADERS_TO,				// Sending To header
	PUTHEADERS_CC_INIT,			// Preparing to send CC header
	PUTHEADERS_CC,				// Sending CC header
	PUTHEADERS_SUBJECT_INIT,	// Preparing to send Subject header
	PUTHEADERS_SUBJECT,			// Sending Subject header
	PUTHEADERS_OTHER_INIT,		// Preparing to send additional headers
	PUTHEADERS_OTHER,			// Sending additional headers
	PUTHEADERS_DONE				// Done writing all headers
} PutHeadersState;

// State machine for parsing incoming responses
static enum
{
	RX_BYTE_0 = 0,
	RX_BYTE_1,
	RX_BYTE_2,
	RX_BYTE_3,
	RX_SEEK_CR,
	RX_SEEK_LF
} RXParserState;

// Internal flags used by the SMTP Client
static union
{
	uint8_t Val;
	struct
	{
		unsigned char RXSkipResponse:1;
		unsigned char SMTPInUse:1;
		unsigned char SentSuccessfully:1;
		unsigned char ReadyToStart:1;
		unsigned char ReadyToFinish:1;
		unsigned char ConnectedOnce:1;
		unsigned char filler:2;
	} bits;
} SMTPFlags = {0x00};
	
// Response code from server when an error exists
static uint16_t         ResponseCode;

static int              smtpInitCount = 0;

static tcpipSignalHandle smtpTimerHandle = 0;

/****************************************************************************
  Section:
	SMTP Client Internal Function Prototypes
  ***************************************************************************/
static const uint8_t *FindEmailAddress(const uint8_t *str, uint16_t *wLen);

static char* FindEmailServer(const TCPIP_SMTP_CLIENT_MESSAGE* pClient, DNS_RESOLVE_TYPE* pDnsType);

static void TCPIP_SMTP_ClientProcess(void);

static void _SMTPSocketRxSignalHandler(TCP_SOCKET hTCP, TCPIP_NET_HANDLE hNet, TCPIP_TCP_SIGNAL_TYPE sigType, const void* param);


/****************************************************************************
  Section:
	SMTP Function Implementations
  ***************************************************************************/


bool TCPIP_SMTP_ClientInitialize(const TCPIP_STACK_MODULE_CTRL* const stackCtrl, const TCPIP_SMTP_CLIENT_MODULE_CONFIG* pSmtpConfig)
{
    if(stackCtrl->stackAction == TCPIP_STACK_ACTION_IF_UP)
    {  // interface restart
        return true;
    }
    
    // stack init
    if(smtpInitCount == 0)
    {   // first time we run
        // create the SMTP timer
        smtpTimerHandle =_TCPIPStackSignalHandlerRegister(TCPIP_THIS_MODULE_ID, TCPIP_SMTP_ClientTask, TCPIP_SMTP_TASK_TICK_RATE); 
        if(smtpTimerHandle == 0)
        {   // cannot create the SMTP timer
            return false;
        }

    }
    
    // Reset per interface state machine and flags to default values
	
    smtpInitCount++;
	return true;
}

void TCPIP_SMTP_ClientDeinitialize(const TCPIP_STACK_MODULE_CTRL* const stackCtrl)
{
    // if(stackCtrl->stackAction == TCPIP_STACK_ACTION_DEINIT) // stack shut down
    // if(stackCtrl->stackAction == TCPIP_STACK_ACTION_IF_DOWN) // interface down


    if(smtpInitCount > 0)
    {   // we're up and running
        if(stackCtrl->stackAction == TCPIP_STACK_ACTION_DEINIT)
        {   // whole stack is going down
            if(--smtpInitCount == 0)
            {   // all closed
                // release resources
                if(smtpTimerHandle)
                {
                    _TCPIPStackSignalHandlerDeregister(smtpTimerHandle);
                    smtpTimerHandle = 0;
                }
            }
        }
    }
}



/*****************************************************************************
  Function:
	bool TCPIP_SMTP_UsageBegin(void)

  Summary:
	Requests control of the SMTP client module.

  Description:
	Call this function before calling any other SMTP Client APIs.  This 
	function obtains a lock on the SMTP Client, which can only be used by
	one stack application at a time.  Once the application is finished
	with the SMTP client, it must call TCPIP_SMTP_UsageEnd to release control
	of the module to any other waiting applications.
	
	This function initializes all the SMTP state machines and variables
	back to their default state.

  Precondition:
	None

  Parameters:
	None

  Return Values:
	true - The application has successfully obtained control of the module
	false - The SMTP module is in use by another application.  Call 
		TCPIP_SMTP_UsageBegin again later, after returning to the main program loop
  ***************************************************************************/
bool TCPIP_SMTP_UsageBegin(void)
{
	if(SMTPFlags.bits.SMTPInUse)
		return false;

	SMTPFlags.Val = 0x00;
	SMTPFlags.bits.SMTPInUse = true;
	TransportState = TRANSPORT_BEGIN;
	RXParserState = RX_BYTE_0;
	SMTPState = SMTP_HOME;
	memset((void*)&SMTPClient, 0x00, sizeof(SMTPClient));
	SMTPClient.ServerPort = TCPIP_SMTP_SERVER_REMOTE_PORT;
		
	return true;
}

/*****************************************************************************
  Function:
	uint16_t TCPIP_SMTP_UsageEnd(void)

  Summary:
	Releases control of the SMTP client module.

  Description:
	Call this function to release control of the SMTP client module once
	an application is finished using it.  This function releases the lock
	obtained by TCPIP_SMTP_UsageBegin, and frees the SMTP client to be used by 
	another application.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	None

  Return Values:
	SMTP_SUCCESS - A message was successfully sent
	SMTP_RESOLVE_ERROR - The SMTP server could not be resolved
	SMTP_CONNECT_ERROR - The connection to the SMTP server failed or was
		prematurely terminated
	1-199 and 300-399 - The last SMTP server response code
  ***************************************************************************/
uint16_t TCPIP_SMTP_UsageEnd(void)
{
	if(!SMTPFlags.bits.SMTPInUse)
		return 0xFFFF;

	// Release the DNS module, if in use
	
	
	// Release the TCP socket, if in use
	if(MySocket != INVALID_SOCKET)
	{
		TCPIP_TCP_Close(MySocket);
		MySocket = INVALID_SOCKET;
	}
	
	// Release the SMTP module
	SMTPFlags.bits.SMTPInUse = false;
	TransportState = TRANSPORT_HOME;

	if(SMTPFlags.bits.SentSuccessfully)
	{
		return 0;
	}
	else
	{
		return ResponseCode;
	}
}

void TCPIP_SMTP_ClientTask(void)
{
    TCPIP_MODULE_SIGNAL sigPend;

    sigPend = _TCPIPStackModuleSignalGet(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_MASK_ALL);

    if(sigPend != 0)
    { // TMO or RX signals occurred
        TCPIP_SMTP_ClientProcess();
    }

}


// send a signal to the SMTP module that data is available
// no manager alert needed since this normally results as a higher layer (TCP) signal
static void _SMTPSocketRxSignalHandler(TCP_SOCKET hTCP, TCPIP_NET_HANDLE hNet, TCPIP_TCP_SIGNAL_TYPE sigType, const void* param)
{
    if(sigType == TCPIP_TCP_SIGNAL_RX_DATA)
    {
        _TCPIPStackModuleSignalRequest(TCPIP_THIS_MODULE_ID, TCPIP_MODULE_SIGNAL_RX_PENDING, true); 
    }
}



static void TCPIP_SMTP_ClientProcess(void)
{
    uint8_t			    i;
    uint16_t			w;
    uint8_t			    vBase64Buffer[4];
    static uint32_t	    SMTPTimer;

    static uint8_t		RXBuffer[4];
    static const uint8_t *ROMStrPtr, *ROMStrPtr2;
    static const uint8_t *RAMStrPtr;
    static uint16_t		wAddressLength;
    TCPIP_DNS_RESULT    dnsRes;
    DNS_RESOLVE_TYPE    dnsType;

    switch(TransportState)
    {
        case TRANSPORT_HOME:
            // TCPIP_SMTP_UsageBegin() is the only function which will kick
            // the state machine into the next state
            break;

        case TRANSPORT_BEGIN:
            // Wait for the user to program all the pointers and then
            // call TCPIP_SMTP_MailSend()
            if(!SMTPFlags.bits.ReadyToStart)
            {
                break;
            }

            SMTPClient.Server = FindEmailServer(&SMTPClient, &dnsType);
            // See if we found a hostname anywhere which we could resolve
            if(!(SMTPClient.Server))
            {
                ResponseCode = SMTP_RESOLVE_ERROR;
                TransportState = TRANSPORT_HOME;
                break;
            }

            // check for a plain IP address
            if(TCPIP_Helper_StringToIPAddress(SMTPClient.Server, &SMTPServer))
            {
                TransportState = TRANSPORT_OBTAIN_SOCKET;
                break;
            }

            // use DNS to resolve the name
            TCPIP_DNS_Resolve(SMTPClient.Server, dnsType);
            SMTPTimer = SYS_TMR_TickCountGet();

            TransportState++;
            break;

        case TRANSPORT_NAME_RESOLVE:
            // Wait for the DNS server to return the requested IP address
            dnsRes = TCPIP_DNS_IsResolved((const char*)SMTPClient.Server,&SMTPServer);
            if(dnsRes == DNS_RES_PENDING)
            {
                break;
            }
            if(dnsRes < 0)
            {   // some error occurred
                ResponseCode = SMTP_RESOLVE_ERROR;
                TransportState = TRANSPORT_HOME;
                break;
            }
            
            // DNS_RES_OK
            TransportState++;
            // No need to break here

        case TRANSPORT_OBTAIN_SOCKET:
            // Connect a TCP socket to the remote SMTP server
            MySocket = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4, SMTPClient.ServerPort, (IP_MULTI_ADDRESS*)&SMTPServer.Val);

            // Abort operation if no TCP socket could be opened.
            // If this ever happens, you need to update your tcp_config.h
            if(MySocket == INVALID_SOCKET)
                break;

            TCPIP_TCP_SignalHandlerRegister(MySocket, TCPIP_TCP_SIGNAL_RX_DATA, _SMTPSocketRxSignalHandler, 0);

            TransportState++;
            SMTPTimer = SYS_TMR_TickCountGet();

            // No break; fall into TRANSPORT_SOCKET_OBTAINED
			
        case TRANSPORT_SOCKET_OBTAINED:
            if(!TCPIP_TCP_IsConnected(MySocket))
            {
                // Don't stick around in the wrong state if the
                // server was connected, but then disconnected us.
                // Also time out if we can't establish the connection
                // to the SMTP server
                if(SMTPFlags.bits.ConnectedOnce || ((SYS_TMR_TickCountGet()-SMTPTimer) > (TCPIP_SMTP_SERVER_REPLY_TIMEOUT * SYS_TMR_TickCounterFrequencyGet())))
                {
                    ResponseCode = SMTP_CONNECT_ERROR;
                    TransportState = TRANSPORT_CLOSE;
                }

                break;
            }
            SMTPFlags.bits.ConnectedOnce = true;

            // See if the server sent us anything
            while(TCPIP_TCP_GetIsReady(MySocket))
            {
                TCPIP_TCP_Get(MySocket, &i);
                switch(RXParserState)
                {
                    case RX_BYTE_0:
                    case RX_BYTE_1:
                    case RX_BYTE_2:
                        RXBuffer[RXParserState] = i;
                        RXParserState++;
                        break;
	
                    case RX_BYTE_3:
                        switch(i)
                        {
                            case ' ':
                                SMTPFlags.bits.RXSkipResponse = false;
                                RXParserState++;
                                break;
                            case '-':
                                SMTPFlags.bits.RXSkipResponse = true;
                                RXParserState++;
                                break;
                            case '\r':
                                RXParserState = RX_SEEK_LF;
                                break;
                        }
                        break;
	
					case RX_SEEK_CR:
                                            if(i == '\r')
                                                    RXParserState++;
                                            break;
	
					case RX_SEEK_LF:
						// If we received the whole command
						if(i == '\n')
						{
							RXParserState = RX_BYTE_0;

							if(!SMTPFlags.bits.RXSkipResponse)
							{
								// The server sent us a response code
								// Null terminate the ASCII reponse code so we can convert it to an integer
								RXBuffer[3] = 0;
								ResponseCode = atoi((char*)RXBuffer);

								// Handle the response
								switch(SMTPState)
								{
									case SMTP_HELO_ACK:
										if(ResponseCode >= 200u && ResponseCode <= 299u)
										{
											if(SMTPClient.Username)
												SMTPState = SMTP_AUTH_LOGIN;
											else
												SMTPState = SMTP_MAILFROM;
										}
										else
											SMTPState = SMTP_QUIT_INIT;
										break;


									case SMTP_AUTH_LOGIN_ACK:
									case SMTP_AUTH_USERNAME_ACK:
										if(ResponseCode == 334u)
											SMTPState++;
										else
											SMTPState = SMTP_QUIT_INIT;
										break;

									case SMTP_AUTH_PASSWORD_ACK:
										if(ResponseCode == 235u)
											SMTPState++;
										else
											SMTPState = SMTP_QUIT_INIT;
										break;

									case SMTP_HOME:
									case SMTP_MAILFROM_ACK:
									case SMTP_RCPTTO_ACK:
									case SMTP_RCPTTOCC_ACK:
									case SMTP_RCPTTOBCC_ACK:
										if(ResponseCode >= 200u && ResponseCode <= 299u)
											SMTPState++;
										else
											SMTPState = SMTP_QUIT_INIT;
										break;
							
									case SMTP_DATA_ACK:
										if(ResponseCode == 354u)
											SMTPState++;
										else
											SMTPState = SMTP_QUIT_INIT;
										break;
							
									case SMTP_DATA_BODY_ACK:
										if(ResponseCode >= 200u && ResponseCode <= 299u)
											SMTPFlags.bits.SentSuccessfully = true;
							
										SMTPState = SMTP_QUIT_INIT;
										break;

									// Default case needed to supress compiler diagnostics
									default:
										break;
								}
							}
						}
						else if(i != '\r')
							RXParserState--;
	
						break;
				}
			}

			// Generate new data in the TX buffer, as needed, if possible
			if(TCPIP_TCP_PutIsReady(MySocket) < 64u)
				break;

			switch(SMTPState)
			{
				case SMTP_HELO:
					if(SMTPClient.Username == NULL)
						TCPIP_TCP_StringPut(MySocket, (uint8_t*)"HELO MCHPBOARD\r\n");
					else    
						TCPIP_TCP_StringPut(MySocket, (uint8_t*)"EHLO MCHPBOARD\r\n");
					TCPIP_TCP_Flush(MySocket);
					SMTPState++;
					break;

				case SMTP_AUTH_LOGIN:
					// Note: This state is only entered from SMTP_HELO_ACK if the application 
					// has specified a Username to use (SMTPClient.Username is non-NULL)
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)"AUTH LOGIN\r\n");
					TCPIP_TCP_Flush(MySocket);
					SMTPState++;
					break;

				case SMTP_AUTH_USERNAME:
					// Base 64 encode and transmit the username.
                    RAMStrPtr = (uint8_t*)SMTPClient.Username;
                    w = strlen((char*)RAMStrPtr);

					while(w)
					{
						i = 0;
						while((i < w) && (i < sizeof(vBase64Buffer)*3/4))
						{
                            vBase64Buffer[i] = *RAMStrPtr++;
							i++;
						}
						w -= i;					
						TCPIP_Helper_Base64Encode(vBase64Buffer, i, vBase64Buffer, sizeof(vBase64Buffer));
						TCPIP_TCP_ArrayPut(MySocket, vBase64Buffer, sizeof(vBase64Buffer));
					}
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\n");
					TCPIP_TCP_Flush(MySocket);
					SMTPState++;
					break;

				case SMTP_AUTH_PASSWORD:
					// Base 64 encode and transmit the password
                    RAMStrPtr = (uint8_t*)SMTPClient.Password;
                    w = strlen((char*)RAMStrPtr);

					while(w)
					{
						i = 0;
						while((i < w) && (i < sizeof(vBase64Buffer)*3/4))
						{
                            vBase64Buffer[i] = *RAMStrPtr++;
							i++;
						}
						w -= i;					
						TCPIP_Helper_Base64Encode(vBase64Buffer, i, vBase64Buffer, sizeof(vBase64Buffer));
						TCPIP_TCP_ArrayPut(MySocket, vBase64Buffer, sizeof(vBase64Buffer));
					}
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\n");
					TCPIP_TCP_Flush(MySocket);
					SMTPState++;
					break;

				case SMTP_MAILFROM:
					// Send MAIL FROM header.  Note that this is for the SMTP server validation, 
					// not what actually will be displayed in the recipients mail client as a 
					// return address.
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)"MAIL FROM:<");
                    RAMStrPtr = FindEmailAddress((uint8_t*)SMTPClient.From, &wAddressLength);
                    TCPIP_TCP_ArrayPut(MySocket, RAMStrPtr, wAddressLength);
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)">\r\n");
					TCPIP_TCP_Flush(MySocket);
					SMTPState++;
					break;

				case SMTP_RCPTTO_INIT:
					// See if there are any (To) recipients to process
					if(SMTPClient.To)
					{
						RAMStrPtr = FindEmailAddress((uint8_t*)SMTPClient.To, &wAddressLength);
						if(wAddressLength)
						{
							SMTPState = SMTP_RCPTTO;
							break;
						}
					}
					
					SMTPState = SMTP_RCPTTOCC_INIT;
					break;

				case SMTP_RCPTTO:
				case SMTP_RCPTTOCC:
				case SMTP_RCPTTOBCC:
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)"RCPT TO:<");
                    TCPIP_TCP_ArrayPut(MySocket, RAMStrPtr, wAddressLength);
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)">\r\n");
					TCPIP_TCP_Flush(MySocket);
					SMTPState++;
					break;

				case SMTP_RCPTTO_ISDONE:
					// See if we have any more (To) recipients to process
					// If we do, we must roll back a couple of states
                    RAMStrPtr = FindEmailAddress(RAMStrPtr+wAddressLength, &wAddressLength);
	
					if(wAddressLength)
					{
						SMTPState = SMTP_RCPTTO;
						break;
					}

					// All done with To field
					SMTPState++;
					//No break

				case SMTP_RCPTTOCC_INIT:
					// See if there are any Carbon Copy (CC) recipients to process
					if(SMTPClient.CC)
					{
						RAMStrPtr = FindEmailAddress((uint8_t*)SMTPClient.CC, &wAddressLength);
						if(wAddressLength)
						{
							SMTPState = SMTP_RCPTTOCC;
							break;
						}
					}
					
					SMTPState = SMTP_RCPTTOBCC_INIT;
					break;

				case SMTP_RCPTTOCC_ISDONE:
					// See if we have any more Carbon Copy (CC) recipients to process
					// If we do, we must roll back a couple of states
                    RAMStrPtr = FindEmailAddress(RAMStrPtr+wAddressLength, &wAddressLength);

					if(wAddressLength)
					{
						SMTPState = SMTP_RCPTTOCC;
						break;
					}

					// All done with CC field
					SMTPState++;
					//No break

				case SMTP_RCPTTOBCC_INIT:
					// See if there are any Blind Carbon Copy (BCC) recipients to process
					if(SMTPClient.BCC)
					{
						RAMStrPtr = FindEmailAddress((uint8_t*)SMTPClient.BCC, &wAddressLength);
						if(wAddressLength)
						{
							SMTPState = SMTP_RCPTTOBCC;
							break;
						}
					}

					// All done with BCC field
					SMTPState = SMTP_DATA;
					break;

				case SMTP_RCPTTOBCC_ISDONE:
					// See if we have any more Blind Carbon Copy (CC) recipients to process
					// If we do, we must roll back a couple of states
                    RAMStrPtr = FindEmailAddress(RAMStrPtr+wAddressLength, &wAddressLength);

					if(wAddressLength)
					{
						SMTPState = SMTP_RCPTTOBCC;
						break;
					}

					// All done with BCC field
					SMTPState++;
					//No break

				case SMTP_DATA:
					TCPIP_TCP_StringPut(MySocket, (uint8_t*)"DATA\r\n");
					SMTPState++;
					PutHeadersState = PUTHEADERS_FROM_INIT;
					TCPIP_TCP_Flush(MySocket);
					break;

				case SMTP_DATA_HEADER:
					while((PutHeadersState != PUTHEADERS_DONE) && (TCPIP_TCP_PutIsReady(MySocket) > 64u))
					{
						switch(PutHeadersState)
						{
							case PUTHEADERS_FROM_INIT:
								if(SMTPClient.From)
								{
									PutHeadersState = PUTHEADERS_FROM;
									TCPIP_TCP_StringPut(MySocket, (uint8_t*)"From: ");
								}
								else
								{
									PutHeadersState = PUTHEADERS_TO_INIT;
								}
								break;
								
							case PUTHEADERS_FROM:
                                SMTPClient.From = (char*)TCPIP_TCP_StringPut(MySocket, (uint8_t*)SMTPClient.From);
                                if(*SMTPClient.From == 0u)
                                    PutHeadersState = PUTHEADERS_TO_INIT;
								break;

							case PUTHEADERS_TO_INIT:
								if(SMTPClient.To)
								{
									PutHeadersState = PUTHEADERS_TO;
									TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\nTo: ");
								}
								else
								{
									PutHeadersState = PUTHEADERS_CC_INIT;
								}
								break;
								
							case PUTHEADERS_TO:
                                SMTPClient.To = (char*)TCPIP_TCP_StringPut(MySocket, (uint8_t*)SMTPClient.To);
                                if(*SMTPClient.To == 0u)
                                    PutHeadersState = PUTHEADERS_CC_INIT;
								break;

							case PUTHEADERS_CC_INIT:
								if(SMTPClient.CC)
								{
									PutHeadersState = PUTHEADERS_CC;
									TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\nCC: ");
								}
								else
								{
									PutHeadersState = PUTHEADERS_SUBJECT_INIT;
								}
								break;
								
							case PUTHEADERS_CC:
                                SMTPClient.CC = (char*)TCPIP_TCP_StringPut(MySocket, (uint8_t*)SMTPClient.CC);
                                if(*SMTPClient.CC == 0u)
                                    PutHeadersState = PUTHEADERS_SUBJECT_INIT;
								break;

							case PUTHEADERS_SUBJECT_INIT:
								if(SMTPClient.Subject)
								{
									PutHeadersState = PUTHEADERS_SUBJECT;
									TCPIP_TCP_StringPut(MySocket, (uint8_t*)"\r\nSubject: ");
								}
								else
								{
									PutHeadersState = PUTHEADERS_OTHER_INIT;
								}
								break;
								
							case PUTHEADERS_SUBJECT:
                                SMTPClient.Subject = (char*)TCPIP_TCP_StringPut(MySocket, (uint8_t*)SMTPClient.Subject);
                                if(*SMTPClient.Subject == 0u)
                                    PutHeadersState = PUTHEADERS_OTHER_INIT;
								break;

							case PUTHEADERS_OTHER_INIT:
								TCPIP_TCP_ArrayPut(MySocket, (uint8_t*)"\r\n", 2);
								if(SMTPClient.OtherHeaders)
								{
									PutHeadersState = PUTHEADERS_OTHER;
								}
								else
								{
									TCPIP_TCP_ArrayPut(MySocket, (uint8_t*)"\r\n", 2);
									PutHeadersState = PUTHEADERS_DONE;
									SMTPState++;
								}
								break;
								
							case PUTHEADERS_OTHER:
                                SMTPClient.OtherHeaders = (char*)TCPIP_TCP_StringPut(MySocket, (uint8_t*)SMTPClient.OtherHeaders);
                                if(*SMTPClient.OtherHeaders == 0u)
                                {
                                    TCPIP_TCP_ArrayPut(MySocket, (uint8_t*)"\r\n", 2);
                                    PutHeadersState = PUTHEADERS_DONE;
                                    SMTPState++;
                                }
								break;
							
							// Default case needed to supress compiler diagnostics
							default:
								break;
						}
					}
					TCPIP_TCP_Flush(MySocket);
					break;
		
				case SMTP_DATA_BODY_INIT:
					SMTPState++;
					RAMStrPtr = (uint8_t*)SMTPClient.Body;
					ROMStrPtr2 = (const uint8_t*)"\r\n.\r\n";
					CRPeriod.Pos = NULL;
					if(RAMStrPtr)
						CRPeriod.Pos = (uint8_t*)strstr((char*)RAMStrPtr, (const char*)"\r\n.");
					// No break here
		
				case SMTP_DATA_BODY:
					if(SMTPClient.Body)
					{
						if(*ROMStrPtr2)
						{
							// Put the application data, doing the transparancy replacement of "\r\n." with "\r\n.."
							while(CRPeriod.Pos)
							{
								CRPeriod.Pos += 3;
								RAMStrPtr += TCPIP_TCP_ArrayPut(MySocket, RAMStrPtr, CRPeriod.Pos-RAMStrPtr);
								if(RAMStrPtr == CRPeriod.Pos)
								{
									if(!TCPIP_TCP_Put(MySocket, '.'))
									{
										CRPeriod.Pos -= 3;
										break;
									}
								}
								else
								{
									CRPeriod.Pos -= 3;
									break;
								}
								CRPeriod.Pos = (uint8_t*)strstr((char*)RAMStrPtr, (const char*)"\r\n.");
							}
							
							// If we get down here, either all replacements have been made or there is no remaining space in the TCP output buffer
							RAMStrPtr = TCPIP_TCP_StringPut(MySocket, RAMStrPtr);
							ROMStrPtr2 = TCPIP_TCP_StringPut(MySocket, (uint8_t*)ROMStrPtr2);
							TCPIP_TCP_Flush(MySocket);
						}
					}
					else
					{
						if(SMTPFlags.bits.ReadyToFinish)
						{
							if(*ROMStrPtr2)
							{
								ROMStrPtr2 = TCPIP_TCP_StringPut(MySocket, (uint8_t*)ROMStrPtr2);
								TCPIP_TCP_Flush(MySocket);
							}
		
						}
					}

					if(*ROMStrPtr2 == 0u)
					{
						SMTPState++;
					}
					break;
		
				case SMTP_QUIT_INIT:
					SMTPState++;
					ROMStrPtr = (const uint8_t*)"QUIT\r\n";
					// No break here

				case SMTP_QUIT:
					if(*ROMStrPtr)
					{
						ROMStrPtr = TCPIP_TCP_StringPut(MySocket, (uint8_t*)ROMStrPtr);
						TCPIP_TCP_Flush(MySocket);
					}

					if(*ROMStrPtr == 0u)
					{
						TransportState = TRANSPORT_CLOSE;
					}
					break;
				
				// Default case needed to supress compiler diagnostics
				default:
					break;
			}
			break;

		case TRANSPORT_CLOSE:
			// Close the socket so it can be used by other modules
			TCPIP_TCP_Close(MySocket);
			MySocket = INVALID_SOCKET;

			// Go back to doing nothing
			TransportState = TRANSPORT_HOME;
			break;
	}
}

/*****************************************************************************
  Function:
	void TCPIP_SMTP_MailSend(TCPIP_SMTP_CLIENT_MESSAGE* smtpClientMessage)

  Summary:
	Initializes the message sending process.

  Description:
	This function starts the state machine that performs the actual
	transmission of the message.  Call this function after all the fields
	in SMTPClient have been set.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	smtpClientMessage   - pointer to a TCPIP_SMTP_CLIENT_MESSAGE structure that configures
                          the SMTP Client to send an e-mail to

  Returns:
	None

  Note:
    The fileds pointed by the smtpClientMessage have to be non-volatile
    until the SMTP send mail process is completed!
    
  ***************************************************************************/
void TCPIP_SMTP_MailSend(TCPIP_SMTP_CLIENT_MESSAGE* smtpClientMessage)
{
    memcpy(&SMTPClient, smtpClientMessage, sizeof(SMTPClient));	

	SMTPFlags.bits.ReadyToStart = true;
}

/*****************************************************************************
  Function:
	bool TCPIP_SMTP_IsBusy(void)

  Summary:
	Determines if the SMTP client is busy.

  Description:
	Call this function to determine if the SMTP client is busy performing
	background tasks.  This function should be called after any call to 
	TCPIP_SMTP_MailSend, TCPIP_SMTP_PutIsDone to determine if the stack has finished
	performing its internal tasks.  It should also be called prior to any
	call to TCPIP_SMTP_IsPutReady to verify that the SMTP client has not
	prematurely disconnected.  When this function returns false, the next
	call should be to TCPIP_SMTP_UsageEnd to release the module and obtain the
	status code for the operation.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	None

  Return Values:
	true - The SMTP Client is busy with internal tasks or sending an 
		on-the-fly message.
	false - The SMTP Client is terminated and is ready to be released.
  ***************************************************************************/
bool TCPIP_SMTP_IsBusy(void)
{
	return TransportState != TRANSPORT_HOME;
}

/*****************************************************************************
  Function:
	uint16_t TCPIP_SMTP_IsPutReady(void)

  Summary:
	Determines how much data can be written to the SMTP client.

  Description:
	Use this function to determine how much data can be written to the SMTP 
	client when generating an on-the-fly message.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call, and an on-the-fly 
	message is being generated.  This requires that TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.

  Parameters:
	None

  Returns:
	The number of free bytes the SMTP TX FIFO.

  Remarks:
	This function should only be called externally when the SMTP client is
	generating an on-the-fly message.  (That is, TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.)
  ***************************************************************************/
uint16_t TCPIP_SMTP_IsPutReady(void)
{
	if(SMTPState != SMTP_DATA_BODY)
		return 0;

	return TCPIP_TCP_PutIsReady(MySocket);	
}

/*****************************************************************************
  Function:
	bool TCPIP_SMTP_Put(char c)

  Description:
	Writes a single byte to the SMTP client.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	c - The byte to be written

  Return Values:
	true - The byte was successfully written
	false - The byte was not written, most likely because the buffer was full

  Remarks:
	This function should only be called externally when the SMTP client is
	generating an on-the-fly message.  (That is, TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.)
  ***************************************************************************/
bool TCPIP_SMTP_Put(char c)
{
	if(CRPeriod.State == CR_PERIOD_NEED_INSERTION)
	{
		if(TCPIP_TCP_Put(MySocket, '.'))
			CRPeriod.State = CR_PERIOD_SEEK_CR;
		else
			return false;
	}

	switch(CRPeriod.State)
	{
		case CR_PERIOD_SEEK_CR:
			if(c == '\r')
				CRPeriod.State++;
			break;

		case CR_PERIOD_SEEK_LF:
			if(c == '\n')
				CRPeriod.State++;
			else if(c != '\r')
				CRPeriod.State--;
			break;

		case CR_PERIOD_SEEK_PERIOD:
			if(c == '.')
				CRPeriod.State++;	// CR_PERIOD_NEED_INSERTION
			else if(c == '\r')
				CRPeriod.State--;
			else
				CRPeriod.State = CR_PERIOD_SEEK_CR;
			break;
		
		// Default case needed to supress compiler diagnostics 
		// (CR_PERIOD_NEED_INSERTION state already handled above)
		default:
			break;
	}

	if(!TCPIP_TCP_Put(MySocket, c))
		return false;

	return true;
}

/*****************************************************************************
  Function:
	uint16_t TCPIP_SMTP_ArrayPut(uint8_t* Data, uint16_t Len)

  Description:
	Writes a series of bytes to the SMTP client.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	Data - The data to be written
	Len - How many bytes should be written

  Returns:
	The number of bytes written.  If less than Len, then the TX FIFO became
	full before all bytes could be written.
	
  Remarks:
	This function should only be called externally when the SMTP client is
	generating an on-the-fly message.  (That is, TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.)
	
  Internal:
	TCPIP_SMTP_Put must be used instead of TCPIP_TCP_ArrayPut because "\r\n." must be
	transparently replaced by "\r\n..".
  ***************************************************************************/
uint16_t TCPIP_SMTP_ArrayPut(uint8_t* Data, uint16_t Len)
{
	uint16_t result = 0;

	while(Len--)
	{
		if(TCPIP_SMTP_Put(*Data++))
		{
			result++;
		}
		else
		{
			Data--;
			break;
		}
	}

	return result;
}


/*****************************************************************************
  Function:
	uint16_t TCPIP_SMTP_StringPut(char* Data)

  Description:
	Writes a string to the SMTP client.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	Data - The data to be written

  Returns:
	The number of bytes written.  If less than the length of Data, then the 
	TX FIFO became full before all bytes could be written.
	
  Remarks:
	This function should only be called externally when the SMTP client is
	generating an on-the-fly message.  (That is, TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.)
	
  Internal:
	TCPIP_SMTP_Put must be used instead of TCPIP_TCP_StringPut because "\r\n." must be
	transparently replaced by "\r\n..".
  ***************************************************************************/
uint16_t TCPIP_SMTP_StringPut(char* Data)
{
	uint16_t result = 0;

	while(*Data)
	{
		if(TCPIP_SMTP_Put(*Data++))
		{
			result++;
		}
		else
		{
			Data--;
			break;
		}
	}

	return result;
}


/*****************************************************************************
  Function:
	void TCPIP_SMTP_Flush(void)

  Description:
	Flushes the SMTP socket and forces all data to be sent.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	None

  Returns:
	None
	
  Remarks:
	This function should only be called externally when the SMTP client is
	generating an on-the-fly message.  (That is, TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.)
  ***************************************************************************/
void TCPIP_SMTP_Flush(void)
{
	TCPIP_TCP_Flush(MySocket);
}

/*****************************************************************************
  Function:
	void TCPIP_SMTP_PutIsDone(void)

  Description:
	Indicates that the on-the-fly message is complete.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call, and the SMTP client is
	generated an on-the-fly message.  (That is, TCPIP_SMTP_MailSend was called
	with SMTPClient.Body set to NULL.)

  Parameters:
	None

  Returns:
	None
  ***************************************************************************/
void TCPIP_SMTP_PutIsDone(void)
{
	SMTPFlags.bits.ReadyToFinish = true;
}

/*****************************************************************************
  Function:
	static uint8_t *FindEmailAddress(uint8_t *str, uint16_t *wLen)

  Summary:
	Searches a string for an e-mail address.

  Description:
	This function locates an e-mail address in a string.  It is used 
	internally by the SMTP client to parse out the actual address from
	the From and To strings so that the MAIL FROM and RCPT TO commands
	can be sent to the SMTP server.

  Precondition:
	TCPIP_SMTP_UsageBegin returned true on a previous call.

  Parameters:
	str - The string in which to search for an e-mail address
	wLen - the length of str

  Returns:
	A pointer to the e-mail address
  ***************************************************************************/
static const uint8_t *FindEmailAddress(const uint8_t *str, uint16_t *wLen)
{
	const uint8_t *lpStart;
	uint8_t c;
	union
	{
		uint8_t Val;
		struct
		{
			uint8_t FoundOpenBracket	: 1;
			uint8_t FoundAt			: 1;
		} bits;
	} ParseStates;

	lpStart = str;
	*wLen = 0x0000;
	ParseStates.Val = 0x00;

	while((c = *str++))
	{	
		if(c == '<')
		{
			ParseStates.bits.FoundOpenBracket = 1;
			lpStart = str;
			*wLen = -1;
		}
		else if(c == '@')
			ParseStates.bits.FoundAt = 1;


		if(	!ParseStates.bits.FoundOpenBracket &&
			!ParseStates.bits.FoundAt &&
			(c == ' ' || c == ','))
		{
			lpStart = str;
			continue;
		}
		else if(c == ',')
			break;

		if(ParseStates.bits.FoundOpenBracket && ParseStates.bits.FoundAt)
		{
			if(c == '>')
				break;
		}
		
		// Advance to next character
		*wLen += 1;
	}

	if(!ParseStates.bits.FoundAt)
		*wLen = 0;

	return lpStart;
}

// Obtain the IP address associated with the SMTP mail server
static char* FindEmailServer(const TCPIP_SMTP_CLIENT_MESSAGE* pClient, DNS_RESOLVE_TYPE* pDnsType)
{
    char*   dnsServer;


    if(pClient->Server != 0)
    {
        *pDnsType = DNS_TYPE_A;
        return pClient->Server;
    }

    // If no mail server, try to send the mail
    // directly to the destination SMTP server
    *pDnsType = DNS_TYPE_MX;

    if(pClient->To && (dnsServer = strchr(pClient->To, '@')) != 0)
    {
        return dnsServer++;
    }

    if(pClient->CC && (dnsServer = strchr(pClient->CC, '@')) != 0)
    {
        return dnsServer++;
    }

    if(pClient->BCC && (dnsServer = strchr(pClient->BCC, '@')) != 0)
    {
        return dnsServer++;
    }

    // no hostname found
    return 0;
}

#endif //#if defined(TCPIP_STACK_USE_SMTP_CLIENT)
