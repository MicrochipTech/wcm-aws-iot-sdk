
/******************************************************
 * USB Driver Initialization
 ******************************************************/
<#if CONFIG_PIC32MX == true >
/****************************************************
 * Endpoint Table needed by the Device Layer.
 ****************************************************/
uint8_t __attribute__((aligned(512))) endPointTable[DRV_USBFS_ENDPOINTS_NUMBER * 32];
const DRV_USBFS_INIT drvUSBInit =
{
    /* Assign the endpoint table */
    .endpointTable= endPointTable,

<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
    /* Interrupt Source for USB module */
    .interruptSource = ${CONFIG_DRV_USB_INTERRUPT_SOURCE_IDX0},
    
</#if>
<#if CONFIG_USB_DEVICE_POWER_STATE_IDX0?has_content>
    /* System module initialization */
    .moduleInit = {${CONFIG_USB_DEVICE_POWER_STATE_IDX0}},
    
</#if>
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>   
    .operationMode = DRV_USBFS_OPMODE_DEVICE,
    
<#elseif CONFIG_DRV_USB_HOST_SUPPORT == true>
    .operationMode = DRV_USBFS_OPMODE_HOST,
    
</#if>
    .operationSpeed = USB_SPEED_FULL,
    
    /* Stop in idle */
<#if CONFIG_USB_DEVICE_SLEEP_IN_IDLE_IDX0 == true>
    .stopInIdle = true,
<#else>
    .stopInIdle = false,
</#if>

    /* Suspend in sleep */
<#if CONFIG_USB_DEVICE_SUSPEND_IN_SLEEP_IDX0 == true>
    .suspendInSleep= true,
<#else>
    .suspendInSleep = false,
</#if>

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_ID_1
};
<#elseif CONFIG_PIC32MZ == true >
const DRV_USBHS_INIT drvUSBInit =
{
<#if CONFIG_DRV_USB_INTERRUPT_MODE == true>
    /* Interrupt Source for USB module */
    .interruptSource = ${CONFIG_DRV_USB_INTERRUPT_SOURCE_IDX0},
    
    /* Interrupt Source for USB module */
    .interruptSourceUSBDma = INT_SOURCE_USB_1_DMA,
</#if>

<#if CONFIG_USB_DEVICE_POWER_STATE_IDX0?has_content>
    /* System module initialization */
    .moduleInit = {${CONFIG_USB_DEVICE_POWER_STATE_IDX0}},
</#if>
    
<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>   
    .operationMode = DRV_USBHS_OPMODE_DEVICE,
<#elseif CONFIG_DRV_USB_HOST_SUPPORT == true>
    .operationMode = DRV_USBHS_OPMODE_HOST,
</#if>

    .operationSpeed = ${CONFIG_USB_DEVICE_SPEED_HS_IDX0},
    
    /* Stop in idle */
<#if CONFIG_USB_DEVICE_SLEEP_IN_IDLE_IDX0 == true>
    .stopInIdle = true,
<#else>
    .stopInIdle = false,
</#if>

    /* Suspend in sleep */
<#if CONFIG_USB_DEVICE_SUSPEND_IN_SLEEP_IDX0 == true>
    .suspendInSleep= true,
<#else>
    .suspendInSleep = false,
</#if>

    /* Identifies peripheral (PLIB-level) ID */
    .usbID = 0,
};
</#if>

