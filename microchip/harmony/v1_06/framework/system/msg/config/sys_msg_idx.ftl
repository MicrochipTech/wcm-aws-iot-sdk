config SYS_MSG_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on USE_SYS_MSG
<#if INSTANCE != 0>
	default n if SYS_MSG_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if SYS_MSG_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config SYS_MSG_INST_IDX${INSTANCE}
    depends on USE_SYS_MSG 
<#if INSTANCE != 0>
	             && SYS_MSG_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "System Message Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_SYS_MSG_INSTANCES_NUMBER
    ---endhelp---

ifblock SYS_MSG_INST_IDX${INSTANCE}

menu "RTOS Configuration (Instance ${INSTANCE})"
    depends on USE_SYS_MSG
    depends on USE_3RDPARTY_RTOS

config SYS_MSG_RTOS_IDX${INSTANCE}
    string "Run This Instance As"
    range 3RDPARTY_RTOS_SYS_TASKS_OPTIONS
    default "Combined with System Tasks"

config SYS_MSG_IDX${INSTANCE}_RTOS_TASK_SIZE
    int "Task Size"
    depends on SYS_MSG_RTOS_IDX${INSTANCE} = "Standalone"
    default 1024

config SYS_MSG_IDX${INSTANCE}_RTOS_TASK_PRIORITY
    int "Task Priority"
    depends on SYS_MSG_RTOS_IDX${INSTANCE} = "Standalone"
    default 1

config SYS_MSG_IDX${INSTANCE}_RTOS_USE_DELAY
    bool "Use Task Delay?"
    depends on SYS_MSG_RTOS_IDX${INSTANCE} = "Standalone"
    default y

config SYS_MSG_IDX${INSTANCE}_RTOS_DELAY
    int "Task Delay"
    depends on SYS_MSG_RTOS_IDX${INSTANCE} = "Standalone"
    depends on SYS_MSG_IDX${INSTANCE}_RTOS_USE_DELAY
    default 1000
endmenu

endif



	
