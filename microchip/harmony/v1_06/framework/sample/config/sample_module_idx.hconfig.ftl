config SAMPLE_MODULE_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
<#if INSTANCE != 0>
	default n if SAMPLE_MODULE_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if SAMPLE_MODULE_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config SAMPLE_MODULE_INST_IDX${INSTANCE}
    depends on USE_SAMPLE_MODULE 
<#if INSTANCE != 0>
	             && SAMPLE_MODULE_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "Sample Module Instance ${INSTANCE}"
    default y
    ---help---
    Use sample module instance ${INSTANCE}
    ---endhelp---

ifblock SAMPLE_MODULE_INST_IDX${INSTANCE}

config SAMPLE_MODULE_POWER_STATE_IDX${INSTANCE}
    string "Power State"
    range SYS_MODULE_POWER_STATE
    default "SYS_MODULE_POWER_RUN_FULL"
    ---help---
    Select the initial power state.
    ---endhelp---
    
config SAMPLE_MODULE_SOME_DATA_IDX${INSTANCE}
    int "Some Data"
    default 42
    ---help---
    Some init data for the sample module.
    ---endhelp---


menu "Timer Configuration"
    depends on SAMPLE_MODULE_INTERRUPT_MODE

config SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE}
    string "Timer Module ID"
    depends on USE_SAMPLE_MODULE
    range TMR_MODULE_ID
    default "TMR_ID_2" if ${INSTANCE} = 0
    default "TMR_ID_3" if ${INSTANCE} = 1
    ---help---
    IDH_HTML_TMR_MODULE_ID
    ---endhelp---

config SAMPLE_MODULE_TIMER_CLOCK_PRESCALER_IDX${INSTANCE}
    string "Prescale Divisor"
    depends on USE_SAMPLE_MODULE
    range TMR_PRESCALE
    default "TMR_PRESCALE_VALUE_256"
    ---help---
    IDH_HTML_TMR_PRESCALE
    ---endhelp---

config SAMPLE_MODULE_TIMER_PERIOD_IDX${INSTANCE}
    int "Timer Interrupt Period"
    depends on USE_SAMPLE_MODULE
    default 31250
    ---help---
    IDH_HTML_PLIB_TMR_Period16BitSet_TMR_MODULE_ID_uint16_t
    ---endhelp---
    
config SAMPLE_MODULE_INTERRUPT_SOURCE_IDX${INSTANCE}
    string
    depends on USE_SAMPLE_MODULE 
    default "INT_SOURCE_TIMER_1" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_1"
    default "INT_SOURCE_TIMER_2" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_2"
    default "INT_SOURCE_TIMER_3" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_3"
    default "INT_SOURCE_TIMER_4" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_4"
    default "INT_SOURCE_TIMER_5" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_5"
    default "INT_SOURCE_TIMER_6" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_6"
    default "INT_SOURCE_TIMER_7" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_7"
    default "INT_SOURCE_TIMER_8" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_8"
    default "INT_SOURCE_TIMER_9" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_9"
    ---help---
    IDH_HTML_INT_SOURCE
    ---endhelp---
    
config SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE}
    string
    depends on USE_SAMPLE_MODULE
    default "INT_VECTOR_T1" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_1"
    default "INT_VECTOR_T2" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_2"
    default "INT_VECTOR_T3" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_3"
    default "INT_VECTOR_T4" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_4"
    default "INT_VECTOR_T5" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_5"
    default "INT_VECTOR_T6" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_6"
    default "INT_VECTOR_T7" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_7"
    default "INT_VECTOR_T8" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_8"
    default "INT_VECTOR_T9" if SAMPLE_MODULE_TIMER_ID_IDX${INSTANCE} = "TMR_ID_9"
    ---help---
    IDH_HTML_INT_VECTOR
    ---endhelp---

config SAMPLE_MODULE_TIMER_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on USE_TEST_HARNESS
    default "_TIMER_1_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T1"
    default "_TIMER_2_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T2"
    default "_TIMER_3_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T3"
    default "_TIMER_4_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T4"
    default "_TIMER_5_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T5"
    default "_TIMER_6_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T6"
    default "_TIMER_7_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T7"
    default "_TIMER_8_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T8"
    default "_TIMER_9_VECTOR" if SAMPLE_MODULE_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_T9"
    
config SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX${INSTANCE}
    string "Interrupt Priority"
    depends on USE_SAMPLE_MODULE
    range INT_PRIORITY_LEVEL
    default "INT_PRIORITY_LEVEL2"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---

config SAMPLE_MODULE_TIMER_INTERRUPT_SUBPRIORITY_IDX${INSTANCE}
    string "Interrupt Sub-priority"
    depends on USE_SAMPLE_MODULE
    range INT_SUBPRIORITY_LEVEL
    default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---

endmenu


menu "RTOS Configuration"
    depends on USE_SAMPLE_MODULE
    depends on USE_3RDPARTY_RTOS

config SAMPLE_RTOS_DAEMON_IDX${INSTANCE}
    string "Run Library Tasks As"
    range 3RDPARTY_RTOS_SYS_TASKS_OPTIONS
    default "Standalone"

config SAMPLE_RTOS_TASK_SIZE_IDX${INSTANCE}
    int "Task Size"
    depends on SAMPLE_RTOS_DAEMON_IDX${INSTANCE} = "Standalone"
    default 1024

config SAMPLE_RTOS_TASK_PRIORITY_IDX${INSTANCE}
    int "Task Priority"
    depends on SAMPLE_RTOS_DAEMON_IDX${INSTANCE} = "Standalone"
    default 1

config SAMPLE_RTOS_USE_DELAY_IDX${INSTANCE}
    bool "Use Task Delay?"
    depends on SAMPLE_RTOS_DAEMON_IDX${INSTANCE} = "Standalone"
    default y

config SAMPLE_RTOS_DELAY_IDX${INSTANCE}
    int "Task Delay"
    depends on SAMPLE_RTOS_DAEMON_IDX${INSTANCE} = "Standalone"
    depends on SAMPLE_RTOS_USE_DELAY_IDX${INSTANCE}
    default 500
endmenu


endif # SAMPLE_MODULE_INST_IDX${INSTANCE}


	