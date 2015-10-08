config USB_DEVICE_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on USE_USB_STACK
	depends on DRV_USB_DEVICE_SUPPORT
    <#if INSTANCE != 0>
	default n if USB_DEVICE_INSTANCES_NUMBER_GT_${INSTANCE} = n
    </#if>
	default n if USB_DEVICE_INSTANCES_NUMBER = ${INSTANCE+1}
	default y

config USB_DEVICE_INST_IDX${INSTANCE}
    depends on USE_USB_STACK
    depends on DRV_USB_DEVICE_SUPPORT
    <#if INSTANCE != 0> && USB_DEVICE_INSTANCES_NUMBER_GT_${INSTANCE}
    </#if>
    bool "USB Device Instance ${INSTANCE}"
    default y
    ---help---
    ---endhelp---

ifblock USB_DEVICE_INST_IDX${INSTANCE}

 config USB_DEVICE_SPEED_HS_IDX${INSTANCE}
        string "Device Speed"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && PIC32MZ
		range USB_DEVICE_SPEED_HS
        default "USB_SPEED_HIGH"
        ---help---
        IDH_HTML_USB_DEVICE_INIT

        ---endhelp---
		
	config USB_DEVICE_SPEED_FS_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && PIC32MX
        range USB_DEVICE_SPEED_FS
        default "USB_SPEED_FULL"
        ---help---
        IDH_HTML_USB_DEVICE_INIT
        ---endhelp---
        
	    config USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE}
        int "Number of Functions Registered to this Device Instance"
        depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT
        range 0 10
        default 1
        ---help---
        IDH_HTML_USB_DEVLAYER_Function_Driver_Registration_Table
        ---endhelp---
		
		
config USB_DEVICE_FUNCTION_1_GT_IDX${INSTANCE}
        bool
		depends on DRV_USB_DEVICE_SUPPORT
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 0
        default y


		
			
config USB_DEVICE_FUNCTION_1_IDX${INSTANCE}
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_FUNCTION_1_GT_IDX${INSTANCE}
        bool "Function 1"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		

ifblock USB_DEVICE_FUNCTION_1_IDX${INSTANCE}
source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_1.ftl" 1 instances
   endif		

 config USB_DEVICE_FUNCTION_2_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 1
        default n if USB_DEVICE_FUNCTION_1_GT_IDX${INSTANCE} = n
        default y
		
   config USB_DEVICE_FUNCTION_2_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_2_GT_IDX${INSTANCE}
        bool "Function 2"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_2_IDX${INSTANCE}
    source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_2.ftl"  1 instances
   endif	

 
   
    config USB_DEVICE_FUNCTION_3_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 2
        default n if USB_DEVICE_FUNCTION_2_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_3_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_3_GT_IDX${INSTANCE}
        bool "Function 3"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_3_IDX${INSTANCE}
      source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_3.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_4_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 3
        default n if USB_DEVICE_FUNCTION_3_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_4_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_4_GT_IDX${INSTANCE}
        bool "Function 4"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_4_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_4.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_5_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 4
        default n if USB_DEVICE_FUNCTION_4_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_5_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_5_GT_IDX${INSTANCE}
        bool "Function 5"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_5_IDX${INSTANCE}
        source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_5.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_6_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 5
        default n if USB_DEVICE_FUNCTION_5_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_6_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_6_GT_IDX${INSTANCE}
        bool "Function 6"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_6_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_6.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_7_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 6
        default n if USB_DEVICE_FUNCTION_6_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_7_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_7_GT_IDX${INSTANCE}
        bool "Function 7"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_7_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_7.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_8_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 7
        default n if USB_DEVICE_FUNCTION_7_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_8_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_8_GT_IDX${INSTANCE}
        bool "Function 8"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_8_IDX${INSTANCE}
        source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_8.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_9_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 8
        default n if USB_DEVICE_FUNCTION_8_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_9_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_9_GT_IDX${INSTANCE}
        bool "Function 9"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_9_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_9.ftl"  1 instances
   endif
   
    config USB_DEVICE_FUNCTION_10_GT_IDX${INSTANCE}
        bool
		depends on USE_USB_STACK
		depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE}
        default n if USB_DEVICE_NUMBER_REGISTRED_FUNCTION_DRIVER_IDX${INSTANCE} = 9
        default n if USB_DEVICE_FUNCTION_9_GT_IDX${INSTANCE} = n
        default y
		
	config USB_DEVICE_FUNCTION_10_IDX${INSTANCE}
		depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT && USB_DEVICE_INST_IDX${INSTANCE} && USB_DEVICE_FUNCTION_10_GT_IDX${INSTANCE}
        bool "Function 10"
        default y
		---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	ifblock USB_DEVICE_FUNCTION_10_IDX${INSTANCE}
       source "$HARMONY_VERSION_PATH/framework/usb/config/usb_device_function_10.ftl"  1 instances
   endif
    
    config USB_DEVICE_ENDPOINT_QUEUE_SIZE_READ_IDX${INSTANCE}
    int "Endpoint Read Queue Size"
    depends on USE_USB_STACK
    depends on USB_DEVICE_USE_ENDPOINT_FUNCTIONS
    default 1
    ---help---
    IDH_HTML_USB_DEVICE_INIT 
    ---endhelp---

config USB_DEVICE_ENDPOINT_QUEUE_SIZE_WRITE_IDX${INSTANCE}
    int "Endpoint Write Queue Size"
    depends on USE_USB_STACK
    depends on USB_DEVICE_USE_ENDPOINT_FUNCTIONS
    default 1
    ---help---
    IDH_HTML_USB_DEVICE_INIT 
    ---endhelp---
    
        config USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE}
        string "Product ID Selection"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT 
        range USB_DEVICE_PRODCUCT_ID_ENUM
        default "User Defined Product ID"
        ---help---
        ---endhelp---
        
        config USB_DEVICE_VENDOR_ID_IDX${INSTANCE}
        string "Enter Vendor ID"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT                
        default "0x0000" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
		default "0x04D8"
        ---help---
        Enter Vendor ID in Hex format with '0x' prefix. eg:-0x4545
        ---endhelp---

        config USB_DEVICE_PRODUCT_ID_IDX${INSTANCE}
        string "Enter Product ID"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT                
        default "0x0000" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
        default "0x0064" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_headset_demo"
        default "0xABCD" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "mac_audio_hi_res_demo"
        default "0x0208" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_dual_demo"
        default "0x000A" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_single_demo"
        default "0x0057" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_msd_basic_demo"
        default "0x000A" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_demo"
        default "0x0057" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_msd_demo"
        default "0x003F" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_basic_demo"
        default "0x005E" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_joystick_demo"
        default "0x0055" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_keyboard_demo"
        default "0x0000" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_mouse_demo"
        default "0x0054" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo"
        default "0x0009" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo"
        default "0x0009" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_sdcard_demo"
        default "0x0053" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "vendor_demo"
        ---help---
        Enter Product ID in Hex format with '0x' prefix. eg:-0x5454
        ---endhelp---

        config USB_DEVICE_MANUFACTURER_STRING_IDX${INSTANCE}
        string "Manufacturer String" 
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT
        default "" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
		default "Microchip Technology Inc."
        ---help---
        ---endhelp---
        
        config USB_DEVICE_PRODUCT_STRING_DESCRIPTOR_IDX${INSTANCE}
        string "Product String" 
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT
        default "" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "User Defined Product ID"
        default "Harmony USB Speaker Example" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "usb_headset_demo"
        default "Harmony USB Speaker 2.0" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "mac_audio_hi_res_demo"
        default "CDC Dual COM Port Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_dual_demo"
        default "Simple CDC Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_com_port_single_demo"
        default "CDC + MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_msd_basic_demo"
        default "Simple CDC Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_demo"
        default "CDC + MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "cdc_serial_emulator_msd_demo"
        default "Simple HID Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_basic_demo"
        default "HID Joystick Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_joystick_demo"
        default "HID Keyboard Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_keyboard_demo"
        default "HID Mouse Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_mouse_demo"
        default "HID + MSD Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo"
        default "Simple MSD Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo"
        default "Simple MSD Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_sdcard_demo"
        default "Simple WinUSB Device Demo" if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "vendor_demo"
        ---help---
		Enter product string
        ---endhelp---
        
        config USB_DEVICE_MSD_DISK_IMAGE_ADD_SUPPORT
        bool
        depends on USE_USB_STACK
        depends on DRV_USB_DEVICE_SUPPORT
        depends on USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo" || USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo"
        select USB_DEVICE_MSD_DISK_IMAGE_FILE_NEEDED
        default y if USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "hid_msd_demo" || USB_DEVICE_PRODUCT_ID_SELECT_IDX${INSTANCE} =  "msd_basic_demo"
        default n
    
     config USB_DEVICE_SLEEP_IN_IDLE_IDX${INSTANCE}
        bool
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT
        default n
        ---help---
        Select this to stop the USB peripheral when the CPU enters IDLE mode.
        ---endhelp---

    config USB_DEVICE_SUSPEND_IN_SLEEP_IDX${INSTANCE}
        bool "Suspend in Sleep"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT
        default n
        ---help---
        Select this to automatically suspend the USB peripheral when the PIC32
        device enter Sleep mode.
        ---endhelp---
        
	config DRV_USB_PERIPHERAL_ID_IDX${INSTANCE}
		string
		depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
		default "0" if PIC32MZ
		default "USB_ID_1" if PIC32MX
	
	
    config DRV_USB_INTERRUPT_SOURCE_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
        range INT_SOURCE
        default "INT_SOURCE_USB_1"
        ---help---
        Interrupt source for this USB Peripheral
        ---endhelp---

    config DRV_USB_INTERRUPT_VECTOR_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
        range INT_VECTOR
        default "INT_VECTOR_USB1"
        ---help---
        Interrupt vector for this USB Peripheral
        ---endhelp---
        
    config DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE}
        string "USB Interrupt Priority"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
        range INT_PRIORITY_LEVEL
        default "INT_PRIORITY_LEVEL4"
        ---help---
        IDH_HTML_INT_PRIORITY_LEVEL
        ---endhelp---

    config DRV_USB_INTERRUPT_SUB_PRIORITY_IDX${INSTANCE}
        string "USB Interrupt Sub-priority"
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
        range INT_SUBPRIORITY_LEVEL
        default "INT_SUBPRIORITY_LEVEL0"
        ---help---
        IDH_HTML_INT_SUBPRIORITY_LEVEL
        ---endhelp---

    config DRV_USB_INT_PRIO_NUM_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
        default "0" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
        default "1" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
        default "2" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
        default "3" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
        default "4" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
        default "5" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
        default "6" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
        default "7" if DRV_USB_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"

    config DRV_USB_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE
    default "_USB_1_VECTOR" if DRV_USB_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_USB1" && PIC32MX
	default "_USB_VECTOR"  if DRV_USB_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_USB1" && PIC32MZ  

    config DRV_USBDMA_INTERRUPT_SOURCE_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE && PIC32MZ
        range INT_SOURCE
        default "INT_SOURCE_USB_1_DMA"
        ---help---
        Interrupt source for this USB DMA Peripheral
        ---endhelp---

    config DRV_USBDMA_INTERRUPT_VECTOR_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE && PIC32MZ
        range INT_VECTOR
        default "INT_VECTOR_USB1_DMA"
        ---help---
        Interrupt vector for this USB DMA Peripheral
        ---endhelp---


    config DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE}
        string "USB DMA Interrupt Priority" if PIC32MZ
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE && PIC32MZ
        range INT_PRIORITY_LEVEL
        default "INT_PRIORITY_LEVEL4"
        ---help---
        IDH_HTML_INT_PRIORITY_LEVEL
        ---endhelp---

    config DRV_USBDMA_INTERRUPT_SUB_PRIORITY_IDX${INSTANCE}
        string "USB DMA Interrupt Sub-priority" if PIC32MZ
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE && PIC32MZ
        range INT_SUBPRIORITY_LEVEL
        default "INT_SUBPRIORITY_LEVEL0"
        ---help---
        IDH_HTML_INT_SUBPRIORITY_LEVEL
        ---endhelp---

    config DRV_USBDMA_INT_PRIO_NUM_IDX${INSTANCE}
        string
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE && PIC32MZ
        default "0" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
        default "1" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
        default "2" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
        default "3" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
        default "4" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
        default "5" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
        default "6" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
        default "7" if DRV_USBDMA_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"

    config DRV_USBDMA_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT && DRV_USB_INTERRUPT_MODE && PIC32MZ
    default "_USB_DMA_VECTOR" if DRV_USBDMA_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_USB1_DMA"

    config USB_DEVICE_POWER_STATE_IDX${INSTANCE}
        depends on USE_USB_STACK && DRV_USB_DEVICE_SUPPORT
        string "Power State"
        range SYS_MODULE_POWER_STATE
        default "SYS_MODULE_POWER_RUN_FULL"
        ---help---
        IDH_HTML_SYS_MODULE_INIT
        ---endhelp---

   
 
endif



