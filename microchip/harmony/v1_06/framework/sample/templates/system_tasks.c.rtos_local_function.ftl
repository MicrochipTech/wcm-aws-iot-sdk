<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE != true && CONFIG_USE_TEST_HARNESS != true>
  <#if CONFIG_SAMPLE_RTOS_DAEMON_IDX0 == "Standalone" && CONFIG_SAMPLE_MODULE_INST_IDX0 == true>

/*******************************************************************************
  Function:
    void _SAMPLE_TasksDaemon0 ( void )

  Summary:
    Maintains state machine of sample module instance 0.
*/

static void _SAMPLE_TasksDaemon0(void)
{
    while(1)
    {
      <#if CONFIG_USE_SAMPLE_FUNC_TEST == true>
        // TODO:  Call test tasks for sample module 0
      <#else>
        SAMPLE_Tasks(sysObj.sampleModule0);
        <@RTOS_TASK_DELAY RTOS_NAME=CONFIG_3RDPARTY_RTOS_USED TASK_DELAY=CONFIG_SAMPLE_RTOS_DELAY_IDX0/>
      </#if>
    }
}

  </#if>
  <#if CONFIG_SAMPLE_RTOS_DAEMON_IDX1 == "Standalone" && CONFIG_SAMPLE_MODULE_INST_IDX1 == true>

/*******************************************************************************
  Function:
    void _SAMPLE_TasksDaemon1 ( void )

  Summary:
    Maintains state machine of sample module instance 1.
*/

static void _SAMPLE_TasksDaemon1(void)
{
    while(1)
    {
      <#if CONFIG_USE_SAMPLE_FUNC_TEST == true>
        // TODO:  Call test tasks for sample module 1
      <#else>
        SAMPLE_Tasks(sysObj.sampleModule1);
        <@RTOS_TASK_DELAY RTOS_NAME=CONFIG_3RDPARTY_RTOS_USED TASK_DELAY=CONFIG_SAMPLE_RTOS_DELAY_IDX1/>
      </#if>
    }
}

  </#if>
</#if><#-- CONFIG_SAMPLE_MODULE_INTERRUPT_MODE != true && CONFIG_USE_TEST_HARNESS != true -->
