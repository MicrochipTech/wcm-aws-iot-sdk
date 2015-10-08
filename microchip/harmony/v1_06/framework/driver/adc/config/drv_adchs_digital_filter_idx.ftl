config DRV_ADCHS_DIGITAL_FILTER_INSTANCES_NUMBER_GT_${INSTANCE+1}
    depends on USE_DRV_ADC
    bool
<#if INSTANCE != 0>
	default n if DRV_ADCHS_DIGITAL_FILTER_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_ADCHS_DIGITAL_FILTER_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_ADCHS_DIGITAL_FILTER_INST_IDX${INSTANCE}
    depends on USE_DRV_ADC 
<#if INSTANCE != 0>
	             && DRV_ADCHS_DIGITAL_FILTER_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "Digital Filter Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_ADCHS_DIGITAL_FILTER_INSTANCES_NUMBER
    ---endhelp---

ifblock DRV_ADCHS_DIGITAL_FILTER_INST_IDX${INSTANCE}

config DRV_ADCHS_DIGITAL_FILTER_ID_IDX${INSTANCE}
    string "Digital Filter Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_DIGITAL_FILTER_ID
    default "ADCHS_DIGITAL_FILTER_1"
	---help---
	IDH_HTML_ADCHS_DIGITAL_FILTER_ID
	---endhelp---

config DRV_ADCHS_DIGITAL_FILTER_ANALOG_INPUT_ID_IDX${INSTANCE}
    string "Analog Input to be processed by Digital Filter"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_AN_INPUT_ID
    default "ADCHS_AN0"
	---help---
	IDH_HTML_ADCHS_DIGITAL_FILTER_ANALOG_INPUT_ID
	---endhelp---

config DRV_ADCHS_DIGITAL_FILTER_MODE_IDX${INSTANCE}
    bool "Check for Oversampling-mode; Un-Check for Averaging mode"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_DIGITAL_FILTER_MODE
	---endhelp---

ifblock DRV_ADCHS_DIGITAL_FILTER_MODE_IDX${INSTANCE} = y
config DRV_ADCHS_DIGITAL_FILTER_OVERSAMPLING_RATIO_IDX${INSTANCE}
    string "Oversampling Ratio"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_DIGITAL_FILTER_OVERSAMPLE_RATIO
    default "ADCHS_DIGITAL_FILTER_OVERSAMPLE_RATIO_16X"
	---help---
	IDH_HTML_ADCHS_DIGITAL_FILTER_ANALOG_INPUT_ID
	---endhelp---
endif

ifblock DRV_ADCHS_DIGITAL_FILTER_MODE_IDX${INSTANCE} = n
config DRV_ADCHS_DIGITAL_FILTER_AVERAGING_COUNT_IDX${INSTANCE}
    string "Averaging Sample Count"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_DIGITAL_FILTER_AVERAGE_SAMPLE_COUNT
    default "ADCHS_DIGITAL_FILTER_AVERAGE_SAMPLE_COUNT_16"
	---help---
	IDH_HTML_ADCHS_DIGITAL_FILTER_AVERAGING_COUNT
	---endhelp---
endif

config DRV_ADCHS_DIGITAL_FILTER_SIG_BITS_IDX${INSTANCE}
    string "Significant bits"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_DIGITAL_FILTER_SIGNIFICANT_BITS
	default "ADCHS_DIGITAL_FILTER_SIGNIFICANT_ALL_16BITS"
	---help---
	IDH_HTML_ADCHS_DIGITAL_FILTER_SIG_BITS
	---endhelp---
	
endif

	
