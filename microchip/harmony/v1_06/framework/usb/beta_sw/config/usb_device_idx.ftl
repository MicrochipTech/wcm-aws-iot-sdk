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
        string "Device Speed"
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
       config USB_DEVICE_FUNCTION_1_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_1_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_1_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_1_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_1_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_1_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_1_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_1_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_1_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_1_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
	   config USB_DEVICE_FUNCTION_1_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_1_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_1_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_1_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
	   
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
       config USB_DEVICE_FUNCTION_2_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_2_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_2_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_2_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
	   
       config USB_DEVICE_FUNCTION_2_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   

       config USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
		

       config USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_2_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_2_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_2_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_2_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_2_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_2_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_2_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_2_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_2_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_2_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_2_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_2_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_2_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_2_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_2_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_2_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_3_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_3_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_3_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_3_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_3_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_3_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_3_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_3_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_3_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_3_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_3_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_3_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_3_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_3_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_3_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_3_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_3_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_3_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_3_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_3_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_4_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_4_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_4_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_4_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_4_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_4_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_4_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_4_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_4_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_4_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_4_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_4_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_4_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_4_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_4_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_4_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_4_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_4_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_4_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_4_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_4_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_5_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_5_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_5_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_5_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_5_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_5_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_5_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_5_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_5_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_5_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_5_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_5_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_5_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_5_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_5_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_5_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_5_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_5_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_5_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_5_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_6_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_6_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_6_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_6_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_6_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_6_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_6_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_6_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_6_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_6_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_6_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_6_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_6_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_6_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_6_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_6_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_6_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_6_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_6_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_6_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_6_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_7_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_7_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_7_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_7_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_7_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_7_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_7_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_7_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_7_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_7_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_7_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_7_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_7_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_7_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_7_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_7_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_7_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_7_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_7_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_7_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_7_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_8_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_8_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_8_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_8_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_8_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_8_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_8_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_8_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_8_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_8_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_8_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_8_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_8_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_8_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_8_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_8_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_8_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_8_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_8_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_8_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_9_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_9_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_9_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_9_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_9_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_9_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_9_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_9_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_9_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_9_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_9_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_9_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_9_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_9_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_9_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_9_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_9_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_9_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_9_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_9_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_9_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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
       config USB_DEVICE_FUNCTION_10_CONFIGURATION_IDX${INSTANCE}
       int "Configuration Value"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_10_INTERFACE_NUMBER_IDX${INSTANCE}
       int "Start Interface Number"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 0
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_10_NUMBER_OF_INTERFACES_IDX${INSTANCE}
       int "Number of Interfaces"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       default 2 if USE_SYS_CONSOLE
       default 1
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_10_SPEED_HS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MZ
       range USB_DEVICE_FUNCTION_SPEED_HS
       default "USB_SPEED_HIGH|USB_SPEED_FULL" if PIC32MZ
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
	   
       config USB_DEVICE_FUNCTION_10_SPEED_FS_IDX${INSTANCE}
       string "Speed"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config  USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE}
       string "Device Class"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       range USB_FUNCTION_DRIVER
       default "CDC" if USE_SYS_CONSOLE
       default "VENDOR"
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---

       config USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_CDC_NEEDED
       default y if USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE} = "CDC"
       default n

       config USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_AUDIO_NEEDED
       default y if USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO"
       default n
	   
	   config USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
	   select USB_DEVICE_USE_AUDIO_2_0_NEEDED
       default y if USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
       default n

       config USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_MSD_NEEDED
       default y if USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE} = "MSD"
       default n

       config USB_DEVICE_FUNCTION_10_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_VENDOR_NEEDED
       default y if USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR"
       default n

       config USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX${INSTANCE}
       bool
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       select USB_DEVICE_USE_HID_NEEDED
       default y if USB_DEVICE_FUNCTION_10_DEVICE_CLASS_IDX${INSTANCE} = "HID"
       default n
		
       config USB_DEVICE_FUNCTION_10_CDC_READ_Q_SIZE_IDX${INSTANCE}
       int "CDC Read Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT 
	   depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
	   

       config USB_DEVICE_FUNCTION_10_CDC_WRITE_Q_SIZE_IDX${INSTANCE}
       int "CDC Write Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_10_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX${INSTANCE}
       int "CDC Serial Notification Queue Size"
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT
       depends on  USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_10_HID_READ_Q_SIZE_IDX${INSTANCE}
       int "HID Report Send Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_10_HID_WRITE_Q_SIZE_IDX${INSTANCE}
       int "HID Report Receive Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_10_MSD_LUN_IDX${INSTANCE}
       int "Number of Logical Units"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_MSD_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_10_AUDIO_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---

       config USB_DEVICE_FUNCTION_10_AUDIO_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_10_AUDIO_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---

	   config USB_DEVICE_FUNCTION_10_AUDIO_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
		
		config USB_DEVICE_FUNCTION_10_AUDIO_2_0_READ_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Read Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
       config USB_DEVICE_FUNCTION_10_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX${INSTANCE}
       int "Audio Write Queue Size"
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_INIT
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_10_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX${INSTANCE}
	   int "Number of Audio Streaming Interfaces"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 1
	   ---help---
	   IDH_HTML_USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES
	   ---endhelp---
	   
	   config USB_DEVICE_FUNCTION_10_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX${INSTANCE}
       int "Maximum Number of Interface Alternate Settings"
       depends on USE_USB_STACK
	   depends on USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX${INSTANCE}
       default 2
	   ---help---
       IDH_HTML_USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING
        ---endhelp---
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



