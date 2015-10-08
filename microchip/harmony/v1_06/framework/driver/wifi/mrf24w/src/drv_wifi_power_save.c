/*******************************************************************************
  MRF24W Driver Power Save functions

  File Name:
    drv_wifi_power_save.c

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    -Provides access to MRF24W WiFi controller
    -Reference: MRF24W Data sheet, IEEE 802.11 Standard
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

/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include "tcpip/src/tcpip_private.h"

#if defined(TCPIP_IF_MRF24W)
/*
*********************************************************************************************************
*                                           DEFINES
*********************************************************************************************************
*/
#define REG_ENABLE_LOW_POWER_MASK   ((uint16_t)(0x01))
#define REG_DISABLE_LOW_POWER_MASK  ((uint16_t)(0x00))
/*
*********************************************************************************************************
*                                           LOCAL DATA TYPES
*********************************************************************************************************
*/

/* Enumeration of valid values for WFSetPowerSaveMode() */
typedef enum
{
    PS_POLL_ENABLED = 0,    /* power save mode enabled  */
    PS_POLL_DISABLED        /* power save mode disabled */
} t_wfPsPwrMode;

typedef struct pwrModeRequestStruct
{
    uint8_t mode;
    uint8_t wake;
    uint8_t rcvDtims;
    uint8_t reserved;            /* pad byte */
} t_wfPwrModeReq;

/*
*********************************************************************************************************
*                                           LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static uint8_t g_powerSaveState = DRV_WIFI_PS_OFF;
static bool    g_psPollActive   = false;
static bool    g_sleepNeeded    = false;
static bool    g_AppPowerSaveModeEnabled = false;
static bool    g_hibernateMode = false;
DRV_WIFI_PS_POLL_CONTEXT g_savedPsPollContext;

/*
*********************************************************************************************************
*                                           LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static void SendPowerModeMsg(t_wfPwrModeReq *p_powerMode);
static void SetPowerSaveState(uint8_t powerSaveState);
void SetAppPowerSaveMode(bool state);
bool GetAppPowerSaveMode(void);

/*******************************************************************************
  Function:
    void WFConfigureLowPowerMode(uint8_t action)

  Summary:
    Driver function to configure PS Poll mode.

  Description:
    This function is only used by the driver, not the application.  This
    function, other than at initialization, is only used when the application
    has enabled PS-Poll mode.  This function is used to temporarily deactivate
    PS-Poll mode when there is mgmt or data message tx/rx and then, when message
    activity has ceased, to again activate PS-Poll mode.

  Precondition:
    MACInit must be called first.

  Parameters:
    action - Can be either:
             * WF_LOW_POWER_MODE_ON
             * WF_LOW_POWER_MODE_OFF

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WFConfigureLowPowerMode(uint8_t action)
{
    uint16_t lowPowerStatusRegValue;

    /*-----------------------------------------*/
    /* if activating PS-Poll mode on MRF24W */
    /*-----------------------------------------*/
    if (action == WF_LOW_POWER_MODE_ON)
    {
        //SYS_CONSOLE_MESSAGE("EPS\r\n");
        Write16BitWFRegister(WF_PSPOLL_H_REG, REG_ENABLE_LOW_POWER_MASK);
        g_psPollActive = true;
    }
    /*---------------------------------------------------------------------------------------------*/
    /* else deactivating PS-Poll mode on MRF24W (taking it out of low-power mode and waking it up) */
    /*---------------------------------------------------------------------------------------------*/
    else /* action == WF_LOW_POWER_MODE_OFF */
    {
        //SYS_CONSOLE_MESSAGE("DPS\r\n");
        Write16BitWFRegister(WF_PSPOLL_H_REG, REG_DISABLE_LOW_POWER_MASK);
        g_psPollActive = false;

        /* poll the response bit that indicates when the MRF24W has come out of low power mode */
        do
        {
            /* set the index register to the register we wish to read */
            Write16BitWFRegister(WF_INDEX_ADDR_REG, WF_SCRATCHPAD_1_REG);
            lowPowerStatusRegValue = Read16BitWFRegister(WF_INDEX_DATA_REG);

        } while (lowPowerStatusRegValue & REG_ENABLE_LOW_POWER_MASK);
    }
}

/*******************************************************************************
  Function:
    void DRV_WIFI_PsPollEnable(DRV_WIFI_PS_POLL_CONTEXT *p_context);

  Summary:
    Enables PS Poll mode.

  Description:
    Enables PS Poll mode.  PS-Poll (Power-Save Poll) is a mode allowing for
    longer battery life.  The MRF24W coordinates with the Access Point to go
    to sleep and wake up at periodic intervals to check for data messages, which
    the Access Point will buffer.  The listenInterval in the Connection
    Algorithm defines the sleep interval.  By default, PS-Poll mode is disabled.

    When PS Poll is enabled, the WF Host Driver will automatically force the
    MRF24W to wake up each time the Host sends Tx data or a control message
    to the MRF24W.  When the Host message transaction is complete the
    MRF24W driver will automatically re-enable PS Poll mode.

    When the application is likely to experience a high volume of data traffic
    then PS-Poll mode should be disabled for two reasons:
    1. No power savings will be realized in the presence of heavy data traffic.
    2. Performance will be impacted adversely as the WiFi Host Driver
        continually activates and deactivates PS-Poll mode via SPI messages.

  Parameters:
     p_context -  Pointer to ps poll context.  See DRV_WIFI_PS_POLL_CONTEXT
                  structure.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void DRV_WIFI_PsPollEnable(DRV_WIFI_PS_POLL_CONTEXT *p_context)
{
    t_wfPwrModeReq   pwrModeReq;

    // if not currently connected
    if (!WFisConnected())
    {
        // save caller parameters for later, when we can enable this mode
        g_savedPsPollContext.listenInterval = p_context->listenInterval;
        g_savedPsPollContext.dtimInterval   = p_context->dtimInterval;
        g_savedPsPollContext.useDtim        = p_context->useDtim;
        SetAppPowerSaveMode(true);
        return;
    }

    SetListenInterval(p_context->listenInterval);
    SetDtimInterval(p_context->dtimInterval);

    // fill in request structure and send message to MRF24WG
    pwrModeReq.mode     = PS_POLL_ENABLED;
    pwrModeReq.wake     = 0;
    pwrModeReq.rcvDtims = p_context->useDtim;
    SendPowerModeMsg(&pwrModeReq);

    if (p_context->useDtim)
    {
        SetPowerSaveState(DRV_WIFI_PS_PS_POLL_DTIM_ENABLED);
    }
    else
    {
        SetPowerSaveState(DRV_WIFI_PS_PS_POLL_DTIM_DISABLED);
    }

    WFConfigureLowPowerMode(WF_LOW_POWER_MODE_ON);
    SetAppPowerSaveMode(true);
}

/*******************************************************************************
  Function:
    void DRV_WIFI_PsPollDisable(void)

  Summary:
    Disables PS-Poll mode.

  Description:
    Disables PS Poll mode.  The MRF24W will stay active and not go sleep.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void DRV_WIFI_PsPollDisable(void)
{
    t_wfPwrModeReq   pwrModeReq;

    pwrModeReq.mode     = PS_POLL_DISABLED;
    pwrModeReq.wake     = 1;
    pwrModeReq.rcvDtims = 1;
    SendPowerModeMsg(&pwrModeReq);

    SetPowerSaveState(DRV_WIFI_PS_OFF);
    WFConfigureLowPowerMode(WF_LOW_POWER_MODE_OFF);
    SetAppPowerSaveMode(false);
}

bool isSleepNeeded(void)
{
    return g_sleepNeeded;
}

void ClearSleepNeeded(void)
{
    g_sleepNeeded = false;
}

void SetSleepNeeded(void)
{
    g_sleepNeeded = true;
}

void SetAppPowerSaveMode(bool state)  // true or false
{
    g_AppPowerSaveModeEnabled = state;
}

bool GetAppPowerSaveMode(void)
{
    return g_AppPowerSaveModeEnabled;
}

/*******************************************************************************
  Function:
    void DRV_WIFI_PowerSaveStateGet(uint8_t *p_powerSaveState)

  Summary:
    Gets the current power-save state.

  Description:
    This function gets the current MRF24WG power save state.

  Parameters:
    p_powerSaveState - Pointer to where power state is written
    <table>
    Value                             Definition
    -----                             ----------
    DRV_WIFI_PS_HIBERNATE             MRF24W in hibernate state
    DRV_WIFI_PS_PS_POLL_DTIM_ENABLED  MRF24W in PS-Poll mode with DTIM enabled
    DRV_WIFI_PS_PS_POLL_DTIM_DISABLED MRF24W in PS-Poll mode with DTIM disabled
    DRV_WIFI_PS_POLL_OFF              MRF24W is not in any power-save state
    </table>

  Returns:
    None.
 *******************************************************************************/
void DRV_WIFI_PowerSaveStateGet(uint8_t *p_powerSaveState)
{
    *p_powerSaveState = g_powerSaveState;
}

/*******************************************************************************
  Function:
    bool WFisPsPollEnabled(void)

  Summary:
    Determines if application has enable PS-Poll mode.

  Description:

  Precondition:
    MACInit must be called first.

  Parameters:
    None.

  Returns:
    true if application has enabled PS-Poll mode, else returns false

  Remarks:
    None.
 *******************************************************************************/
bool WFisPsPollEnabled(void)
{
    if ((g_powerSaveState == DRV_WIFI_PS_PS_POLL_DTIM_ENABLED) || (g_powerSaveState == DRV_WIFI_PS_PS_POLL_DTIM_DISABLED))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/*******************************************************************************
  Function:
    bool WFIsPsPollActive(void)

  Summary: Determine if PS Poll is currently active.

  Description:
    This function is only called when PS-Poll mode has been enabled by the
    application.  When transmitting or receiving data or mgmt messages the
    driver will temporarily disable PS-Poll.  This function is used by the
    driver to determine if PS-Poll is active or has been temporarily disabled.

  Precondition:
    MACInit must be called first.

  Parameters:
    None.

  Returns:
    true if driver has enabled PS-Poll, else false

  Remarks:
    None.
 *******************************************************************************/
bool WFIsPsPollActive(void)
{
    return g_psPollActive;
}

/*******************************************************************************
  Function:
    void EnsureWFisAwake()

  Summary:
    If PS-Poll is active or the MRF24W is asleep, ensure that it is woken up.

  Description:
    Called by the WiFi driver when it needs to transmit or receive a data or
    mgmt message. If the application has enabled PS-Poll mode and the WiFi
    driver has activated PS-Poll mode then this function will deactivate PS-Poll
    mode and wake up the MRF24W.

  Precondition:
    MACInit must be called first.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void EnsureWFisAwake()
{
    /* if the application desires the MRF24W to be in PS-Poll mode (PS-Poll with DTIM enabled or disabled */
    if ((g_powerSaveState == DRV_WIFI_PS_PS_POLL_DTIM_ENABLED) || (g_powerSaveState == DRV_WIFI_PS_PS_POLL_DTIM_DISABLED))
    {
        /* if the WF driver has activated PS-Poll */
        if (g_psPollActive == true)
        {
            /* wake up MRF24W */
            WFConfigureLowPowerMode(WF_LOW_POWER_MODE_OFF);
        }

        // will need to put device back into PS-Poll sleep mode after transaction
        SetSleepNeeded();
        WiFiAsyncSetEventPending(ASYNC_POWER_SAVE_PENDING); // wake up power save task
    }
}

/*******************************************************************************
  Function:
    void DRV_WIFI_HibernateEnable(void)

  Summary:
    Puts the MRF24WG into hibernate mode.

  Description:
    Enables Hibernate mode on the MRF24W, which effectively turns off the
    device for maximum power savings.

    MRF24W state is not maintained when it transitions to hibernate mode.

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None
 *******************************************************************************/
void DRV_WIFI_HibernateEnable()
{
#if defined(PLIB_PORTS_ExistsPortsDirection)
    if (PLIB_PORTS_ExistsPortsDirection(PORTS_ID_0))
    {
        /* set XCEN33 pin high, which puts MRF24W in hibernate mode */
        WF_SetCE_N(WIFI_PIN_HIGH);
    }
#endif

    g_hibernateMode = true;
}

/*******************************************************************************
  Function:
    bool DRV_WIFI_isHibernateEnable(void)

  Summary:
    Checks if MRF24W is in hibernate mode

  Description:
    Checks if MRF24W is in hibernate mode

  Parameters:
    None.

  Returns:
    true or false

 *******************************************************************************/
bool DRV_WIFI_isHibernateEnable(void)
{
    return g_hibernateMode;
}

void ClearHibernateMode(void)
{
    g_hibernateMode = false;
}

/*******************************************************************************
  Function:
    static void SendPowerModeMsg(t_wfPwrModeReq *p_powerMode)

  Summary:
    Send power mode management message to the MRF24W.

  Description:

  Precondition:
    MACInit must be called first.

  Parameters:
    p_powerMode - Pointer to t_wfPwrModeReq structure to send to MRF24W.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
static void SendPowerModeMsg(t_wfPwrModeReq *p_powerMode)
{
    uint8_t hdr[2];

    hdr[0] = WF_MGMT_REQUEST_TYPE;
    hdr[1] = WF_SET_POWER_MODE_SUBTYPE;

    SendMgmtMsg(hdr,
                sizeof(hdr),
               (uint8_t *)p_powerMode,
               sizeof(t_wfPwrModeReq));

    /* wait for mgmt response, free buffer after it comes in (no data to read) */
    WaitForMgmtResponse(WF_SET_POWER_MODE_SUBTYPE, FREE_MGMT_BUFFER);

}

/*******************************************************************************
  Function:
    static void SetPowerSaveState(uint8_t powerSaveState)

  Summary:
    Sets the desired power save state of the MRF24W.

  Description:

  Precondition:
    MACInit must be called first.

  Parameters:
    powerSaveState - Value of the power save state desired.

    <table>
    Value                            Definition
    -----                             ----------
    DRV_WIFI_PS_HIBERNATE             MRF24W in hibernate state
    DRV_WIFI_PS_PS_POLL_DTIM_ENABLED  MRF24W in PS-Poll mode with DTIM enabled
    DRV_WIFI_PS_PS_POLL_DTIM_DISABLED MRF24W in PS-Poll mode with DTIM disabled
    DRV_WIFI_PS_POLL_OFF              MRF24W is not in any power-save state
    </table>

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
static void SetPowerSaveState(uint8_t powerSaveState)
{
    g_powerSaveState = powerSaveState;
}
#endif /* TCPIP_IF_MRF24W */

//DOM-IGNORE-END
