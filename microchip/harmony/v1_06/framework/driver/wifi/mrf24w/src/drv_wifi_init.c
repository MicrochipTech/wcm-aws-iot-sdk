/*******************************************************************************
  MRF24W Driver Initialization

  File Name:
    drv_wifi_init.c

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

#define EXPECTED_MRF24W_VERSION_NUMBER (2)

#ifdef WF_CONFIG_MHC
/* Used in MHC */
#define MhcWep40KeyLength 5
#define MhcWep104KeyLength 13
#endif

/* This MAC address is the default MAC address used in TCP/IP stack.  If the */
/* user leaves this MAC address unchanged then the Wi-Fi Driver will get the */
/* unique MAC address from the MRF24W and have the stack use it.             */
#define MCHP_DEFAULT_MAC_ADDRESS_BYTE_1 (0x00)
#define MCHP_DEFAULT_MAC_ADDRESS_BYTE_2 (0x04)
#define MCHP_DEFAULT_MAC_ADDRESS_BYTE_3 (0xa3)
#define MCHP_DEFAULT_MAC_ADDRESS_BYTE_4 (0x00)
#define MCHP_DEFAULT_MAC_ADDRESS_BYTE_5 (0x00)
#define MCHP_DEFAULT_MAC_ADDRESS_BYTE_6 (0x00)

#define WF_NOTIFY_CONNECTION_ATTEMPT_SUCCESSFUL ((uint8_t)(0x01))
#define WF_NOTIFY_CONNECTION_ATTEMPT_FAILED     ((uint8_t)(0x02))
#define WF_NOTIFY_CONNECTION_TEMPORARILY_LOST   ((uint8_t)(0x04))
#define WF_NOTIFY_CONNECTION_PERMANENTLY_LOST   ((uint8_t)(0x08))
#define WF_NOTIFY_CONNECTION_REESTABLISHED      ((uint8_t)(0x10))
#define WF_NOTIFY_ALL_EVENTS                    ((uint8_t)(0x1f))

// Initialize SM states
typedef enum
{
    I_INIT_BEGIN          = 0,
    I_CHIP_RESET          = 1,
    I_RAW_INIT            = 2,
    I_SEND_INIT_MGMT_MSGS = 4,
    I_CONNECT             = 5,
    I_INIT_COMPLETE       = 6,
} t_initStates;

typedef enum
{
    CR_BEGIN                = 0,
    CR_WAIT_HW_RESET        = 1,
    CR_WAIT_FIFO_BYTE_COUNT = 2,
} t_chipResetStates;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void ValidateConfig(void);

/*
*********************************************************************************************************
*                                           LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/* This MAC address is the default MAC address used in TCP/IP stack.  If the */
/* user leaves this MAC address unchanged then the Wi-Fi Driver will get the */
/* unique MAC address from the MRF24W and have the stack use it.             */
static const uint8_t MchpDefaultMacAddress[WF_MAC_ADDRESS_LENGTH] = {0x00u, 0x04u, 0xA3u, 0x00u, 0x00u, 0x00u};

#ifdef WF_CONFIG_MHC
static uint8_t MhcWepKey[10] = {0}; /* This array is used to store WEP key when MHC is enabled */
#endif

static uint8_t g_initState;
static uint8_t g_chipResetState;
static bool g_chipResetTimeout;

uint16_t g_mgmt_base; // mgmt msg base index in scratch memory

uint8_t g_prescan_waiting = true; // WF_PRESCAN - This is used only to allow prescan once.

#if defined(WIFI_USE_RTOS)
static int32_t s_InitTaskState = TCPIP_MAC_RES_PENDING;
#endif

/*
*********************************************************************************************************
*                                           LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static int ChipResetStateMachine(void);
static void WF_LibInitialize(void);
static void ChipResetTimeoutCallback(uintptr_t context, uint32_t currTick);
static void InitWiFiInterrupts(void);
static void HostInterrupt2RegInit(uint16_t hostIntMaskRegMask, uint8_t state);
static void HostInterruptRegInit(uint8_t hostIntrMaskRegMask, uint8_t state);

void WF_InitForSoftApReDirection_enable(void)
{
    g_initState = I_INIT_BEGIN;
}

int WF_InitForSoftApReDirection(void)
{
    int retCode = TCPIP_MAC_RES_PENDING, tmp;
    DRV_WIFI_DEVICE_INFO deviceInfo;
    TCPIP_NET_HANDLE netH = TCPIP_STACK_NetHandleGet("MRF24W");
    TCPIP_NET_IF *pNetIf = (TCPIP_NET_IF *)netH;

    switch (g_initState) {
    //----------------------------------------
    case I_INIT_BEGIN:
    //----------------------------------------
        // Release TX heap memory
        WiFi_ReleaseTxHeapInRedirection();

        // Disable DHCP client if the previous mode is Infrastructure mode
        TCPIP_DHCP_Disable(pNetIf);

#if defined(PLIB_PORTS_ExistsPortsDirection)
        if (PLIB_PORTS_ExistsPortsDirection(PORTS_ID_0))
        {
            /* Toggle the module into and then out of hibernate */
            WF_SetCE_N(WIFI_PIN_HIGH); /* disable module */
            WF_SetCE_N(WIFI_PIN_LOW); /* enable module */

            /* Toggle the module into and out of reset */
            WF_SetRST_N(WIFI_PIN_LOW); // put module into reset
            WF_SetRST_N(WIFI_PIN_HIGH); // take module out of of reset
        }
#endif

        ResetPll(); // needed until PLL fix made in A2 silicon
        WiFiAsyncClearAllEvents();
        g_chipResetState = CR_BEGIN;
        g_initState = I_CHIP_RESET;
        break;

    //----------------------------------------
    case I_CHIP_RESET:
    //----------------------------------------
        retCode = ChipResetStateMachine();
        // if chip reset complete
        if (retCode == TCPIP_MAC_RES_OK)
        {
            retCode = TCPIP_MAC_RES_PENDING; // not done initializing, so set code back to busy
            RawResetInitStateMachine();
            g_initState = I_RAW_INIT;
        }
        break;

    //----------------------------------------
    case I_RAW_INIT:
    //----------------------------------------
        retCode = RawInitStateMachine();
        // if RAW init complete
        if (retCode == RAW_INIT_COMPLETE)
        {
            g_initState = I_SEND_INIT_MGMT_MSGS;
            retCode = TCPIP_MAC_RES_PENDING; // not done initializing, so still busy with init
        }
        // else if not still busy doing raw init then raw init failed
        else if (retCode != TCPIP_MAC_RES_PENDING)
        {
            // TBD: signal WiFi fatal error
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }
        break;

    //----------------------------------------
    case I_SEND_INIT_MGMT_MSGS:
    //----------------------------------------
        g_mgmt_base = WF_ReadMgmtBase();
        DRV_WIFI_DeviceInfoGet(&deviceInfo);
        WF_CPCreate();
        WiFiAsyncClearAllEvents();

        /* send init messages to MRF24W */
        WF_LibInitialize();
        if (p_wifi_ConfigData->Flags.bWFEasyConfig)
        {
            WFEasyConfigInit();
        }

        // init data tx and rx state machines
        InitDataTxStateMachine();
        DeInitDataRxStateMachine();
        if (InitDataRxStateMachine() == TCPIP_MAC_RES_INIT_FAIL)
        {
            // before returning failure code, deallocate any packets previously allocated in this loop
            DeInitDataRxStateMachine();
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }

        // clear scan flags set in last connection
        SCAN_CLEAR_VALID(SCANCXT.scanState);
        SCAN_CLEAR_DISPLAY(SCANCXT.scanState);
        SCANCXT.displayIdx = 0;

        g_initState = I_CONNECT;
        //break; // do not break, just continue to run I_CONNECT

    //-----------------------------------------
    case I_CONNECT:
    //-----------------------------------------
        tmp = DRV_WIFI_ConnectStateMachine();
        if (tmp == TCPIP_MAC_RES_OK)
        {
            /*
             * Now we only allow redirect to Infrastructure mode, so we always disable DHCP server
             * and enable DHCP client
             */
            // The following macros are temporary workarounds before the Enable and Disable
            // functions have been taken care of. They should be removed afterwards.
#ifdef TCPIP_STACK_USE_DHCP_SERVER
            TCPIP_DHCPS_Disable(pNetIf);
#endif
#ifdef TCPIP_STACK_USE_DHCP_CLIENT
            TCPIP_DHCP_Enable(pNetIf);
#endif

            retCode = TCPIP_MAC_RES_OK;

            // init done, this state machine won't be called anymore, but
            // interrupt needs to know init is complete
            g_initState = I_INIT_COMPLETE;
        }
        break;

    } // end switch

    return retCode;
}

/******************************************************************************
 * Function:        void WF_InitStateMachine(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          success, failure, or still initializing (busy)
 *
 * Side Effects:    None
 *
 * Overview:        Called from MRF24W_MACInit().  Performs the WiFi driver
 *                  initialization, resets the MRF24WG, and configures MRF24WG
 *                  for operations.
 *
 * Note:            None
 *****************************************************************************/
int WF_InitStateMachine(void)
{
    int retCode = TCPIP_MAC_RES_PENDING, tmp;
    DRV_WIFI_DEVICE_INFO deviceInfo;
    // check for and process events. The async handler is
    // not invoked during initialization at a system level, so
    // it needs to be done here
    WiFiAsyncTask();

    switch (g_initState) {
    //----------------------------------------
    case I_INIT_BEGIN:
    //----------------------------------------
        if (!DRV_WIFI_ConfigDataLoad())
        {
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }

        /* initialize the SPI interface */
        if (!DRV_WIFI_SpiInit())
        {
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }

#if defined(PLIB_PORTS_ExistsPortsDirection)
        if (PLIB_PORTS_ExistsPortsDirection(PORTS_ID_0))
        {
            /* Toggle the module into and then out of hibernate */
            WF_SetCE_N(WIFI_PIN_HIGH); /* disable module */
            WF_SetCE_N(WIFI_PIN_LOW); /* enable module */

            /* Toggle the module into and out of reset */
            WF_SetRST_N(WIFI_PIN_LOW); // put module into reset
            WF_SetRST_N(WIFI_PIN_HIGH); // take module out of of reset
        }
#endif

        ResetPll(); // needed until PLL fix made in A2 silicon
        WiFiAsyncClearAllEvents();
        g_chipResetState = CR_BEGIN;
        g_initState = I_CHIP_RESET;
#if defined(MRF24W_USE_CN_INT)
        // if don't read pin status, board cannot boot after programming
        SYS_PORTS_PinRead(PORTS_ID_0, WF_INT_PORT_CHANNEL, WF_INT_BIT_POS);
#endif
        break;

    //----------------------------------------
    case I_CHIP_RESET:
    //----------------------------------------
        retCode = ChipResetStateMachine();
        // if chip reset complete
        if (retCode == TCPIP_MAC_RES_OK)
        {
            // TBD -- disable low power mode
            retCode = TCPIP_MAC_RES_PENDING; // not done initializing, so set code back to busy
            RawResetInitStateMachine();
            g_initState = I_RAW_INIT;
        }
        break;

    //----------------------------------------
    case I_RAW_INIT:
    //----------------------------------------
        retCode = RawInitStateMachine();
        // if RAW init complete
        if (retCode == RAW_INIT_COMPLETE)
        {
            g_initState = I_SEND_INIT_MGMT_MSGS;
            retCode = TCPIP_MAC_RES_PENDING; // not done initializing, so still busy with init
        }
        // else if not still busy doing raw init then raw init failed
        else if (retCode != TCPIP_MAC_RES_PENDING)
        {
            // TBD: signal WiFi fatal error
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }
        break;

    //----------------------------------------
    case I_SEND_INIT_MGMT_MSGS:
    //----------------------------------------
        g_mgmt_base = WF_ReadMgmtBase();
        DRV_WIFI_DeviceInfoGet(&deviceInfo);
        WF_CPCreate();
        WiFiAsyncClearAllEvents();
        DRV_WIFI_ASSERT(deviceInfo.romVersion == 0x31 || deviceInfo.romVersion == 0x32, "");

        /* send init messages to MRF24W */
        WF_LibInitialize();

        if (p_wifi_ConfigData->Flags.bWFEasyConfig)
        {
            WFEasyConfigInit();
        }

        // init data tx and rx state machines
        InitDataTxStateMachine();
        DeInitDataRxStateMachine();
        if (InitDataRxStateMachine() == TCPIP_MAC_RES_INIT_FAIL)
        {
            // before returning failure code, deallocate any packets previously allocated in this loop
            DeInitDataRxStateMachine();
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }

        #if defined(SYS_CONSOLE_ENABLE) || defined(SYS_CONSOLE_INSTANCES_NUMBER)
        ValidateConfig();
        #endif

#if defined(TCPIP_STACK_COMMANDS_WIFI_ENABLE)
        // initialize Wi-Fi console commands
        WIFICmdsInit();
#endif

        g_initState = I_CONNECT;
        break;

    //-----------------------------------------
    case I_CONNECT:
    //-----------------------------------------
        if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE)
        {
            tmp = DRV_WIFI_ConnectStateMachine();
            WiFiAsyncSetEventPending(ASYNC_DHCP_CONFIG_PENDING); // configure DHCP after init complete
        }
        else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_SOFT_AP)
        {
            g_prescan_waiting = true;
            WF_PreScanStart();
            tmp = TCPIP_MAC_RES_OK;
        }
        else if (p_wifi_ConfigData->networkType == DRV_WIFI_NETWORK_TYPE_ADHOC)
        {
            #if (DRV_WIFI_DEFAULT_ADHOC_PRESCAN == DRV_WIFI_ENABLED)
                g_prescan_waiting = true;
                WF_PreScanStart();
                tmp = TCPIP_MAC_RES_OK;
            #else
                tmp = TCPIP_MAC_RES_OK;
            #endif
        }
        else
        {
            tmp = TCPIP_MAC_RES_INIT_FAIL;
            DRV_WIFI_ASSERT(false, "Please compile with correct Network Type");
        }

        if (tmp == TCPIP_MAC_RES_OK)
        {
            // init done, this state machine won't be called anymore, but
            // interrupt needs to know the state
            retCode = TCPIP_MAC_RES_OK;
            g_initState = I_INIT_COMPLETE;
        }
        break;

    //-----------------------------------------
    case I_INIT_COMPLETE:
    //-----------------------------------------
        // should never reach this case
        //DRV_WIFI_ASSERT(false, "");
        retCode = TCPIP_MAC_RES_OK;
        break;

    } // end switch

    return retCode;
}

#if defined(WIFI_USE_RTOS)
int32_t WF_InitTaskState_Get(void)
{
    return s_InitTaskState;
}

void DRV_WIFI_InitTask(void *p_arg)
{
    while (1)
    {
        WF_Init_SemTake();
        s_InitTaskState = WF_InitStateMachine();
        
        if (s_InitTaskState == TCPIP_MAC_RES_OK)
            DRV_WIFI_TaskDestroy(NULL);
    }
}
#endif /* defined(WIFI_USE_RTOS) */

bool isInitComplete(void)
{
    return g_initState == I_INIT_COMPLETE;
}

static void ChipResetTimeoutCallback(uintptr_t context, uint32_t currTick)
{
    g_chipResetTimeout = true;
}

static int ChipResetStateMachine(void)
{
    uint16_t value;
    uint16_t timeout;
    static SYS_TMR_HANDLE timer = 0;

    int retCode = TCPIP_MAC_RES_PENDING;

    ClearHibernateMode();  // reset always clears hibernate mode

    switch (g_chipResetState) {
    //----------------------------------------
    case CR_BEGIN:
    //----------------------------------------
        /* clear the power bit to disable low power mode on the MRF24W */
        Write16BitWFRegister(WF_PSPOLL_H_REG, 0x0000);

        /* set HOST_RESET bit in register to put device in reset */
        Write16BitWFRegister(WF_HOST_RESET_REG, Read16BitWFRegister(WF_HOST_RESET_REG) | WF_HOST_RESET_MASK);

        /* clear HOST_RESET bit in register to take device out of reset */
        Write16BitWFRegister(WF_HOST_RESET_REG, Read16BitWFRegister(WF_HOST_RESET_REG) & ~WF_HOST_RESET_MASK);

        /* after reset is started poll register to determine when HW reset has completed */
        g_chipResetTimeout = false;
        timeout = SYS_TMR_TickCounterFrequencyGet() * 3;
        timer = SYS_TMR_CallbackSingle(timeout, 0, ChipResetTimeoutCallback);

        g_chipResetState = CR_WAIT_HW_RESET;
        break;

    //----------------------------------------
    case CR_WAIT_HW_RESET:
    //----------------------------------------
        // if HW reset completed
        Write16BitWFRegister(WF_INDEX_ADDR_REG, WF_HW_STATUS_REG);
        value = Read16BitWFRegister(WF_INDEX_DATA_REG);
        if ((value & WF_HW_STATUS_NOT_IN_RESET_MASK) != 0)
        {
            // if value 0xffff then most likely SPI is not connected
            if (value == 0xffff)
            {
                retCode = TCPIP_MAC_RES_INIT_FAIL;
            }
            // else successful HW reset, set timer and go to next state
            else
            {
                uint16_t timeout;

                g_chipResetTimeout = false;
                timeout = SYS_TMR_TickCounterFrequencyGet() * 3;
                SYS_TMR_CallbackStop(timer);
                timer = SYS_TMR_CallbackSingle(timeout, 0, ChipResetTimeoutCallback);

                g_chipResetState = CR_WAIT_FIFO_BYTE_COUNT;
            }
        }
        // else if timed out waiting for HW reset
        else if (g_chipResetTimeout)
        {
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }
        break;

    //----------------------------------------
    case CR_WAIT_FIFO_BYTE_COUNT:
    //----------------------------------------
        // if chip reset complete
        value = Read16BitWFRegister(WF_HOST_WFIFO_BCNT0_REG);
        if (value != 0)
        {
            SYS_TMR_CallbackStop(timer);
            InitWiFiInterrupts();
            retCode = TCPIP_MAC_RES_OK;
        }
        // else timed out
        else if (g_chipResetTimeout)
        {
            retCode = TCPIP_MAC_RES_INIT_FAIL;
        }
        break;

    } // end switch

    return retCode;
}

static void InitWiFiInterrupts(void)
{
    uint8_t  mask8;
    uint16_t mask16;

    /* disable the interrupts gated by the 16-bit host int register */
    HostInterrupt2RegInit(WF_HOST_2_INT_MASK_ALL_INT, (uint16_t)WF_INT_DISABLE);

    /* disable the interrupts gated the by main 8-bit host int register */
    HostInterruptRegInit(WF_HOST_INT_MASK_ALL_INT, WF_INT_DISABLE);

    /* initialize the External Interrupt allowing the MRF24W to interrupt */
    /* the Host from this point forward.                                  */
    DRV_WIFI_INT_Init();
    DRV_WIFI_INT_SourceEnable();

    /* enable the following MRF24W interrupts in the INT1 8-bit register */
    mask8 = (WF_HOST_INT_MASK_FIFO_0_THRESHOLD | /* Data Rx Msg interrupt                  */
             WF_HOST_INT_MASK_RAW_0_INT_0      | /* RAW0 Move Complete (Data Rx) interrupt */
             WF_HOST_INT_MASK_RAW_1_INT_0      | /* RAW1 Move Complete (Data Tx) interrupt */
             WF_HOST_INT_MASK_INT2);             /* Interrupt 2 interrupt                  */
    HostInterruptRegInit(mask8, WF_INT_ENABLE);

    /* enable the following MRF24W interrupts in the INT2 16-bit register */
    mask16 = (WF_HOST_INT_MASK_RAW_4_INT_0     | /* RAW4 Move Complete (Scratch) interrupt */
              WF_HOST_INT_MASK_MAIL_BOX_1_WRT);  /* used for mgmt msg signalling           */
    HostInterrupt2RegInit(mask16, WF_INT_ENABLE);
}

/*****************************************************************************
 * FUNCTION: HostInterrupt2RegInit
 *
 * RETURNS: N/A
 *
 * PARAMS:
 *      hostIntrMaskRegMask - The bit mask to be modified.
 *      state               - One of WF_INT_DISABLE, WF_INT_ENABLE where
 *                             Disable implies clearing the bits and enable sets the bits.
 *
 *
 *  NOTES: Initializes the 16-bit Host Interrupt register on the MRF24W with the
 *      specified mask value either setting or clearing the mask register
 *      as determined by the input parameter state.
 *****************************************************************************/
static void HostInterrupt2RegInit(uint16_t hostIntMaskRegMask, uint8_t  state)
{
    uint16_t int2MaskValue;

    /* Host Int Register is a status register where each bit indicates a specific event  */
    /* has occurred. In addition, writing a 1 to a bit location in this register clears  */
    /* the event.                                                                        */

    /* Host Int Mask Register is used to enable those events activated in Host Int Register */
    /* to cause an interrupt to the host                                                    */

    /* read current state of int2 mask reg */
    int2MaskValue = Read16BitWFRegister(WF_HOST_INTR2_MASK_REG);

    /* if caller is disabling a set of interrupts */
    if (state == WF_INT_DISABLE)
    {
        /* zero out that set of interrupts in the interrupt mask copy */
        int2MaskValue &= ~hostIntMaskRegMask;
    }
    /* else caller is enabling a set of interrupts */
    else
    {
        /* set to 1 that set of interrupts in the interrupt mask copy */
        int2MaskValue |= hostIntMaskRegMask;
    }

    /* write out new interrupt mask value */
    Write16BitWFRegister(WF_HOST_INTR2_MASK_REG, int2MaskValue);

    /* ensure that pending interrupts from those updated interrupts are cleared */
    Write16BitWFRegister(WF_HOST_INTR2_REG, hostIntMaskRegMask);
}

/*****************************************************************************
 * FUNCTION: HostInterruptRegInit
 *
 * RETURNS: N/A
 *
 * PARAMS:
 *      hostIntrMaskRegMask - The bit mask to be modified.
 *      state -  one of WF_EXINT_DISABLE, WF_EXINT_ENABLE where
 *                Disable implies clearing the bits and enable sets the bits.
 *
 *
 *  NOTES: Initializes the 8-bit Host Interrupt register on the MRF24W with the
 *      specified mask value either setting or clearing the mask register
 *      as determined by the input parameter state.  The process requires
 *      2 spi operations which are performed in a blocking fashion.  The
 *      function does not return until both spi operations have completed.
 *****************************************************************************/
static void HostInterruptRegInit(uint8_t hostIntrMaskRegMask, uint8_t state)
{
    uint8_t hostIntMaskValue;

    /* Host Int Register is a status register where each bit indicates a specific event  */
    /* has occurred. In addition, writing a 1 to a bit location in this register clears  */
    /* the event.                                                                        */

    /* Host Int Mask Register is used to enable those events activated in Host Int Register */
    /* to cause an interrupt to the host                                                    */

    /* read current state of Host Interrupt Mask register */
    hostIntMaskValue = Read8BitWFRegister(WF_HOST_MASK_REG);

    /* if caller is disabling a set of interrupts */
    if (state == WF_INT_DISABLE)
    {
        /* zero out that set of interrupts in the interrupt mask copy */
        hostIntMaskValue = (hostIntMaskValue & ~hostIntrMaskRegMask);
    }
    /* else caller is enabling a set of interrupts */
    else
    {
        /* set to 1 that set of interrupts in the interrupt mask copy */
        hostIntMaskValue = (hostIntMaskValue & ~hostIntrMaskRegMask) | hostIntrMaskRegMask;
    }

    /* write out new interrupt mask value */
    Write8BitWFRegister(WF_HOST_MASK_REG, hostIntMaskValue);

    /* ensure that pending interrupts from those updated interrupts are cleared */
    Write8BitWFRegister(WF_HOST_INTR_REG, hostIntrMaskRegMask);
}

// *****************************************************************************
/* Function:
    bool DRV_WIFI_Initialize(void* pNetIf);

  Summary:
    Initializes the MRF24WG WiFi driver.

  Description:
    This function initializes the MRF24WG driver, making it ready for clients to
    use.

  Parameters:
    pNetIf      - Pointer to network interface

  Returns:
    If successful returns true, else false.
*/
bool DRV_WIFI_Initialize(void* pNetIf)
{
    pNetIf = pNetIf; // avoid compiler warning
    g_initState = I_INIT_BEGIN;

    // Initialization takes place in WF_InitStateMachine()
    return true;
}

// *****************************************************************************
/* Function:
    bool DRV_WIFI_Deinitialize(void);

  Summary:
    Initializes the MRF24WG WiFi driver.

  Description:
    This function deinitializes the MRF24WG driver.  It also saves the WiFi
    parameters in non-volatile storage.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    If successful returns true, else false.

  Remarks:
    None
*/
bool DRV_WIFI_Deinitialize( void)
{
    StopRawMoveTimer(RAW_DATA_RX_ID);
    StopRawMoveTimer(RAW_DATA_TX_ID);
    StopRawMoveTimer(RAW_SCRATCH_ID);
    g_RxDataLock = true;
    g_TxDataLock = true;
    
    g_initState = I_INIT_BEGIN;
    DeInitDataRxStateMachine();
    return DRV_WIFI_ConfigDataSave();
}

/*****************************************************************************
 * FUNCTION: WF_LibInitialize
 *
 * RETURNS:  None
 *
 * PARAMS:   None
 *
 *  NOTES:   Performs initialization which is specific to the Microchip Demo code.
 *****************************************************************************/
static void WF_LibInitialize(void)
{
    const TCPIP_MAC_MODULE_CTRL *p_ctrl = GetStackData();
    uint8_t allZeroes[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /* if the user has left the default MAC address in TCP/IP stack unchanged then use */
    /* the unique MRF24W MAC address so prevent multiple devices from having the same  */
    /* MAC address.                                                                    */
    if ( (memcmp((void *)p_ctrl->ifPhyAddress.v, (void *)MchpDefaultMacAddress, WF_MAC_ADDRESS_LENGTH) == 0) ||
         (memcmp((void *)p_ctrl->ifPhyAddress.v, (void *)allZeroes, WF_MAC_ADDRESS_LENGTH) == 0) )
    {
        /* get the MRF24W MAC address and overwrite the MAC in pNetIf */
        DRV_WIFI_MacAddressGet((uint8_t *)(p_ctrl->ifPhyAddress.v));

        // If this is Wi-Fi G demo and the default SSID is being used, 
        // append the last 4 digits of MAC address to SSID here. 
#if defined(WIFI_TCPIP_WIFI_G_DEMO)
        if ( strcmp((const char *)p_wifi_ConfigData->netSSID, "MCHP_G_xxxx") == 0 ) {
            sprintf((char *)p_wifi_ConfigData->netSSID, "MCHP_G_%02x%02x", p_ctrl->ifPhyAddress.v[4], p_ctrl->ifPhyAddress.v[5]);
            p_wifi_ConfigData->SsidLength = strlen((char*)(p_wifi_ConfigData->netSSID));
        }
#endif
    }
    /* else presume the user has a unique MAC address of their own that they wish to use */
    else
    {
        /* set MAC address with user-supplied MAC */
        DRV_WIFI_MacAddressSet((uint8_t *)(p_ctrl->ifPhyAddress.v));

        // If this is Wi-Fi G demo and the default SSID is being used,
        // append the last 4 digits of MAC address to SSID here.
#if defined(WIFI_TCPIP_WIFI_G_DEMO)
        if ( strcmp((const char *)p_wifi_ConfigData->netSSID, "MCHP_G_xxxx") == 0 ) {
            sprintf((char *)p_wifi_ConfigData->netSSID, "MCHP_G_%02x%02x", p_ctrl->ifPhyAddress.v[4], p_ctrl->ifPhyAddress.v[5]);
            p_wifi_ConfigData->SsidLength = strlen((char*)(p_wifi_ConfigData->netSSID));
        }
#endif
    }

    InitPowerSaveTask();
}

/*******************************************************************************
  Function:
    bool DRV_WIFI_ConfigDataSave(void);

  Summary:
    Save configuration data to the board EEPROM.

  Description:
    This function saves configuration data to the board EEPROM.

  Precondition:
    TCPIP stack should be initialized.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
  *****************************************************************************/
bool DRV_WIFI_ConfigDataSave(void)
{
    WiFi_WriteConfigToMemory();
    return true;
}

/*******************************************************************************
  Function:
    bool DRV_WIFI_ConfigDataLoad(void);

  Summary:
    Loads configuration data from the board EEPROM.

  Description:
    This function loads configuration data from the board EEPROM.  If not present
    or corrupted then default values will be used.

  Precondition:
    TCPIP stack should be initialized.

  Parameters:
    None

  Returns:
    None

  Remarks:
    None.
  *****************************************************************************/
bool DRV_WIFI_ConfigDataLoad(void)
{
#ifdef WF_CONFIG_MHC
    int i = 0, j = 0;
#endif
    char tempSsidStr[DRV_WIFI_MAX_SSID_LENGTH + 1] = {0};
    bool bVerifySuccess = false;

    WiFi_ReadConfigFromMemory();

    memcpy(tempSsidStr, (const char *)p_wifi_ConfigData->netSSID, sizeof(p_wifi_ConfigData->netSSID));

    // Verify
    if (false == WF_NVM_Verify())
    {
        bVerifySuccess = false;
    }
    else if ((p_wifi_ConfigData->networkType == 0) || (p_wifi_ConfigData->networkType == 0xff) ||
        (p_wifi_ConfigData->SsidLength == 0) ||
        (strlen(tempSsidStr) != p_wifi_ConfigData->SsidLength))
    {
        bVerifySuccess = false;
    }
    else
    {
        bVerifySuccess = true;
    }

    if (bVerifySuccess == false) // Use default value
    {
        p_wifi_ConfigData->networkType = WF_DEFAULT_NETWORK_TYPE;

        // Load the default SSID Name
        // If this is Wi-Fi G demo and the default SSID is being used, 
        // the last 4 digits of MAC address will be appended in WF_LibInitialize(). 
        if (sizeof(WF_DEFAULT_SSID_NAME) - 1 > DRV_WIFI_MAX_SSID_LENGTH) {
            SYS_ERROR_PRINT(SYS_ERROR_ERROR, "WF_DEFAULT_SSID_NAME could not contain more than %u characters\r\n", DRV_WIFI_MAX_SSID_LENGTH);
            return false;
        }

        memcpy(p_wifi_ConfigData->netSSID, (const void *)WF_DEFAULT_SSID_NAME, sizeof(WF_DEFAULT_SSID_NAME) - 1);
        p_wifi_ConfigData->SsidLength = sizeof(WF_DEFAULT_SSID_NAME) - 1;
        p_wifi_ConfigData->SecurityMode = WF_DEFAULT_WIFI_SECURITY_MODE;
        p_wifi_ConfigData->Flags.bWFEasyConfig = 1;

        switch (p_wifi_ConfigData->SecurityMode) {
        case DRV_WIFI_SECURITY_OPEN:
            memset(p_wifi_ConfigData->SecurityKey, 0x00, sizeof(p_wifi_ConfigData->SecurityKey));
            p_wifi_ConfigData->SecurityKeyLength = 0;
            break;

        case DRV_WIFI_SECURITY_WEP_40:
#ifdef WF_CONFIG_MHC
            // Check the WF_DEFAULT_WEP_KEYS_40 length
            DRV_WIFI_ASSERT(sizeof(WF_DEFAULT_WEP_KEY_40) - 1 == 10, "WEP 40 (64-bit) key must contain exact 10 characters");
            // Convert character array to hex number array
            for (i = 0, j = 0; i < MhcWep40KeyLength; i++) {
                uint8_t temp = 0;
                DRV_WIFI_ASSERT(j < sizeof(WF_DEFAULT_WEP_KEY_40) - 1, "");
                if ((uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) >= (uint8_t)'0' && (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) <= (uint8_t)'9')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) - (uint8_t)'0';
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) >= (uint8_t)'A' && (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) <= (uint8_t)'F')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) - (uint8_t)'A' + 10;
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) >= (uint8_t)'a' && (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) <= (uint8_t)'f')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) - (uint8_t)'a' + 10;
                else
                    DRV_WIFI_ASSERT(false, "WEP 40 (64-bit) key contains invalid hex digit");
                MhcWepKey[i] = temp;
                j++;
                DRV_WIFI_ASSERT(j < sizeof(WF_DEFAULT_WEP_KEY_40) - 1, "");
                if ((uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) >= (uint8_t)'0' && (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) <= (uint8_t)'9')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) - (uint8_t)'0';
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) >= (uint8_t)'A' && (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) <= (uint8_t)'F')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) - (uint8_t)'A' + 10;
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) >= (uint8_t)'a' && (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) <= (uint8_t)'f')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_40 + j * sizeof(uint8_t)) - (uint8_t)'a' + 10;
                else
                    DRV_WIFI_ASSERT(false, "WEP 40 (64-bit) key contains invalid hex digit");
                MhcWepKey[i] = MhcWepKey[i] * 16 + temp;
                j++;
            }
            p_wifi_ConfigData->defaultWepKey = DRV_WIFI_DEFAULT_WEP_KEY_INDEX;
            memcpy(p_wifi_ConfigData->SecurityKey, (const void *)MhcWepKey, MhcWep40KeyLength);
            p_wifi_ConfigData->SecurityKeyLength = MhcWep40KeyLength;
            p_wifi_ConfigData->wepKeyType = DRV_WIFI_DEFAULT_WEP_KEY_TYPE;
#else /* WF_CONFIG_MHC is not defined */
            if (sizeof(WF_DEFAULT_WEP_KEYS_40) - 1 > DRV_WIFI_MAX_SECURITY_KEY_LENGTH) {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR,
                    "\r\nCurrent length of WF_DEFAULT_WEP_KEYS_40 is %u, it must not be longer than %u\r\n",
                    sizeof(WF_DEFAULT_WEP_KEYS_40) - 1, DRV_WIFI_MAX_SECURITY_KEY_LENGTH);
                return false;
            }
            p_wifi_ConfigData->defaultWepKey = DRV_WIFI_DEFAULT_WEP_KEY_INDEX;
            memcpy(p_wifi_ConfigData->SecurityKey, (const void *)WF_DEFAULT_WEP_KEYS_40, sizeof(WF_DEFAULT_WEP_KEYS_40) - 1);
            p_wifi_ConfigData->SecurityKeyLength = sizeof(WF_DEFAULT_WEP_KEYS_40) - 1;
            p_wifi_ConfigData->wepKeyType = DRV_WIFI_DEFAULT_WEP_KEY_TYPE;
#endif /* WF_CONFIG_MHC */
            break;

        case DRV_WIFI_SECURITY_WEP_104:
#ifdef WF_CONFIG_MHC
            // Check the WF_DEFAULT_WEP_KEYS_104 length
            DRV_WIFI_ASSERT(sizeof(WF_DEFAULT_WEP_KEY_104) - 1 == 26, "WEP 104 (128-bit) key must contain exact 26 characters");
            // Convert character array to hex number array
            for (i = 0, j = 0; i < MhcWep104KeyLength; i++) {
                uint8_t temp = 0;
                DRV_WIFI_ASSERT(j < sizeof(WF_DEFAULT_WEP_KEY_104) - 1, "");
                if ((uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) >= (uint8_t)'0' && (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) <= (uint8_t)'9')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) - (uint8_t)'0';
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) >= (uint8_t)'A' && (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) <= (uint8_t)'F')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) - (uint8_t)'A' + 10;
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) >= (uint8_t)'a' && (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) <= (uint8_t)'f')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) - (uint8_t)'a' + 10;
                else
                    DRV_WIFI_ASSERT(false, "WEP 104 (128-bit) key contains invalid hex digit");
                MhcWepKey[i] = temp;
                j++;
                DRV_WIFI_ASSERT(j < sizeof(WF_DEFAULT_WEP_KEY_104) - 1, "");
                if ((uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) >= (uint8_t)'0' && (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) <= (uint8_t)'9')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) - (uint8_t)'0';
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) >= (uint8_t)'A' && (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) <= (uint8_t)'F')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) - (uint8_t)'A' + 10;
                else if ((uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) >= (uint8_t)'a' && (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) <= (uint8_t)'f')
                    temp = (uint8_t)*(WF_DEFAULT_WEP_KEY_104 + j * sizeof(uint8_t)) - (uint8_t)'a' + 10;
                else
                    DRV_WIFI_ASSERT(false, "WEP 104 (128-bit) key contains invalid hex digit");
                MhcWepKey[i] = MhcWepKey[i] * 16 + temp;
                j++;
            }
            p_wifi_ConfigData->defaultWepKey = DRV_WIFI_DEFAULT_WEP_KEY_INDEX;
            memcpy(p_wifi_ConfigData->SecurityKey, (const void *)MhcWepKey, MhcWep104KeyLength);
            p_wifi_ConfigData->SecurityKeyLength = MhcWep104KeyLength;
            p_wifi_ConfigData->wepKeyType = DRV_WIFI_DEFAULT_WEP_KEY_TYPE;
#else /* WF_CONFIG_MHC is not defined */
            if (sizeof(WF_DEFAULT_WEP_KEYS_104) - 1 > DRV_WIFI_MAX_SECURITY_KEY_LENGTH) {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR,
                    "\r\nCurrent length of WF_DEFAULT_WEP_KEYS_104 is %u, it must not be longer than %u\r\n",
                    sizeof(WF_DEFAULT_WEP_KEYS_104) - 1, DRV_WIFI_MAX_SECURITY_KEY_LENGTH);
                return false;
            }
            p_wifi_ConfigData->defaultWepKey = DRV_WIFI_DEFAULT_WEP_KEY_INDEX;
            memcpy(p_wifi_ConfigData->SecurityKey, (const void *)WF_DEFAULT_WEP_KEYS_104, sizeof(WF_DEFAULT_WEP_KEYS_104) - 1);
            p_wifi_ConfigData->SecurityKeyLength = sizeof(WF_DEFAULT_WEP_KEYS_104) - 1;
            p_wifi_ConfigData->wepKeyType = DRV_WIFI_DEFAULT_WEP_KEY_TYPE;
#endif /* WF_CONFIG_MHC */
            break;

        case DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE:
            if (sizeof(WF_DEFAULT_PSK_PHRASE) - 1 < DRV_WIFI_MIN_WPA_PASS_PHRASE_LENGTH)
            {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR,
                        "\r\nCurrent length of WF_DEFAULT_PSK_PHRASE is %u, it must not be shorter than %u\r\n",
                        sizeof(WF_DEFAULT_PSK_PHRASE) - 1, DRV_WIFI_MIN_WPA_PASS_PHRASE_LENGTH);
                return false;
            }
            else if (sizeof(WF_DEFAULT_PSK_PHRASE) - 1 > DRV_WIFI_MAX_WPA_PASS_PHRASE_LENGTH)
            {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR,
                        "\r\nCurrent length of WF_DEFAULT_PSK_PHRASE is %u, it must not be longer than %u\r\n",
                        sizeof(WF_DEFAULT_PSK_PHRASE) - 1, DRV_WIFI_MAX_WPA_PASS_PHRASE_LENGTH);
                return false;
            }
            memcpy(p_wifi_ConfigData->SecurityKey, (const void *)WF_DEFAULT_PSK_PHRASE, sizeof(WF_DEFAULT_PSK_PHRASE) - 1);
            p_wifi_ConfigData->SecurityKeyLength = sizeof(WF_DEFAULT_PSK_PHRASE) - 1;
            break;

        case DRV_WIFI_SECURITY_WPS_PUSH_BUTTON:
            memset(p_wifi_ConfigData->SecurityKey, 0x00, sizeof(p_wifi_ConfigData->SecurityKey));
            p_wifi_ConfigData->SecurityKeyLength = 0;
            break;

        case DRV_WIFI_SECURITY_WPS_PIN:
            if (sizeof(WF_DEFAULT_WPS_PIN) - 1 > DRV_WIFI_MAX_SECURITY_KEY_LENGTH - 1) {
                SYS_ERROR_PRINT(SYS_ERROR_ERROR,
                    "\r\nCurrent length of WF_DEFAULT_WPS_PIN is %u, it must not be longer than %u\r\n",
                    sizeof(WF_DEFAULT_WPS_PIN) - 1, DRV_WIFI_MAX_SECURITY_KEY_LENGTH - 1);
                return false;
            }
            memcpy(p_wifi_ConfigData->SecurityKey, (const void *)WF_DEFAULT_WPS_PIN, sizeof(WF_DEFAULT_WPS_PIN) - 1);
            p_wifi_ConfigData->SecurityKeyLength = sizeof(WF_DEFAULT_WPS_PIN) - 1;
            break;

        default:
            DRV_WIFI_ASSERT(false, "Invalid security mode");
            break;
        }

#if defined(WF_ENABLE_STATIC_IP)
    p_wifi_ConfigData->Flags.bIsDHCPEnabled = false;
#else
    p_wifi_ConfigData->Flags.bIsDHCPEnabled = true;
#endif
    }

    return true;
}

#endif /* TCPIP_IF_MRF24W */

//DOM-IGNORE-END
