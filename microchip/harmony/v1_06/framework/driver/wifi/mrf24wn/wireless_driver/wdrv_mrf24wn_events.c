/*******************************************************************************
  TCP/IP MRF24WN MAC Events Implementation

  File Name:  
    wdrv_mrf24wn_events.c  

  Summary:


  Description:


 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
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

#include "system/tmr/sys_tmr.h"
#include "system/ports/sys_ports.h"
#include "peripheral/ports/plib_ports.h"

#include "wdrv_mrf24wn_common.h"
#include "wdrv_mrf24wn_main.h"
#include "wdrv_mrf24wn_priv.h"

#if defined(TCPIP_IF_MRF24WN)

#define isAsyncDhcpPending() ((ASYNC_DHCP_CONFIG_PENDING & s_asyncTaskFlags) == ASYNC_DHCP_CONFIG_PENDING)

volatile static uint8_t s_asyncTaskFlags;
static WDRV_MRF24WN_EVGROUP_DCPT s_mrfGroupDcpt = 
{
    TCPIP_MAC_EV_NONE, TCPIP_MAC_EV_NONE, 0
};
static WDRV_MRF24WN_USREV_DCPT s_mrfUsrEvent; // stack user events

static void AsyncEventClear(uint8_t event);

void TrafficEventInit(TCPIP_MAC_EventF eventF, const void *eventParam)
{
    s_mrfGroupDcpt.mrfNotifyFnc = eventF; // set new handler
    s_mrfGroupDcpt.mrfNotifyParam = eventParam;   
    s_mrfGroupDcpt.mrfEnabledEvents = false;
    s_mrfGroupDcpt.mrfPendingEvents = 0;

    s_mrfUsrEvent.trafficEvents = 0;
    s_mrfUsrEvent.trafficEventInfo = 0;
}

void TrafficEventDeInit(void)
{
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // stop MRF ints
    SYS_INT_SourceStatusClear(MRF_INT_SOURCE);

    s_mrfGroupDcpt.mrfNotifyFnc = 0;
    s_mrfGroupDcpt.mrfEnabledEvents = false;
    s_mrfGroupDcpt.mrfPendingEvents = 0;
}

bool TrafficEventMask(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents, bool enable)
{
    if (enable) {
        s_mrfGroupDcpt.mrfEnabledEvents = true;
        SYS_INT_SourceEnable(MRF_INT_SOURCE);    
    } else {
        SYS_INT_SourceDisable(MRF_INT_SOURCE);    
        s_mrfGroupDcpt.mrfEnabledEvents = false;
    }

    return true;
}

bool TrafficEventAcknowledge(TCPIP_MAC_HANDLE hMac, TCPIP_MAC_EVENT macEvents)
{
    if(s_mrfGroupDcpt.mrfPendingEvents) {
        s_mrfGroupDcpt.mrfPendingEvents = 0;
        return true;
    } else {
        return false;
    }
}

TCPIP_MAC_EVENT  TrafficEventGet(TCPIP_MAC_HANDLE hMac)
{
    return s_mrfGroupDcpt.mrfPendingEvents;
}

void TrafficEventRequest(uint16_t event, uint16_t eventInfo)
{
 
    s_mrfUsrEvent.trafficEvents = event;
    s_mrfGroupDcpt.mrfPendingEvents = event; // add this line so event function sees event?
    s_mrfUsrEvent.trafficEventInfo = eventInfo;
    
    // let app know of event
    if (s_mrfGroupDcpt.mrfNotifyFnc)
        (*s_mrfGroupDcpt.mrfNotifyFnc)(s_mrfGroupDcpt.mrfPendingEvents, s_mrfGroupDcpt.mrfNotifyParam);
} 

// called by TCPIP_STACK_Task() if any WiFi event is pending
void AsyncEventHandle(void)
{
    WDRV_DBG_TRACE_MESSAGE(("wdrv async task\r\n"));

    if (isAsyncDhcpPending()) {
        AsyncEventClear(ASYNC_DHCP_CONFIG_PENDING);
        DhcpConfigTask();
    }
}

bool AsyncEventPended(void)
{
    return s_asyncTaskFlags;
}

void AsyncEventClearAll(void)
{
    s_asyncTaskFlags = 0x00;
}

// sets an event bit
void AsyncEventSet(uint8_t event)
{  
    s_asyncTaskFlags |= event;
}

static void AsyncEventClear(uint8_t event)
{
    s_asyncTaskFlags &= ~event;
}

void WDRV_INTR_SourceEnable(void)
{
    bool pinStatus = SYS_PORTS_PinRead(PORTS_ID_0, WF_INT_PORT_CHANNEL, WF_INT_BIT_POS);

    // If INT line is low, we might have missed an interrupt, hence force an interrupt to
    // clear the status
    if (pinStatus == 0)
        PLIB_INT_SourceFlagSet(INT_ID_0,MRF_INT_SOURCE);

    /* enable the external interrupt */
    SYS_INT_SourceEnable(MRF_INT_SOURCE);
}

void WDRV_INTR_SourceDisable(void)
{
    SYS_INT_SourceDisable(MRF_INT_SOURCE);
}

void WDRV_INTR_Init(void)
{
    /* disable the external interrupt */
    SYS_INT_SourceDisable(MRF_INT_SOURCE);

    /* clear and enable the interrupt */
    SYS_INT_SourceStatusClear(MRF_INT_SOURCE);                       // clear status
    SYS_INT_VectorPrioritySet(MRF_INT_VECTOR, WF_INT_PRIORITY);      //set priority level
    SYS_INT_VectorSubprioritySet(MRF_INT_VECTOR,WF_INT_SUBPRIORITY); //set sub priority level
}

void WDRV_INTR_Deinit(void)
{
    SYS_INT_SourceDisable(MRF_INT_SOURCE);
}

void WDRV_MRF24WN_ISR(SYS_MODULE_OBJ index)
{
    SYS_INT_SourceDisable(MRF_INT_SOURCE); // disable further interrupts
    SYS_INT_SourceStatusClear(MRF_INT_SOURCE); // acknowledge the int Controller

    WDRV_EXT_HWInterruptHandler(NULL);
    WDRV_DBG_TRACE_MESSAGE_IN_ISR(("-> ISR\r\n"));
}
#endif // defined(TCPIP_IF_MRF24WN)

//DOM-IGNORE-END
