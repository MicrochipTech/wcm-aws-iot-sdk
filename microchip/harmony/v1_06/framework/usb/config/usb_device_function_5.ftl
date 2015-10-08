

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
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "VENDOR" || USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO" || USB_DEVICE_FUNCTION_5_DEVICE_CLASS_IDX${INSTANCE} = "AUDIO_2_0"
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
       string
	   depends on USE_USB_STACK
       depends on DRV_USB_DEVICE_SUPPORT && PIC32MX
       range USB_DEVICE_FUNCTION_SPEED_FS
       default "USB_SPEED_FULL" 
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
	   ---help---
        IDH_HTML_USB_DEVICE_FUNCTION_REGISTRATION_TABLE
        ---endhelp---
		
		

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
       
       config USB_DEVICE_FUNCTION_5_CDC_INT_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Interrupt Endpoint Number"
       range 1 15 if PIC32MX
       range 1 7  if PIC32MZ
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX${INSTANCE}
	   ---help---
	   IDH_HTML_USB_DEVICE_CDC_INIT
	   ---endhelp---
       
       config USB_DEVICE_FUNCTION_5_CDC_BULK_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Bulk Endpoint Number"
       range 1 15 if PIC32MX
       range 1 7  if PIC32MZ
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX${INSTANCE}
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
       
       config USB_DEVICE_FUNCTION_5_HID_DEVICE_TYPE_IDX${INSTANCE}
       string 
       range USB_DEVICE_HID_TYPE
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX${INSTANCE}
       default "Other"
	   ---help---
	   IDH_HTML_USB_DEVICE_HID_INIT
	   ---endhelp---
       
       config USB_DEVICE_FUNCTION_5_HID_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Endpoint Number"
       range 1 15 if PIC32MX
       range 1 7  if PIC32MZ
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX${INSTANCE}
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
        
       config USB_DEVICE_FUNCTION_5_MSD_ENDPOINT_NUMBER_IDX${INSTANCE}
       int "Endpoint Number"
       range 1 15 if PIC32MX
       range 1 7  if PIC32MZ
       depends on USE_USB_STACK
       depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX${INSTANCE}
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
        
        config USB_DEVICE_FUNCTION_5_VENDOR_ENDPOINT_NUMBER_IDX${INSTANCE}
        int "Endpoint Number"
        range 1 15 if PIC32MX
        range 1 7  if PIC32MZ
        depends on USE_USB_STACK
        depends on USB_DEVICE_FUNCTION_5_DEVICE_CLASS_VENDOR_IDX${INSTANCE}
        ---help---
        IDH_HTML_USB_DEVICE_INIT 
        ---endhelp---
        
	   
