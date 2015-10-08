<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE != true && CONFIG_USE_TEST_HARNESS != true>
  <#if CONFIG_SAMPLE_RTOS_DAEMON_IDX0 == "Standalone" && CONFIG_SAMPLE_MODULE_INST_IDX0 == true>
static void _SAMPLE_TasksDaemon0 ( void );
  </#if>
  <#if CONFIG_SAMPLE_RTOS_DAEMON_IDX1 == "Standalone" && CONFIG_SAMPLE_MODULE_INST_IDX1 == true>
static void _SAMPLE_TasksDaemon1 ( void );
  </#if>
</#if>
