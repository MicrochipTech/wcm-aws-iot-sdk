#include "../net_pres.h"
#include "../net_pres_encryptionproviderapi.h"
#include "../net_pres_socketapi.h"
#include "../net_pres_transportapi.h"
#include "net_pres_local.h"
#include "string.h"

NET_PRES_InternalData sNetPresData;
NET_PRES_SocketData sNetPresSockets[NET_PRES_NUM_SOCKETS];

SYS_MODULE_OBJ NET_PRES_Initialize( const SYS_MODULE_INDEX index,
                                           const SYS_MODULE_INIT * const init )
{
    if (sNetPresData.initialized || !init)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    NET_PRES_INIT_DATA *pInitData = (NET_PRES_INIT_DATA*)init;
    
    if (pInitData->numLayers > NET_PRES_NUM_INSTANCE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    

    memset(&sNetPresData, 0, sizeof(NET_PRES_InternalData));
    if (OSAL_MUTEX_Create(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
    {
        return SYS_MODULE_OBJ_INVALID;
    }
    memset(&sNetPresSockets, 0, sizeof(NET_PRES_SocketData) * NET_PRES_NUM_SOCKETS);
    sNetPresData.initialized = true;
    sNetPresData.numLayers = pInitData->numLayers;
    uint8_t x;
    for (x = 0; x < sNetPresData.numLayers; x++)
    {
        if (pInitData->pInitData[x].pTransObject_ss)
        {
            memcpy(&sNetPresData.transObjectSS[x], pInitData->pInitData[x].pTransObject_ss, sizeof(NET_PRES_TransportObject));
        }
        if (pInitData->pInitData[x].pTransObject_sc)
        {
            memcpy(&sNetPresData.transObjectSC[x], pInitData->pInitData[x].pTransObject_sc, sizeof(NET_PRES_TransportObject));
        }
        if (pInitData->pInitData[x].pTransObject_ds)
        {
            memcpy(&sNetPresData.transObjectDS[x], pInitData->pInitData[x].pTransObject_ds, sizeof(NET_PRES_TransportObject));
        }
        if (pInitData->pInitData[x].pTransObject_dc)
        {
            memcpy(&sNetPresData.transObjectDC[x], pInitData->pInitData[x].pTransObject_dc, sizeof(NET_PRES_TransportObject));
        }
        if (pInitData->pInitData[x].pProvObject_ss)
        {
            memcpy(&sNetPresData.encProvObjectSS[x], pInitData->pInitData[x].pProvObject_ss, sizeof(NET_PRES_EncProviderObject));
        }        
        if (pInitData->pInitData[x].pProvObject_sc)
        {
            memcpy(&sNetPresData.encProvObjectSC[x], pInitData->pInitData[x].pProvObject_sc, sizeof(NET_PRES_EncProviderObject));
        }        
        if (pInitData->pInitData[x].pProvObject_ds)
        {
            memcpy(&sNetPresData.encProvObjectDS[x], pInitData->pInitData[x].pProvObject_ds, sizeof(NET_PRES_EncProviderObject));
        }        
        if (pInitData->pInitData[x].pProvObject_dc)
        {
            memcpy(&sNetPresData.encProvObjectDC[x], pInitData->pInitData[x].pProvObject_dc, sizeof(NET_PRES_EncProviderObject));
        }        
    }
    return (SYS_MODULE_OBJ)&sNetPresData;
}

void NET_PRES_Deinitialize(SYS_MODULE_OBJ obj)
{
    if (!sNetPresData.initialized)
    {
        return;
    }
    
    uint8_t x;
    // Make sure all the sockets are closed down
    for (x = 0; x < NET_PRES_NUM_SOCKETS; x++)
    {
        if (sNetPresSockets[x].inUse)
        {
            if ((sNetPresSockets[x].socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED)
            {
                NET_PRES_EncProviderConnectionClose fpClose = sNetPresSockets[x].provObject->fpClose;
                NET_PRES_TransClose fpTransClose = sNetPresSockets[x].transObject->fpClose;
                if (fpClose != NULL)
                {
                    (*fpClose)(sNetPresSockets[x].providerData);
                }
                if (fpTransClose)
                {
                    (*fpTransClose)(sNetPresSockets[x].transHandle);
                }
                sNetPresSockets[x].inUse = false;
            }
        }
    }
    
    // Make sure all the encryption providers are down
    for (x = 0; x < NET_PRES_NUM_INSTANCE; x++)
    {
        if (sNetPresData.encProvObjectSS[x].fpDeinit != NULL)
        {
            (*sNetPresData.encProvObjectSS[x].fpDeinit)();
        }
        if (sNetPresData.encProvObjectSC[x].fpDeinit != NULL)
        {
            (*sNetPresData.encProvObjectSC[x].fpDeinit)();
        }
        if (sNetPresData.encProvObjectDS[x].fpDeinit != NULL)
        {
            (*sNetPresData.encProvObjectDS[x].fpDeinit)();
        }
        if (sNetPresData.encProvObjectDC[x].fpDeinit != NULL)
        {
            (*sNetPresData.encProvObjectDC[x].fpDeinit)();
        }
    }
    
    if (OSAL_MUTEX_Delete(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
    {
        
    }
    memset(&sNetPresData, 0, sizeof(NET_PRES_InternalData));
    memset(&sNetPresSockets, 0, sizeof(NET_PRES_SocketData) * NET_PRES_NUM_SOCKETS);
}

void NET_PRES_Reinitialize(SYS_MODULE_OBJ obj, const SYS_MODULE_INIT * const init)
{
    NET_PRES_Deinitialize(obj);
    NET_PRES_Initialize(0, init);
}

void NET_PRES_Tasks(SYS_MODULE_OBJ obj)
{
    uint8_t x;
    for (x = 0; x < NET_PRES_NUM_SOCKETS; x++)
    {
        if (sNetPresSockets[x].inUse && ((sNetPresSockets[x].socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED))
        {
            // Check the state of the socket and then pump it if necessary.
            switch (sNetPresSockets[x].status)
            {
                case NET_PRES_ENC_SS_WAITING_TO_START_NEGOTIATION:
                {
                    // First thing is to check if the connection is connected.
                    if (!sNetPresSockets[x].transObject->fpIsConnected(sNetPresSockets[x].transHandle))
                    {
                        break;
                    }
                    // Next check to see if the provider has been initialized
                    if (OSAL_MUTEX_Lock(&sNetPresData.presMutex, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
                    {
                        continue;
                    }
                    if (!(*sNetPresSockets[x].provObject->fpIsInited)())
                    {
                        if (!(*sNetPresSockets[x].provObject->fpInit)(sNetPresSockets[x].transObject))
                        {
                            sNetPresSockets[x].status = NET_PRES_ENC_SS_FAILED;
                            if (OSAL_MUTEX_Unlock(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
                            {
                                continue;
                            }
                            continue;
                        }
                    }
                    if (OSAL_MUTEX_Unlock(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
                    {
                        continue;
                    }
                    if (!(*sNetPresSockets[x].provObject->fpOpen)(sNetPresSockets[x].transHandle, &sNetPresSockets[x].providerData))
                    {
                        sNetPresSockets[x].status = NET_PRES_ENC_SS_FAILED;
                        continue;                       
                    }
                    //Intentional fall through to the next state
                }
                case NET_PRES_ENC_SS_CLIENT_NEGOTIATING:
                case NET_PRES_ENC_SS_SERVER_NEGOTIATING:
                    sNetPresSockets[x].status = (*sNetPresSockets[x].provObject->fpConnect)(sNetPresSockets[x].providerData);
                    break;
                default:
                    break;
            }
        }
    }
}

NET_PRES_SKT_HANDLE_T NET_PRES_SKT_Open(NET_PRES_INDEX index, NET_PRES_SKT_T socketType, NET_PRES_SKT_ADDR_T addrType, NET_PRES_SKT_PORT_T port, NET_PRES_ADDRESS * addr, NET_PRES_SKT_ERROR_T* error)
{
    NET_PRES_TransOpen fpTransOpen = NULL;
    NET_PRES_EncProviderOpen fpProvOpen= NULL;
    NET_PRES_TransportObject * transObject;
    NET_PRES_EncProviderObject * provObject;

    // Check to see if we have a valid index
    if (index >= sNetPresData.numLayers)
    {
        *error = NET_PRES_SKT_OP_INVALID_INDEX;
        return NET_PRES_INVALID_SOCKET;
    }

    // Check to see if the operation is supported
    if ((socketType & (NET_PRES_SKT_CLIENT | NET_PRES_SKT_STREAM)) ==  (NET_PRES_SKT_CLIENT | NET_PRES_SKT_STREAM))
    {
        transObject = &(sNetPresData.transObjectSC[index]);
        provObject = &(sNetPresData.encProvObjectSC[index]);
        fpProvOpen = sNetPresData.encProvObjectSC[index].fpOpen;
        fpTransOpen = sNetPresData.transObjectSC[index].fpOpen;
    }
    else if ((socketType & (NET_PRES_SKT_SERVER | NET_PRES_SKT_STREAM)) ==  (NET_PRES_SKT_SERVER | NET_PRES_SKT_STREAM))
    {
        transObject = &(sNetPresData.transObjectSS[index]);
        provObject = &(sNetPresData.encProvObjectSS[index]);
        fpProvOpen = sNetPresData.encProvObjectSS[index].fpOpen;
        fpTransOpen = sNetPresData.transObjectSS[index].fpOpen;
    }
    else if ((socketType & (NET_PRES_SKT_CLIENT | NET_PRES_SKT_DATAGRAM)) ==  (NET_PRES_SKT_CLIENT | NET_PRES_SKT_DATAGRAM))
    {
        transObject = &(sNetPresData.transObjectDC[index]);
        provObject = &(sNetPresData.encProvObjectDC[index]);
        fpProvOpen = sNetPresData.encProvObjectDC[index].fpOpen;
        fpTransOpen = sNetPresData.transObjectDC[index].fpOpen;
    }
    else if ((socketType & (NET_PRES_SKT_SERVER | NET_PRES_SKT_DATAGRAM)) ==  (NET_PRES_SKT_SERVER | NET_PRES_SKT_DATAGRAM))
    {
        transObject = &(sNetPresData.transObjectDS[index]);
        provObject = &(sNetPresData.encProvObjectDS[index]);
        fpProvOpen = sNetPresData.encProvObjectDS[index].fpOpen;
        fpTransOpen = sNetPresData.transObjectDS[index].fpOpen;
    }
    if (fpTransOpen == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return NET_PRES_INVALID_SOCKET;        
    }
    bool encrypted = (socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED;
    
    if (encrypted)
    {
        if (fpProvOpen == NULL)
        {
            if (error != NULL)
            {
                *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
            }
            return NET_PRES_INVALID_SOCKET;                    
        }
    }
    
    // The inputs have been validated
    if (OSAL_MUTEX_Lock(&sNetPresData.presMutex, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_UNKNOWN_ERROR;
        }
        return NET_PRES_INVALID_SOCKET;
    }
    
    // Search for a free socket
    uint8_t sockIndex;
    for (sockIndex = 0 ; sockIndex < NET_PRES_NUM_SOCKETS; sockIndex++)
    {
        if (sNetPresSockets[sockIndex].inUse)
        {
            continue;
        }
        sNetPresSockets[sockIndex].inUse = true;
        // the socket has been soft locked so no longer need the mutex.
        if (OSAL_MUTEX_Unlock(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
        {
            sNetPresSockets[sockIndex].inUse = false;
            if (error != NULL)
            {
                *error = NET_PRES_SKT_UNKNOWN_ERROR;
            }
            return NET_PRES_INVALID_SOCKET;        
        }
        sNetPresSockets[sockIndex].transHandle = (*fpTransOpen)(addrType, port, addr);
        if (sNetPresSockets[sockIndex].transHandle == NET_PRES_INVALID_SOCKET)
        {
            sNetPresSockets[sockIndex].inUse = false;
            if (error != NULL)
            {
                *error = NET_PRES_SKT_UNKNOWN_ERROR;
            }
            return NET_PRES_INVALID_SOCKET;            
        }
        sNetPresSockets[sockIndex].transObject = transObject;
        sNetPresSockets[sockIndex].provObject = provObject;
        sNetPresSockets[sockIndex].socketType = socketType;
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OK;
        }
        if (encrypted)
        {
            sNetPresSockets[sockIndex].status = NET_PRES_ENC_SS_WAITING_TO_START_NEGOTIATION;
        }
        return sockIndex+1; // avoid returning 0 on success.        
    }
    if (OSAL_MUTEX_Unlock(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_UNKNOWN_ERROR;
        }
        return NET_PRES_INVALID_SOCKET;        
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OP_OUT_OF_HANDLES;
    }
    return NET_PRES_INVALID_SOCKET;   
}

bool NET_PRES_SKT_Bind(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ADDR_T addrType, NET_PRES_SKT_PORT_T port, NET_PRES_ADDRESS * addr, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBind fp = sNetPresSockets[handle].transObject->fpLocalBind;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle, addrType, port, addr);
}


bool NET_PRES_SKT_RemoteBind(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ADDR_T addrType, NET_PRES_SKT_PORT_T port, NET_PRES_ADDRESS * addr, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBind fp =  sNetPresSockets[handle].transObject->fpRemoteBind;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle, addrType, port, addr);
}

bool NET_PRES_SKT_OptionsSet(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_OPTION_TYPE option, void* optParam, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransOption fp = sNetPresSockets[handle].transObject->fpOptionSet;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle, option, optParam);
    
}

bool NET_PRES_SKT_OptionsGet(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_OPTION_TYPE option, void* optParam, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransOption fp = sNetPresSockets[handle].transObject->fpOptionGet;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle, option, optParam);    
}

bool NET_PRES_SKT_IsConnected(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBool fp = sNetPresSockets[handle].transObject->fpIsConnected;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
} 

bool NET_PRES_SKT_WasReset(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBool fp = sNetPresSockets[handle].transObject->fpWasReset;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
}  

bool NET_PRES_SKT_Disconnect(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBool fp = sNetPresSockets[handle].transObject->fpDisconnect;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
}  

bool NET_PRES_SKT_Connect(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBool fp = sNetPresSockets[handle].transObject->fpConnect;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
}  

void NET_PRES_SKT_Close(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return;
    }
    
    if ((sNetPresSockets[handle].socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED)
    {
        NET_PRES_EncProviderConnectionClose fp = sNetPresSockets[handle].provObject->fpClose;
        if (fp != NULL)
        {
            (*fp)(sNetPresSockets[handle].providerData);
        }
    }
    NET_PRES_TransClose fpc = sNetPresSockets[handle].transObject->fpClose;
    if (fpc == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return;
    }
    (*fpc)(sNetPresSockets[handle].transHandle);
    if (OSAL_MUTEX_Lock(&sNetPresData.presMutex, OSAL_WAIT_FOREVER) != OSAL_RESULT_TRUE)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_UNKNOWN_ERROR;
        }
        return;
    }
    memset(&sNetPresSockets[handle], 0, sizeof(NET_PRES_SocketData));
    if (OSAL_MUTEX_Unlock(&sNetPresData.presMutex) != OSAL_RESULT_TRUE)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_UNKNOWN_ERROR;
        }
        return;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    } 
}

bool NET_PRES_SKT_SocketInfoGet(NET_PRES_SKT_HANDLE_T handle, void * info, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransSocketInfoGet fp = sNetPresSockets[handle].transObject->fpSocketInfoGet;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle, info);    
   
}

uint16_t NET_PRES_SKT_WriteIsReady(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransReady fp = sNetPresSockets[handle].transObject->fpReadyToWrite;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
    
}

uint16_t NET_PRES_SKT_ReadIsReady(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransReady fp = sNetPresSockets[handle].transObject->fpReadyToRead;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
    
}

uint16_t NET_PRES_SKT_Write(NET_PRES_SKT_HANDLE_T handle, const void * buffer, uint16_t size, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    if ((sNetPresSockets[handle].socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED)
    {
        NET_PRES_EncProviderWrite fp = sNetPresSockets[handle].provObject->fpWrite;
        if (fp == NULL)
        {
            if (error != NULL)
            {
                *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
            }
            return false;
        }
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OK;
        }
        return (*fp)(sNetPresSockets[handle].providerData, buffer, size);    
    }
    NET_PRES_TransWrite fpc = sNetPresSockets[handle].transObject->fpWrite;
    if (fpc == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fpc)(sNetPresSockets[handle].transHandle, buffer, size);  
}

bool NET_PRES_SKT_Flush(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransBool fp = sNetPresSockets[handle].transObject->fpFlush;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);        
}

uint16_t NET_PRES_SKT_Read(NET_PRES_SKT_HANDLE_T handle, void * buffer, uint16_t size, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    if ((sNetPresSockets[handle].socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED)
    {
        NET_PRES_EncProviderRead fp = sNetPresSockets[handle].provObject->fpRead;
        if (fp == NULL)
        {
            if (error != NULL)
            {
                *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
            }
            return false;
        }
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OK;
        }
        return (*fp)(sNetPresSockets[handle].providerData, buffer, size);    
    }
    NET_PRES_TransRead fpc = sNetPresSockets[handle].transObject->fpRead;
    if (fpc == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fpc)(sNetPresSockets[handle].transHandle, buffer, size);  
}

uint16_t NET_PRES_SKT_Peek(NET_PRES_SKT_HANDLE_T handle, void * buffer, uint16_t size, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    if ((sNetPresSockets[handle].socketType & NET_PRES_SKT_ENCRYPTED) == NET_PRES_SKT_ENCRYPTED)
    {
        NET_PRES_EncProviderRead fp = sNetPresSockets[handle].provObject->fpPeek;
        if (fp == NULL)
        {
            if (error != NULL)
            {
                *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
            }
            return false;
        }
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OK;
        }
        return (*fp)(sNetPresSockets[handle].providerData, buffer, size);    
    }
    NET_PRES_TransPeek fpc = sNetPresSockets[handle].transObject->fpPeek;
    if (fpc == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fpc)(sNetPresSockets[handle].transHandle, buffer, size, 0);  
}

bool NET_PRES_SKT_Discard(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransDiscard fp = sNetPresSockets[handle].transObject->fpDiscard;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle);    
    
}


NET_PRES_SIGNAL_HANDLE NET_PRES_SKT_SignalHandlerRegister(NET_PRES_SKT_HANDLE_T handle, uint16_t sigMask, NET_PRES_SIGNAL_FUNCTION handler, const void* hParam, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransHandlerRegister fp = sNetPresSockets[handle].transObject->fpHandlerRegister;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(sNetPresSockets[handle].transHandle, sigMask, handler, hParam);    

}
bool NET_PRES_SKT_SignalHandlerDeregister(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SIGNAL_HANDLE hSig, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    NET_PRES_TransSignalHandlerDeregister fp = sNetPresSockets[handle].transObject->fpHandlerDeregister;
    if (fp == NULL)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_OP_NOT_SUPPORTED;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }
    return (*fp)(handle, hSig);    

}

bool NET_PRES_SKT_IsNegotiatingEncryption(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    if (error != NULL)
    {
        *error = NET_PRES_SKT_OK;
    }        
    return ((sNetPresSockets[handle].status == NET_PRES_ENC_SS_CLIENT_NEGOTIATING) ||  
            (sNetPresSockets[handle].status == NET_PRES_ENC_SS_SERVER_NEGOTIATING) || 
            (sNetPresSockets[handle].status == NET_PRES_ENC_SS_WAITING_TO_START_NEGOTIATION));
}
bool NET_PRES_SKT_IsSecure(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    return sNetPresSockets[handle].status == NET_PRES_ENC_SS_OPEN;
}
bool NET_PRES_SKT_EncryptSocket(NET_PRES_SKT_HANDLE_T handle, NET_PRES_SKT_ERROR_T* error)
{
    if (handle <= 0 || handle > NET_PRES_NUM_SOCKETS)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    handle--;
    if (!sNetPresSockets[handle].inUse)
    {
        if (error != NULL)
        {
            *error = NET_PRES_SKT_INVALID_SOCKET;
        }
        return false;
    }
    if ((sNetPresSockets[handle].socketType & (NET_PRES_SKT_UNENCRYPTED | NET_PRES_SKT_ENCRYPTED)) != NET_PRES_SKT_UNENCRYPTED)
    {
        return false;
    }
    sNetPresSockets[handle].socketType ^= NET_PRES_SKT_UNENCRYPTED | NET_PRES_SKT_ENCRYPTED;
    sNetPresSockets[handle].status = NET_PRES_ENC_SS_WAITING_TO_START_NEGOTIATION;
    return true;
}
