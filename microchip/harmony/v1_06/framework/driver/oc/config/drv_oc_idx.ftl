config DRV_OC_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on USE_DRV_OC
<#if INSTANCE != 0>
	default n if DRV_OC_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_OC_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_OC_INST_IDX${INSTANCE}
    depends on USE_DRV_OC 
<#if INSTANCE != 0>
	             && DRV_OC_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "OC Driver Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_OC_INSTANCES_NUMBER
    ---endhelp---

ifblock DRV_OC_INST_IDX${INSTANCE}

config DRV_OC_PERIPHERAL_ID_IDX${INSTANCE}
    string "OC module ID"
    depends on USE_DRV_OC
    range OC_MODULE_ID
    default "OC_ID_1"
    ---help---
    IDH_HTML_OC_MODULE_ID
    ---endhelp---

config DRV_OC_INTERRUPT_SOURCE_IDX${INSTANCE}
    string
    depends on USE_DRV_OC
    depends on DRV_OC_INTERRUPT_MODE
    range INT_SOURCE
    default "INT_SOURCE_OUTPUT_COMPARE_1" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_1"
    default "INT_SOURCE_OUTPUT_COMPARE_2" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_2"
    default "INT_SOURCE_OUTPUT_COMPARE_3" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_3"
    default "INT_SOURCE_OUTPUT_COMPARE_4" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_4"
    default "INT_SOURCE_OUTPUT_COMPARE_5" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_5"
    default "INT_SOURCE_OUTPUT_COMPARE_6" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_6"
    default "INT_SOURCE_OUTPUT_COMPARE_7" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_7"
    default "INT_SOURCE_OUTPUT_COMPARE_8" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_8"
    default "INT_SOURCE_OUTPUT_COMPARE_9" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_9"
    
    ---help---
      IDH_HTML_INT_SOURCE
    ---endhelp---
    
config DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE}
    string
    depends on USE_DRV_OC
    depends on DRV_OC_INTERRUPT_MODE
    range INT_VECTOR
    default "INT_VECTOR_OC1" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_1"
    default "INT_VECTOR_OC2" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_2"
    default "INT_VECTOR_OC3" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_3"
    default "INT_VECTOR_OC4" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_4"
    default "INT_VECTOR_OC5" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_5"
    default "INT_VECTOR_OC6" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_6"
    default "INT_VECTOR_OC7" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_7"
    default "INT_VECTOR_OC8" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_8"
    default "INT_VECTOR_OC9" if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_9"
    ---help---
      IDH_HTML_INT_VECTOR
    ---endhelp---
    
config DRV_OC_INT_PRIORITY_IDX${INSTANCE}
    string "Interrupt Priority"
    depends on USE_DRV_OC
    depends on DRV_OC_INTERRUPT_MODE
    range INT_PRIORITY_LEVEL
    default "INT_PRIORITY_LEVEL1"
    ---help---
    IDH_HTML_INT_PRIORITY_LEVEL
    ---endhelp---

config DRV_OC_INT_SUB_PRIORITY_IDX${INSTANCE}
    string "Interrupt Sub-priority"
    depends on USE_DRV_OC
    depends on DRV_OC_INTERRUPT_MODE
    range INT_SUBPRIORITY_LEVEL
    default "INT_SUBPRIORITY_LEVEL0"
    ---help---
    IDH_HTML_INT_SUBPRIORITY_LEVEL
    ---endhelp---
    
config DRV_OC_INT_PRIO_NUM_IDX${INSTANCE}
    string
    depends on USE_DRV_OC
    depends on DRV_OC_INTERRUPT_MODE
    default "0" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_DISABLE_INTERRUPT"
    default "1" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL1"
    default "2" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL2"
    default "3" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL3"
    default "4" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL4"
    default "5" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL5"
    default "6" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL6"
    default "7" if DRV_OC_INT_PRIORITY_IDX${INSTANCE} = "INT_PRIORITY_LEVEL7"
    
config DRV_OC_ISR_VECTOR_IDX${INSTANCE}
    string
    depends on USE_DRV_OC
    depends on DRV_OC_INTERRUPT_MODE
    default "_OUTPUT_COMPARE_1_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC1"
    default "_OUTPUT_COMPARE_2_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC2"
    default "_OUTPUT_COMPARE_3_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC3"
    default "_OUTPUT_COMPARE_4_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC4"
    default "_OUTPUT_COMPARE_5_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC5"
    default "_OUTPUT_COMPARE_6_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC6"
    default "_OUTPUT_COMPARE_7_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC7"
    default "_OUTPUT_COMPARE_8_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC8"
    default "_OUTPUT_COMPARE_9_VECTOR" if DRV_OC_INTERRUPT_VECTOR_IDX${INSTANCE} = "INT_VECTOR_OC9"
    
ifblock DRV_OC_DRIVER_MODE = "STATIC"

config DRV_OC_COMPARE_MODES_IDX${INSTANCE}
    string "OC Modes"
    depends on USE_DRV_OC
    range OC_COMPARE_MODES
    default "OC_COMPARE_TURN_OFF_MODE"
    ---help---
      IDH_HTML_OC_COMPARE_MODES
    ---endhelp---
    
config DRV_OC_BUFFER_SIZE_IDX${INSTANCE}
    string "OC Buffer Size"
    depends on USE_DRV_OC
    range OC_BUFFER_SIZE
    default "OC_BUFFER_SIZE_16BIT"
    ---help---
      IDH_HTML_OC_BUFFER_SIZE
    ---endhelp---

config DRV_OC_16BIT_TIMERS_IDX${INSTANCE}
    string "OC Timebase Timer Selection"
    depends on USE_DRV_OC
    range OC_16BIT_TIMERS
    default "OC_TIMER_16BIT_TMR2"
    ---help---
      IDH_HTML_OC_16BIT_TIMERS
    ---endhelp---
config USE_DRV_16BIT_TIMER_2_ID${INSTANCE}
    bool
    depends on USE_DRV_OC
    select USE_TMR_MODULE_ID_2_NEEDED_BY_IC_OC
    default y if DRV_OC_16BIT_TIMERS_IDX${INSTANCE} = "OC_TIMER_16BIT_TMR2"
    default n if DRV_OC_16BIT_TIMERS_IDX${INSTANCE} = "OC_TIMER_16BIT_TMR3"	
config USE_DRV_16BIT_TIMER_3_ID${INSTANCE}
    bool
    depends on USE_DRV_OC
    select USE_TMR_MODULE_ID_3_NEEDED_BY_IC_OC
    default y if DRV_OC_16BIT_TIMERS_IDX${INSTANCE} = "OC_TIMER_16BIT_TMR3"	
    default n if DRV_OC_16BIT_TIMERS_IDX${INSTANCE} = "OC_TIMER_16BIT_TMR2"		

config DRV_OC_FAULTS_IDX${INSTANCE}
    string "OC Fault Sources"
    depends on USE_DRV_OC
    range OC_FAULTS
    default "OC_FAULT_DISABLE"
    ---help---
      IDH_HTML_OC_FAULTS
    ---endhelp---
    
config DRV_OC_NONPWM_16BIT_PRI_COMPARE_IDX${INSTANCE}
    int "OC Non PWM Mode 16-bit Compare Value"
    depends on USE_DRV_OC
    range 0 65535
    default 0
    ---help---
      IDH_HTML_PLIB_OC_Buffer16BitSet_OC_MODULE_ID_uint16_t
    ---endhelp---

config DRV_OC_16BIT_PULSE_WIDTH_IDX${INSTANCE}
    int "OC Puse Width"
    depends on USE_DRV_OC
    range 0 65535
    default 10
    ---help---
      IDH_HTML_PLIB_OC_PulseWidth16BitSet_OC_MODULE_ID_uint16_t
    ---endhelp---

config DRV_OC_FAULT_A_USED_IDX${INSTANCE}
    bool
    depends on USE_DRV_OC
    depends on DRV_OC_FAULTS_IDX${INSTANCE} = "OC_FAULT_PRESET"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_1"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_2"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_3"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_4" && PIC32MX
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_7"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_8"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_9"

config DRV_OC_FAULT_B_USED_IDX${INSTANCE}
    bool
    depends on USE_DRV_OC
    depends on DRV_OC_FAULTS_IDX${INSTANCE} = "OC_FAULT_PRESET"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_4" && PIC32MZ
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_5"
    default y if DRV_OC_PERIPHERAL_ID_IDX${INSTANCE} = "OC_ID_6"



endif
endif
