config DRV_SPI_USE_SSIN_PIN_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER 
    default DRV_SPI_SPI_ID_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "DYNAMIC" && DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED" && DRV_SPI_SPI_MODE_SLAVE_IDX${INSTANCE}
    default DRV_SPI_SPI_ID_STATIC_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "STATIC" && DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED" && DRV_SPI_SPI_MODE_STATIC_IDX${INSTANCE} = "DRV_SPI_MODE_SLAVE"

config DRV_SPI_USE_SSOUT_PIN_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER 
    default DRV_SPI_SPI_ID_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "DYNAMIC" && DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED" && DRV_SPI_SPI_MODE_MASTER_IDX${INSTANCE}
    default DRV_SPI_SPI_ID_STATIC_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "STATIC" && DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED" && DRV_SPI_SPI_MODE_STATIC_IDX${INSTANCE} = "DRV_SPI_MODE_MASTER"

config DRV_SPI_USE_SDO_PIN_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER 
    default DRV_SPI_SPI_ID_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "DYNAMIC"
    default DRV_SPI_SPI_ID_STATIC_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "STATIC"

config DRV_SPI_USE_SDI_PIN_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER 
    default DRV_SPI_SPI_ID_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "DYNAMIC"
    default DRV_SPI_SPI_ID_STATIC_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "STATIC"

config DRV_SPI_USE_SCK_PIN_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER 
    default DRV_SPI_SPI_ID_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "DYNAMIC"
    default DRV_SPI_SPI_ID_STATIC_IDX${INSTANCE} if DRV_SPI_DRIVER_MODE = "STATIC"


ifblock DRV_SPI_DRIVER_MODE = "DYNAMIC"

config DRV_SPI_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on DRV_SPI_USE_DRIVER
<#if INSTANCE != 0>
	default n if DRV_SPI_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_SPI_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER 
<#if INSTANCE != 0>
	             && DRV_SPI_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "SPI Driver Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_SPI_INSTANCES_NUMBER
    ---endhelp---
    
ifblock DRV_SPI_IDX${INSTANCE}

menu "RTOS Configuration (Instance ${INSTANCE})"
    depends on DRV_SPI_USE_DRIVER
    depends on USE_3RDPARTY_RTOS
    depends on DRV_SPI_DRIVER_MODE = "DYNAMIC"

config DRV_SPI_RTOS_IDX${INSTANCE}
    string "Run This Driver Instance As"
    depends on DRV_SPI_DRIVER_MODE = "DYNAMIC"
    range 3RDPARTY_RTOS_SYS_TASKS_OPTIONS
    default "Standalone"

config DRV_SPI_IDX${INSTANCE}_RTOS_TASK_SIZE
    int "Task Size"
    depends on DRV_SPI_RTOS_IDX${INSTANCE} = "Standalone"
    default 1024

config DRV_SPI_IDX${INSTANCE}_RTOS_TASK_PRIORITY
    int "Task Priority"
    depends on DRV_SPI_RTOS_IDX${INSTANCE} = "Standalone"
    default 1

config DRV_SPI_IDX${INSTANCE}_RTOS_USE_DELAY
    bool "Use Task Delay?"
    depends on DRV_SPI_RTOS_IDX${INSTANCE} = "Standalone"
    default y

config DRV_SPI_IDX${INSTANCE}_RTOS_DELAY
    int "Task Delay"
    depends on DRV_SPI_RTOS_IDX${INSTANCE} = "Standalone"
    depends on DRV_SPI_IDX${INSTANCE}_RTOS_USE_DELAY
    default 1000
endmenu

comment "This instance is in use by the Wi-Fi driver. Do Not Change Settings!"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}

config DRV_SPI_SPI_ID_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    range SPI_MODULE_ID
    string "SPI Module ID"
    default "SPI_ID_1" if DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
    default "SPI_ID_1" if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && !BSP_PIC32MZ_EC_SK_MEB2
    default "SPI_ID_4" if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && BSP_PIC32MZ_EC_SK_MEB2
    ---help---
    IDH_HTML_SPI_MODULE_ID
    ---endhelp---

choice
    prompt "Driver Mode"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    default DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} if DRV_SPI_USE_ISR_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
    default DRV_SPI_TASK_MODE_POLLED_IDX${INSTANCE} if !DRV_SPI_USE_ISR_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
    default DRV_SPI_TASK_MODE_POLLED_IDX${INSTANCE} if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}

config DRV_SPI_TASK_MODE_POLLED_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_POLLED_MODE
    bool "Polled Mode"
    ---help---
    IDH_HTML_DRV_SPI_POLLED
    ---endhelp---
    
config DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_ISR_MODE
    bool "Interrupt Mode"
    ---help---
    IDH_HTML_DRV_SPI_ISR
    ---endhelp---
endchoice

comment "SPI Instance used by Wi-Fi driver only works in Polled Mode"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && !DRV_SPI_TASK_MODE_POLLED_IDX${INSTANCE}

config DRV_SPI_TX_INT_SOURCE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    string
    range INT_SOURCE
    default "INT_SOURCE_SPI_1_TRANSMIT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_SOURCE_SPI_2_TRANSMIT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_SOURCE_SPI_3_TRANSMIT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_SOURCE_SPI_4_TRANSMIT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_SOURCE_SPI_5_TRANSMIT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_SOURCE_SPI_6_TRANSMIT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"
    ---help---
    IDH_HTML_INT_SOURCE
    ---endhelp---

config DRV_SPI_RX_INT_SOURCE_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    range INT_SOURCE
    default "INT_SOURCE_SPI_1_RECEIVE" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_SOURCE_SPI_2_RECEIVE" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_SOURCE_SPI_3_RECEIVE" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_SOURCE_SPI_4_RECEIVE" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_SOURCE_SPI_5_RECEIVE" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_SOURCE_SPI_6_RECEIVE" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"
    ---help---
    IDH_HTML_INT_SOURCE
    ---endhelp---

config DRV_SPI_ERROR_INT_SOURCE_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    range INT_SOURCE
    default "INT_SOURCE_SPI_1_ERROR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_SOURCE_SPI_2_ERROR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_SOURCE_SPI_3_ERROR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_SOURCE_SPI_4_ERROR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_SOURCE_SPI_5_ERROR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_SOURCE_SPI_6_ERROR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"
    ---help---
    IDH_HTML_INT_SOURCE
    ---endhelp---

#PIC32MX
config DRV_SPI_INT_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE}
    default "INT_VECTOR_SPI1" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_VECTOR_SPI2" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_VECTOR_SPI3" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_VECTOR_SPI4" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_VECTOR_SPI5" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_VECTOR_SPI6" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"
    
config DRV_SPI_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MX
    default "_SPI_1_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "_SPI_2_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "_SPI_3_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "_SPI_4_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "_SPI_5_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "_SPI_6_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"

config DRV_SPI_INT_PRIORITY_IDX${INSTANCE}
    string "SPI Interrupt Priority"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MX
    range INT_PRIORITY_LEVEL
    default "INT_PRIORITY_LEVEL1"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---

config DRV_SPI_INT_SUB_PRIORITY_IDX${INSTANCE}
    string "SPI Interrupt Sub-priority"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MX
    range INT_SUBPRIORITY_LEVEL
    default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---

config DRV_SPI_INT_PRIO_NUM_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MX
    default "0" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
    default "1" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
    default "2" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
    default "3" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
    default "4" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
    default "5" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
    default "6" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
    default "7" if DRV_SPI_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"

#PIC32MZ
# Transmit
config DRV_SPI_TX_INT_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE}
    default "INT_VECTOR_SPI1_TX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_VECTOR_SPI2_TX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_VECTOR_SPI3_TX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_VECTOR_SPI4_TX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_VECTOR_SPI5_TX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_VECTOR_SPI6_TX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"

config DRV_SPI_TX_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
    default "_SPI1_TX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "_SPI2_TX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "_SPI3_TX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "_SPI4_TX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "_SPI5_TX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "_SPI6_TX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"
    
config DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
	string "TX Interrupt Priority"
	range INT_PRIORITY_LEVEL
	default "INT_PRIORITY_LEVEL1"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---
	
config DRV_SPI_TX_INT_SUB_PRIORITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
	string "TX Interrupt Sub-priority"
	range INT_SUBPRIORITY_LEVEL
	default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---

config DRV_SPI_TX_INT_PRIO_NUM_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
    default "0" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
    default "1" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
    default "2" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
    default "3" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
    default "4" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
    default "5" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
    default "6" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
    default "7" if DRV_SPI_TX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"	

#Receive
config DRV_SPI_RX_INT_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE}
    default "INT_VECTOR_SPI1_RX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_VECTOR_SPI2_RX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_VECTOR_SPI3_RX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_VECTOR_SPI4_RX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_VECTOR_SPI5_RX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_VECTOR_SPI6_RX" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"

config DRV_SPI_RX_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
    default "_SPI1_RX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "_SPI2_RX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "_SPI3_RX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "_SPI4_RX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "_SPI5_RX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "_SPI6_RX_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"

config DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
	string "RX Interrupt Priority"
	range INT_PRIORITY_LEVEL
	default "INT_PRIORITY_LEVEL1"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---
	
config DRV_SPI_RX_INT_SUB_PRIORITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
	string "RX Interrupt Sub-priority"
	range INT_SUBPRIORITY_LEVEL
	default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---

config DRV_SPI_RX_INT_PRIO_NUM_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
    default "0" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
    default "1" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
    default "2" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
    default "3" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
    default "4" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
    default "5" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
    default "6" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
    default "7" if DRV_SPI_RX_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"	

#Error
config DRV_SPI_ERROR_INT_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE}
    default "INT_VECTOR_SPI1_FAULT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "INT_VECTOR_SPI2_FAULT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "INT_VECTOR_SPI3_FAULT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "INT_VECTOR_SPI4_FAULT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "INT_VECTOR_SPI5_FAULT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "INT_VECTOR_SPI6_FAULT" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"

config DRV_SPI_ERROR_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
    default "_SPI1_FAULT_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_1"
    default "_SPI2_FAULT_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_2"
    default "_SPI3_FAULT_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_3"
    default "_SPI4_FAULT_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_4"
    default "_SPI5_FAULT_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_5"
    default "_SPI6_FAULT_VECTOR" if DRV_SPI_SPI_ID_IDX${INSTANCE} = "SPI_ID_6"

config DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
	string "Error Interrupt Priority"
	range INT_PRIORITY_LEVEL
	default "INT_PRIORITY_LEVEL1"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---
	
config DRV_SPI_ERROR_INT_SUB_PRIORITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
	string "Error Interrupt Sub-priority"
	range INT_SUBPRIORITY_LEVEL
	default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---

config DRV_SPI_ERROR_INT_PRIO_NUM_IDX${INSTANCE}
    string
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_ISR_IDX${INSTANCE} && PIC32MZ
    default "0" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
    default "1" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
    default "2" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
    default "3" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
    default "4" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
    default "5" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
    default "6" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
    default "7" if DRV_SPI_ERROR_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"
    
choice
prompt "Master/Slave Mode"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
default DRV_SPI_SPI_MODE_MASTER_IDX${INSTANCE} if DRV_SPI_USE_MASTER_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_SPI_MODE_SLAVE_IDX${INSTANCE} if DRV_SPI_USE_SLAVE_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_SPI_MODE_MASTER_IDX${INSTANCE} if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}

config DRV_SPI_SPI_MODE_MASTER_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_IDX${INSTANCE} && DRV_SPI_USE_MASTER_MODE
    bool "Master"
    ---help---
    IDH_HTML_DRV_SPI_MASTER
    ---endhelp---
    
config DRV_SPI_SPI_MODE_SLAVE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_IDX${INSTANCE} && DRV_SPI_USE_SLAVE_MODE
    bool "Slave"
    ---help---
    IDH_HTML_DRV_SPI_SLAVE
    ---endhelp---
endchoice

comment "SPI Instance used by Wi-Fi driver only works in Master Mode"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && !DRV_SPI_SPI_MODE_MASTER_IDX${INSTANCE}
    
choice
prompt "Data Width"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
default DRV_SPI_COMM_WIDTH_32_BIT_IDX${INSTANCE} if DRV_SPI_USE_32BIT_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_COMM_WIDTH_16_BIT_IDX${INSTANCE} if DRV_SPI_USE_16BIT_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_COMM_WIDTH_8_BIT_IDX${INSTANCE} if DRV_SPI_USE_8BIT_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_COMM_WIDTH_8_BIT_IDX${INSTANCE} if DRV_SPI_USE_8BIT_MODE && DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}

config DRV_SPI_COMM_WIDTH_8_BIT_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_8BIT_MODE
    bool "8-bit"
    ---help---
    IDH_HTML_DRV_SPI_8BIT
    ---endhelp---

config DRV_SPI_COMM_WIDTH_16_BIT_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}  && DRV_SPI_USE_16BIT_MODE
    bool "16-bit"
    ---help---
    IDH_HTML_DRV_SPI_16BIT
    ---endhelp---

config DRV_SPI_COMM_WIDTH_32_BIT_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}  && DRV_SPI_USE_32BIT_MODE
    bool "32-bit"
    ---help---
    IDH_HTML_DRV_SPI_32BIT
    ---endhelp---
endchoice

comment "SPI Instance used by Wi-Fi driver only works in 8-bit Mode"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && !DRV_SPI_COMM_WIDTH_8_BIT_IDX${INSTANCE}

choice
prompt "Buffer Mode"
    depends on DRV_SPI_USE_DRIVER
default DRV_SPI_BUFFER_ENHANCED_IDX${INSTANCE} if DRV_SPI_USE_ENHANCED_BUFFER_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_BUFFER_STANDARD_IDX${INSTANCE} if DRV_SPI_USE_STANDARD_BUFFER_MODE && DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
default DRV_SPI_BUFFER_ENHANCED_IDX${INSTANCE} if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}

config DRV_SPI_BUFFER_STANDARD_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE} && DRV_SPI_USE_STANDARD_BUFFER_MODE
    bool "Standard"
    ---help---
    IDH_HTML_DRV_SPI_RM
    ---endhelp---
    
config DRV_SPI_BUFFER_ENHANCED_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE} && DRV_SPI_USE_ENHANCED_BUFFER_MODE
    bool "Enhanced"
    ---help---
    IDH_HTML_DRV_SPI_EBM
    ---endhelp---
endchoice

comment "SPI Instance used by Wi-Fi driver only works in Enhanced Buffer Mode"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && !DRV_SPI_BUFFER_ENHANCED_IDX${INSTANCE}
    
config DRV_SPI_ALLOW_IDLE_RUN_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    bool "Allow Idle Run"
    default n
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---

config DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    string "Protocol Type"
    range DRV_SPI_PROTOCOL_TYPE
    default "DRV_SPI_PROTOCOL_TYPE_STANDARD"
    ---help---
    IDH_HTML_DRV_SPI_PROTOCOL_TYPE
    ---endhelp---

comment "SPI Instance used by Wi-Fi driver only works in Standard Protocol Type"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} != "DRV_SPI_PROTOCOL_TYPE_STANDARD"

config DRV_SPI_FRAME_SYNC_PULSE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Sync Pulse"
    range SPI_FRAME_SYNC_PULSE
    default "SPI_FRAME_SYNC_PULSE_ON_EVERY_DATA_CHARACTER"
    ---help---
    IDH_HTML_SPI_FRAME_SYNC_PULSE
    ---endhelp---

config DRV_SPI_FRAME_PULSE_POLARITY_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Polarity"
    range SPI_FRAME_PULSE_POLARITY
    default "SPI_FRAME_PULSE_POLARITY_ACTIVE_LOW"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_POLARITY
    ---endhelp---
    
config DRV_SPI_FRAME_PULSE_DIRECTION_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Direction"
    range SPI_FRAME_PULSE_DIRECTION
    default "SPI_FRAME_PULSE_DIRECTION_OUTPUT"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_DIRECTION
    ---endhelp---
    
config DRV_SPI_FRAME_PULSE_EDGE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Edge"
    range SPI_FRAME_PULSE_EDGE
    default "SPI_FRAME_PULSE_EDGE_PRECEDES_FIRST_BIT_CLOCK"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_EDGE
    ---endhelp---
    
config DRV_SPI_FRAME_PULSE_WIDTH_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Width"
    range SPI_FRAME_PULSE_WIDTH
    default "SPI_FRAME_PULSE_WIDTH_ONE_CLOCK_WIDE"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_WIDTH
    ---endhelp---
    
config DRV_SPI_AUDIO_TRANSMIT_MODE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_AUDIO")
    string "Audio Transmit Mode"
    range SPI_AUDIO_TRANSMIT_MODE
    default "SPI_AUDIO_TRANSMIT_STEREO"
    ---help---
    IDH_HTML_SPI_AUDIO_TRANSMIT_MODE
    ---endhelp---
    
config DRV_SPI_AUDIO_PROTOCOL_MODE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_AUDIO")
    string "Audio Protocol Mode"
    range SPI_AUDIO_PROTOCOL
    default "SPI_AUDIO_PROTOCOL_I2S"
    ---help---
    IDH_HTML_SPI_AUDIO_PROTOCOL
    ---endhelp---
    
config DRV_SPI_SPI_CLOCK_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_IDX${INSTANCE} && USE_SYS_CLK
    string "Clock To Use"
    range CLK_BUSES_PERIPHERAL
    default "CLK_BUS_PERIPHERAL_2"
    ---help---
    IDH_HTML_CLK_BUSES_PERIPHERAL
    ---endhelp---
    
config DRV_SPI_BAUD_RATE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_IDX${INSTANCE}
    int "SPI Clock Rate - Hz"
    default 1000000 if DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
    default 8000000 if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---

config DRV_SPI_CLOCK_MODE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    string "Clock Mode"
    range DRV_SPI_CLOCK_MODE
    default "DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_RISE" if DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
    default "DRV_SPI_CLOCK_MODE_IDLE_HIGH_EDGE_FALL" if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}
    ---help---
    IDH_HTML_DRV_SPI_CLOCK_MODE
    ---endhelp---

comment "SPI Instance used by Wi-Fi driver only works when the Clock Mode is set to IDLE_HIGH_EDGE_FALL"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && DRV_SPI_CLOCK_MODE_IDX${INSTANCE} != "DRV_SPI_CLOCK_MODE_IDLE_HIGH_EDGE_FALL"

config DRV_SPI_INPUT_PHASE_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_IDX${INSTANCE}
    string "Input Phase"
    range SPI_INPUT_SAMPLING_PHASE
    default "SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE" if DRV_WIFI_SPI_INSTANCE_INDEX != ${INSTANCE}
    default "SPI_INPUT_SAMPLING_PHASE_AT_END" if DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE}
    ---help---
    IDH_HTML_SPI_INPUT_SAMPLING_PHASE
    ---endhelp---

config DRV_SPI_USE_DMA_IDX${INSTANCE}
	bool "Use DMA?"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
	depends on DRV_SPI_USE_DMA && DRV_SPI_IDX${INSTANCE}
	default n
	   ---help---
    IDH_HTML_DRV_SPI_DMA
    ---endhelp---
comment "System DMA service should be configured to select  DMA Channel"	
depends on DRV_SPI_USE_DMA_IDX${INSTANCE} && DRV_SPI_IDX${INSTANCE}
 
config DRV_SPI_TX_DMA_INST_IDX${INSTANCE}
	int "TX DMA Instance Index"	
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_DMA_IDX${INSTANCE}
	default 1
	---help---
	IDH_HTML_DMA_CHANNEL
	---endhelp---
	
config DRV_SPI_TX_DMA_THRESHOLD_IDX${INSTANCE}
    int "TX DMA Threshold"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_DMA_IDX${INSTANCE}
    default 4
	
config DRV_SPI_RX_DMA_INST_IDX${INSTANCE}
	string "RX DMA Instance"	
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_DMA_IDX${INSTANCE}
	default 0
	---help---
	IDH_HTML_DMA_CHANNEL
	---endhelp---
    
config DRV_SPI_RX_DMA_THRESHOLD_IDX${INSTANCE}
    int "RX DMA Threshold"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_USE_DMA_IDX${INSTANCE}
    default 4
	
	
config DRV_SPI_QUEUE_SIZE_IDX${INSTANCE}
    int "Max Jobs In Queue"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_IDX${INSTANCE}
    default 10
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---
    
config DRV_SPI_RESERVED_JOB_IDX${INSTANCE}
    int "Minimum Number Of Job Queue Reserved For Instance"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    default 1
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---
    
config DRV_SPI_TRANSACTIONS_PER_SM_RUN_IDX${INSTANCE}
    int "Number Of Transactions Before Yield"
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_IDX${INSTANCE}
    depends on DRV_SPI_TASK_MODE_POLLED_IDX${INSTANCE} && DRV_SPI_BUFFER_STANDARD_IDX${INSTANCE}
    default 16
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---
    
endif
endif



ifblock DRV_SPI_DRIVER_MODE = "STATIC"
config DRV_SPI_STATIC_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on DRV_SPI_USE_DRIVER
<#if INSTANCE != 0>
	default n if DRV_SPI_STATIC_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_SPI_STATIC_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_SPI_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER 
<#if INSTANCE != 0>
	             && DRV_SPI_STATIC_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "SPI Driver Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_SPI_INSTANCES_NUMBER
    ---endhelp---

ifblock DRV_SPI_STATIC_IDX${INSTANCE}

comment "SPI Instance used by Wi-Fi driver only works in Dynamic Mode"
    depends on DRV_WIFI_SPI_INSTANCE_INDEX = ${INSTANCE} && DRV_SPI_STATIC_IDX${INSTANCE}

config DRV_SPI_SPI_ID_STATIC_IDX${INSTANCE}
    range SPI_MODULE_ID
    depends on DRV_SPI_USE_DRIVER
    string "SPI Module ID"
    default "SPI_ID_1"
    ---help---
    IDH_HTML_SPI_MODULE_ID
    ---endhelp---
        
config DRV_SPI_SPI_MODE_STATIC_IDX${INSTANCE}
    string "SPI Mode"
    depends on DRV_SPI_USE_DRIVER
    range DRV_SPI_MODE
    default "DRV_SPI_MODE_MASTER"
    ---help---
    IDH_HTML_DRV_SPI_MASTER
    ---endhelp---
        
config DRV_SPI_COMM_WIDTH_STATIC_IDX${INSTANCE}
    string "Communications Width"
    depends on DRV_SPI_USE_DRIVER
    range SPI_COMMUNICATION_WIDTH
    default "SPI_COMMUNICATION_WIDTH_8BITS"
    ---help---
    IDH_HTML_DRV_SPI_8BIT
    ---endhelp---
    
config DRV_SPI_BUFFER_STATIC_IDX${INSTANCE}
    string "Buffer Mode"
    depends on DRV_SPI_USE_DRIVER
    range DRV_SPI_BUFFER_MODE
    default "DRV_SPI_BUFFER_TYPE_ENHANCED" if ENHANCED_BUFFER_MODE_SUPPORT
    default "DRV_SPI_BUFFER_TYPE_STANDARD" if NO_ENHANCED_BUFFER_MODE_SUPPORT    
    ---help---
    IDH_HTML_DRV_SPI_RM
    ---endhelp---
        
config DRV_SPI_ALLOW_IDLE_RUN_STATIC_IDX${INSTANCE}
    bool "Allow Idle Run"
    depends on DRV_SPI_USE_DRIVER
    default n
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---

config DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE}
    string "Protocol Type"
    depends on DRV_SPI_USE_DRIVER
    range DRV_SPI_PROTOCOL_TYPE
    default "DRV_SPI_PROTOCOL_TYPE_STANDARD"
    ---help---
    IDH_HTML_DRV_SPI_PROTOCOL_TYPE
    ---endhelp---
    
config DRV_SPI_FRAME_SYNC_PULSE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Sync Pulse"
    range SPI_FRAME_SYNC_PULSE
    default "SPI_FRAME_SYNC_PULSE_ON_EVERY_DATA_CHARACTER"
    ---help---
    IDH_HTML_SPI_FRAME_SYNC_PULSE
    ---endhelp---

config DRV_SPI_FRAME_PULSE_POLARITY_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Polarity"
    range SPI_FRAME_PULSE_POLARITY
    default "SPI_FRAME_PULSE_POLARITY_ACTIVE_LOW"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_POLARITY
    ---endhelp---
    
config DRV_SPI_FRAME_PULSE_DIRECTION_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Direction"
    range SPI_FRAME_PULSE_DIRECTION
    default "SPI_FRAME_PULSE_DIRECTION_OUTPUT"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_DIRECTION
    ---endhelp---
    
config DRV_SPI_FRAME_PULSE_EDGE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Edge"
    range SPI_FRAME_PULSE_EDGE
    default "SPI_FRAME_PULSE_EDGE_PRECEDES_FIRST_BIT_CLOCK"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_EDGE
    ---endhelp---
    
config DRV_SPI_FRAME_PULSE_WIDTH_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_FRAMED")
    string "Frame Pulse Width"
    range SPI_FRAME_PULSE_WIDTH
    default "SPI_FRAME_PULSE_WIDTH_ONE_CLOCK_WIDE"
    ---help---
    IDH_HTML_SPI_FRAME_PULSE_WIDTH
    ---endhelp---
    
config DRV_SPI_AUDIO_TRANSMIT_MODE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_AUDIO")
    string "Audio Transmit Mode"
    range SPI_AUDIO_TRANSMIT_MODE
    default "SPI_AUDIO_TRANSMIT_STEREO"
    ---help---
    IDH_HTML_SPI_AUDIO_TRANSMIT_MODE
    ---endhelp---
    
config DRV_SPI_AUDIO_PROTOCOL_MODE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on (DRV_SPI_SPI_PROTOCOL_TYPE_STATIC_IDX${INSTANCE} = "DRV_SPI_PROTOCOL_TYPE_AUDIO")
    string "Audio Protocol Mode"
    range SPI_AUDIO_PROTOCOL
    default "SPI_AUDIO_PROTOCOL_I2S"
    ---help---
    IDH_HTML_SPI_AUDIO_PROTOCOL
    ---endhelp---

config DRV_SPI_SPI_CLOCK_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_STATIC_IDX${INSTANCE} && USE_SYS_CLK
    string "Clock To Use"
    range CLK_BUSES_PERIPHERAL
    default "CLK_BUS_PERIPHERAL_2"
    ---help---
    IDH_HTML_CLK_BUSES_PERIPHERAL
    ---endhelp---

config DRV_SPI_BAUD_RATE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_STATIC_IDX${INSTANCE}
    int "SPI Clock Rate - Hz"
    default 1000000
    ---help---
    IDH_HTML_DRV_SPI_INIT
    ---endhelp---

config DRV_SPI_CLOCK_MODE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_STATIC_IDX${INSTANCE}
    string "Clock Mode"
    range DRV_SPI_CLOCK_MODE
    default "DRV_SPI_CLOCK_MODE_IDLE_LOW_EDGE_RISE"
    ---help---
    IDH_HTML_DRV_SPI_CLOCK_MODE
    ---endhelp---
    
config DRV_SPI_INPUT_PHASE_STATIC_IDX${INSTANCE}
    depends on DRV_SPI_USE_DRIVER
    depends on DRV_SPI_STATIC_IDX${INSTANCE}
    string "Input Phase"
    range SPI_INPUT_SAMPLING_PHASE
    default "SPI_INPUT_SAMPLING_PHASE_IN_MIDDLE"
    ---help---
    IDH_HTML_SPI_INPUT_SAMPLING_PHASE
    ---endhelp---
    
endif
endif
    
