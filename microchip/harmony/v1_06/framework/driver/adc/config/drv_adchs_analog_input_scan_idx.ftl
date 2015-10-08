config DRV_ADCHS_ANALOG_INPUT_SCAN_INSTANCES_NUMBER_GT_${INSTANCE+1}
    depends on USE_DRV_ADC
    bool
<#if INSTANCE != 0>
	default n if DRV_ADCHS_ANALOG_INPUT_SCAN_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_ADCHS_ANALOG_INPUT_SCAN_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_ADCHS_ANALOG_INPUT_SCAN_INST_IDX${INSTANCE}
    depends on USE_DRV_ADC 
<#if INSTANCE != 0>
	             && DRV_ADCHS_ANALOG_INPUT_SCAN_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "Analog Input Scan Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_ADCHS_ANALOG_INPUT_SCAN_INSTANCES_NUMBER
    ---endhelp---

ifblock DRV_ADCHS_ANALOG_INPUT_SCAN_INST_IDX${INSTANCE}

comment "**** For Class-1 or Class-2 analog inputs, additionally, select respective trigger as ADCHS_TRIGGER_SOURCE_SCAN ****"
	depends on DRV_ADC_TYPE_ADCHS
	
config DRV_ADCHS_ANALOG_INPUT_ID_IDX${INSTANCE}
    string "Analog Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_AN_INPUT_ID
    default "ADCHS_AN12"
	---help---
	IDH_HTML_ADCHS_AN_INPUT_ID
	---endhelp---

config DRV_ADCHS_SCAN_ANALOG_INPUT_MODE_IDX${INSTANCE}
    string "Analog Input Mode"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_INPUT_MODE
    default "ADCHS_INPUT_MODE_SINGLE_ENDED_UNIPOLAR"
	---help---
	IDH_HTML_ADCHS_INPUT_MODE
	---endhelp---

endif

	
