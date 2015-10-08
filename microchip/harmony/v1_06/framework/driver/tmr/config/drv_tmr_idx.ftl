config DRV_TMR_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on USE_DRV_TMR
<#if INSTANCE != 0>
	default n if DRV_TMR_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_TMR_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_TMR_INST_${INSTANCE}
    depends on USE_DRV_TMR 
<#if INSTANCE != 0>
	             && DRV_TMR_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "TMR Driver Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_TMR_INSTANCES_NUMBER
    ---endhelp---

ifblock DRV_TMR_INST_${INSTANCE}

config DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE}
    string "Timer Module ID"
    depends on USE_DRV_TMR
    range TMR_MODULE_ID
    default "TMR_ID_2" if USE_TMR_MODULE_ID_2_NEEDED_BY_IC_OC
    default "TMR_ID_3" if USE_TMR_MODULE_ID_3_NEEDED_BY_IC_OC
    default "TMR_ID_1"
    ---help---
    IDH_HTML_TMR_MODULE_ID
    ---endhelp---

menu "RTOS Configuration (Instance ${INSTANCE})"
    depends on USE_DRV_TMR
    depends on USE_3RDPARTY_RTOS
    depends on DRV_TMR_DRIVER_MODE = "DYNAMIC"

config DRV_TMR_RTOS_IDX${INSTANCE}
    string "Run This Driver Instance As"
    depends on DRV_TMR_DRIVER_MODE = "DYNAMIC"
    range 3RDPARTY_RTOS_SYS_TASKS_OPTIONS
    default "Combined with System Tasks"

config DRV_TMR_IDX${INSTANCE}_RTOS_TASK_SIZE
    int "Task Size"
    depends on DRV_TMR_RTOS_IDX${INSTANCE} = "Standalone"
    default 1024

config DRV_TMR_IDX${INSTANCE}_RTOS_TASK_PRIORITY
    int "Task Priority"
    depends on DRV_TMR_RTOS_IDX${INSTANCE} = "Standalone"
    default 1

config DRV_TMR_IDX${INSTANCE}_RTOS_USE_DELAY
    bool "Use Task Delay?"
    depends on DRV_TMR_RTOS_IDX${INSTANCE} = "Standalone"
    default y

config DRV_TMR_IDX${INSTANCE}_RTOS_DELAY
    int "Task Delay"
    depends on DRV_TMR_RTOS_IDX${INSTANCE} = "Standalone"
    depends on DRV_TMR_IDX${INSTANCE}_RTOS_USE_DELAY
    default 1000
endmenu

config DRV_TMR_INTERRUPT_SOURCE_IDX${INSTANCE}
    string
    depends on USE_DRV_TMR
    default "INT_SOURCE_TIMER_1" if DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_1"
    default "INT_SOURCE_TIMER_2" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_2") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_3" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_3") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_4" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_4") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_5" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_5") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_6" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_6") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_7" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_7") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_8" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_8") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_9" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_9") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_SOURCE_TIMER_3" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_2") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    default "INT_SOURCE_TIMER_5" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_4") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    default "INT_SOURCE_TIMER_7" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_6") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    default "INT_SOURCE_TIMER_9" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_8") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    ---help---
    IDH_HTML_INT_SOURCE
    ---endhelp---

config DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE}
    string
    depends on USE_DRV_TMR && DRV_TMR_INTERRUPT_MODE
    default "INT_VECTOR_T1" if DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_1"
    default "INT_VECTOR_T2" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_2") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T3" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_3") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T4" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_4") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T5" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_5") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T6" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_6") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T7" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_7") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T8" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_8") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T9" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_9") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_16_BIT"))
    default "INT_VECTOR_T3" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_2") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    default "INT_VECTOR_T5" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_4") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    default "INT_VECTOR_T7" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_6") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    default "INT_VECTOR_T9" if ((DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} = "TMR_ID_8") && (DRV_TMR_OPERATION_MODE_IDX${INSTANCE}= "DRV_TMR_OPERATION_MODE_32_BIT"))
    ---help---
    IDH_HTML_INT_VECTOR
    ---endhelp---

config DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE}
    string "Interrupt Priority"
    depends on USE_DRV_TMR && DRV_TMR_INTERRUPT_MODE
    range INT_PRIORITY_LEVEL
    default "INT_PRIORITY_LEVEL1"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---

config DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX${INSTANCE}
    string "Interrupt Sub-priority"
    depends on USE_DRV_TMR && DRV_TMR_INTERRUPT_MODE
    range INT_SUBPRIORITY_LEVEL
    default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---

config DRV_TMR_INT_PRIO_NUM_IDX${INSTANCE}
    string
    depends on USE_DRV_TMR && DRV_TMR_INTERRUPT_MODE
    default "0" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
    default "1" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
    default "2" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
    default "3" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
    default "4" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
    default "5" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
    default "6" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
    default "7" if DRV_TMR_INTERRUPT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"

config DRV_TMR_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on USE_DRV_TMR && DRV_TMR_INTERRUPT_MODE
    default "_TIMER_1_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T1"
    default "_TIMER_2_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T2"
    default "_TIMER_3_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T3"
    default "_TIMER_4_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T4"
    default "_TIMER_5_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T5"
    default "_TIMER_6_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T6"
    default "_TIMER_7_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T7"
    default "_TIMER_8_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T8"
    default "_TIMER_9_VECTOR" if DRV_TMR_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T9"

ifblock DRV_TMR_DRIVER_MODE = "DYNAMIC"
config DRV_TMR_CLOCK_SOURCE_IDX${INSTANCE}
    string "Clock Source"
    depends on USE_DRV_TMR
    range DRV_TMR_CLK_SOURCES
    default "DRV_TMR_CLKSOURCE_INTERNAL"
    ---help---
    IDH_HTML_DRV_TMR_CLK_SOURCES
    ---endhelp---
endif

ifblock DRV_TMR_DRIVER_MODE = "STATIC"
config DRV_TMR_CLOCK_SOURCE_ST_IDX${INSTANCE}
    string "Clock Source"
    depends on USE_DRV_TMR
    range TMR_SOURCE_CLOCK
    default "TMR_CLOCK_SOURCE_PERIPHERAL_CLOCK"
    ---help---
    IDH_HTML_DRV_TMR_CLK_SOURCES
    ---endhelp---
endif

config DRV_TMR_USE_EXT_INPUT_IDX${INSTANCE}
    string
    depends on USE_DRV_TMR
    default  DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} if DRV_TMR_CLOCK_SOURCE_ST_IDX${INSTANCE} = "TMR_CLOCK_SOURCE_EXTERNAL_INPUT_PIN"
    default DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} if DRV_TMR_CLOCK_SOURCE_IDX${INSTANCE} = "DRV_TMR_CLKSOURCE_EXTERNAL_ASYNCHRONOUS"
    default DRV_TMR_PERIPHERAL_ID_IDX${INSTANCE} if DRV_TMR_CLOCK_SOURCE_IDX${INSTANCE} = "DRV_TMR_CLKSOURCE_EXTERNAL_SYNCHRONOUS"

config DRV_TMR_PRESCALE_IDX${INSTANCE}
    string "Prescale"
    depends on USE_DRV_TMR
    range TMR_PRESCALE
    default "TMR_PRESCALE_VALUE_256"
    ---help---
    IDH_HTML_TMR_PRESCALE
    ---endhelp---

config DRV_TMR_OPERATION_MODE_IDX${INSTANCE}
    string "Operation Mode"
    depends on USE_DRV_TMR
    range DRV_TMR_OPERATION_MODE
    default "DRV_TMR_OPERATION_MODE_16_BIT"
    ---help---
    IDH_HTML_DRV_TMR_OPERATION_MODE
    ---endhelp---

ifblock DRV_TMR_DRIVER_MODE = "DYNAMIC"
config DRV_TMR_ASYNC_WRITE_ENABLE_IDX${INSTANCE}
    bool "Asynchrounous Write Mode Enable"
    depends on USE_DRV_TMR
    default n
    ---help---
    IDH_HTML_DRV_TMR_ASYNC_WRITE_ENABLE
    ---endhelp---

config DRV_TMR_POWER_STATE_IDX${INSTANCE}
    depends on USE_DRV_TMR
    string "Power State"
    range SYS_MODULE_POWER_STATE
    default "SYS_MODULE_POWER_RUN_FULL"
    ---help---
    IDH_HTML_SYSTEM_Library_Interface
    ---endhelp---
endif

ifblock DRV_TMR_DRIVER_MODE = "STATIC"     
config DRV_TMR_PERIOD_IDX${INSTANCE}
    int "Timer Period"
    depends on USE_DRV_TMR
    default 0
    ---help---
    IDH_HTML_PLIB_TMR_Period16BitSet_TMR_MODULE_ID_uint16_t
    ---endhelp---
endif
 
endif
