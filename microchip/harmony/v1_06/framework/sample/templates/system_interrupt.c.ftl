<#if CONFIG_SAMPLE_MODULE_INTERRUPT_MODE == true && CONFIG_USE_SAMPLE_FUNC_TEST != true>
<#-- Determine IPL value -->
<#if     CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL1">
    <#assign sample0ipl="IPL1AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL2">
    <#assign sample0ipl="IPL2AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL3">
    <#assign sample0ipl="IPL3AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL4">
    <#assign sample0ipl="IPL4AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL5">
    <#assign sample0ipl="IPL5AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL6">
    <#assign sample0ipl="IPL6AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX0 == "INT_PRIORITY_LEVEL7">
    <#assign sample0ipl="IPL7AUTO">
<#else>
    <#assign sample0ipl="#error invalid IPL">
</#if>
<#if     CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL1">
    <#assign sample1ipl="IPL1AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL2">
    <#assign sample1ipl="IPL2AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL3">
    <#assign sample1ipl="IPL3AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL4">
    <#assign sample1ipl="IPL4AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL5">
    <#assign sample1ipl="IPL5AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL6">
    <#assign sample1ipl="IPL6AUTO">
<#elseif CONFIG_SAMPLE_MODULE_INTERRUPT_PRIORITY_IDX1 == "INT_PRIORITY_LEVEL7">
    <#assign sample1ipl="IPL7AUTO">
<#else>
    <#assign sample0ipl="#error invalid IPL">
</#if>
<#if CONFIG_SAMPLE_MODULE_INST_IDX0 == true>

/* Sample module instance 0 using timer ${CONFIG_SAMPLE_MODULE_TIMER_ID_IDX0} vector.*/
void __ISR(${CONFIG_SAMPLE_MODULE_TIMER_ISR_VECTOR_IDX0}, ${sample0ipl}) _SampleModule0TimerISR ( void )
{

    SAMPLE_Tasks(sysObj.sampleModule0);
    PLIB_INT_SourceFlagClear(INT_ID_0, ${CONFIG_SAMPLE_MODULE_INTERRUPT_SOURCE_IDX0});
}
</#if>
<#if CONFIG_SAMPLE_MODULE_INST_IDX1 == true>

/* Sample module instance 1 using timer ${CONFIG_SAMPLE_MODULE_TIMER_ID_IDX1} vector.*/
void __ISR(${CONFIG_SAMPLE_MODULE_TIMER_ISR_VECTOR_IDX1}, ${sample1ipl}) _SampleModule1TimerISR ( void )
{
    SAMPLE_Tasks(sysObj.sampleModule1);
    PLIB_INT_SourceFlagClear(INT_ID_0, ${CONFIG_SAMPLE_MODULE_INTERRUPT_SOURCE_IDX1});
}
</#if>
</#if><#-- CONFIG_DRV_TMR_INTERRUPT_MODE == true && CONFIG_USE_SAMPLE_FUNC_TEST != true -->
