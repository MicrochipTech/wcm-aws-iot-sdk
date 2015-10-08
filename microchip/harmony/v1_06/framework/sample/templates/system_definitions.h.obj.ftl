<#if CONFIG_USE_SAMPLE_FUNC_TEST != true>
<#if CONFIG_USE_SAMPLE_MODULE == true && CONFIG_SAMPLE_MODULE_INST_IDX0 == true>
    SYS_MODULE_OBJ  sampleModule0;
</#if>
<#if CONFIG_USE_SAMPLE_MODULE == true && CONFIG_SAMPLE_MODULE_INST_IDX1 == true>
    SYS_MODULE_OBJ  sampleModule1;
</#if>
</#if><#-- CONFIG_USE_SAMPLE_FUNC_TEST != true -->
