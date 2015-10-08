<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true >
 <#if CONFIG_PIC32MZ == true >
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBHS_Initialize(DRV_USBHS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);
<#elseif CONFIG_PIC32MX == true>
    /* Initialize USB Driver */ 
    sysObj.drvUSBObject = DRV_USBFS_Initialize(DRV_USBFS_INDEX_0, (SYS_MODULE_INIT *) &drvUSBInit);
</#if>
</#if>
