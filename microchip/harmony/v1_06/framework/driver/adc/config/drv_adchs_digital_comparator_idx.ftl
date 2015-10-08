config DRV_ADCHS_DIGITAL_COMPARATOR_INSTANCES_NUMBER_GT_${INSTANCE+1}
    depends on USE_DRV_ADC
    bool
<#if INSTANCE != 0>
	default n if DRV_ADCHS_DIGITAL_COMPARATOR_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_ADCHS_DIGITAL_COMPARATOR_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX${INSTANCE}
    depends on USE_DRV_ADC 
<#if INSTANCE != 0>
	             && DRV_ADCHS_DIGITAL_COMPARATOR_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "Digital Comparator Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_ADCHS_DIGITAL_COMPARATOR_INSTANCES_NUMBER
    ---endhelp---

ifblock DRV_ADCHS_DIGITAL_COMPARATOR_INST_IDX${INSTANCE}

config DRV_ADCHS_DIGITAL_COMPARATOR_ID_IDX${INSTANCE}
    string "Digital Comparator Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_DIGITAL_COMPARATOR_ID
    default "ADCHS_DIGITAL_COMPARATOR_1"
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_ID
	---endhelp---

config DRV_ADCHS_DIGITAL_COMPARATOR_ANALOG_INPUT_ID_IDX${INSTANCE}
    string "Analog Input to be processed by Digital Comparator"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_AN_INPUT_ID
    default "ADCHS_AN0"
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_ANALOG_INPUT_ID
	---endhelp---

config DRV_ADCHS_DIGITAL_COMPARATOR_HIGH_LIMIT_IDX${INSTANCE}
    int "High Limit Value"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default 0
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_HIGH_LIMIT
	---endhelp---

config DRV_ADCHS_DIGITAL_COMPARATOR_LOW_LIMIT_IDX${INSTANCE}
    int "Low Limit Value"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default 0
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_LOW_LIMIT
	---endhelp---

config DRV_ADCHS_DIGITAL_COMPARATOR_BTWN_EVENT_IDX${INSTANCE}
    bool "Digital Comparator event is generated when ADC result is between High-Limit and Low-Limit"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_BTWN_EVENT
	---endhelp---
	
config DRV_ADCHS_DIGITAL_COMPARATOR_HI_HI_EVENT_IDX${INSTANCE}
    bool "Digital Comparator event is generated when ADC result is higher than or equal to High-Limit"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_HI_HI_EVENT
	---endhelp---

config DRV_ADCHS_DIGITAL_COMPARATOR_HI_LO_EVENT_IDX${INSTANCE}
    bool "Digital Comparator event is generated when ADC result is lower than High-Limit"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_HI_LO_EVENT
	---endhelp---
	
config DRV_ADCHS_DIGITAL_COMPARATOR_LO_HI_EVENT_IDX${INSTANCE}
    bool "Digital Comparator event is generated when ADC result is higher or equal to Low-Limit"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_LO_HI_EVENT
	---endhelp---

config DRV_ADCHS_DIGITAL_COMPARATOR_LO_LO_EVENT_IDX${INSTANCE}
    bool "Digital Comparator event is generated when ADC result is lower than to Low-Limit"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_DIGITAL_COMPARATOR_LO_LO_EVENT
	---endhelp---
	
endif

	
