<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE != true>
  <#if CONFIG_USE_SAMPLE_FUNC_TEST != true>
    <#if CONFIG_SAMPLE_RTOS_DAEMON_IDX0 != "Standalone" && CONFIG_SAMPLE_MODULE_INST_IDX0 == true>
        SAMPLE_Tasks(sysObj.sampleModule0);
    </#if>
    <#if CONFIG_SAMPLE_RTOS_DAEMON_IDX1 != "Standalone" && CONFIG_SAMPLE_MODULE_INST_IDX1 == true>
        SAMPLE_Tasks(sysObj.sampleModule1);
    </#if>
  </#if>
</#if>
