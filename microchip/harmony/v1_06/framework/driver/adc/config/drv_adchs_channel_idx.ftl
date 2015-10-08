config DRV_ADCHS_CHANNEL_NUMBER_GT_${INSTANCE+1}
    depends on USE_DRV_ADC
    bool
<#if INSTANCE != 0>
	default n if DRV_ADCHS_CHANNEL_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_ADCHS_CHANNEL_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_ADCHS_CHANNEL_INST_IDX${INSTANCE}
    depends on USE_DRV_ADC 
<#if INSTANCE != 0>
	             && DRV_ADCHS_CHANNEL_NUMBER_GT_${INSTANCE}
</#if>
    bool "Channel Instance ${INSTANCE}"
    default y
    ---help---
    IDH_HTML_DRV_ADCHS_CHANNEL_NUMBER
    ---endhelp---

ifblock DRV_ADCHS_CHANNEL_INST_IDX${INSTANCE}

config DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE}
    string "Channel Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_ID
    default "ADCHS_CHANNEL_0"
	---help---
	IDH_HTML_ADCHS_CHANNEL_ID
	---endhelp---

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_0"
config DRV_ADCHS_CHNL_0_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-0 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_0_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN0"
	---help---
	IDH_HTML_ADCHS_CHNL_0_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_1"
config DRV_ADCHS_CHNL_1_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-1 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_1_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN1"
	---help---
	IDH_HTML_ADCHS_CHNL_1_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_2"
config DRV_ADCHS_CHNL_2_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-2 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_2_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN2"
	---help---
	IDH_HTML_ADCHS_CHNL_2_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_3"
config DRV_ADCHS_CHNL_3_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-3 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_3_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN3"
	---help---
	IDH_HTML_ADCHS_CHNL_3_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_4"
config DRV_ADCHS_CHNL_4_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-4 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_4_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN4"
	---help---
	IDH_HTML_ADCHS_CHNL_4_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_5"
config DRV_ADCHS_CHNL_5_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-5 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_5_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN5"
	---help---
	IDH_HTML_ADCHS_CHNL_5_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} = "ADCHS_CHANNEL_6"
config DRV_ADCHS_CHNL_6_ALT_INP_SEL_IDX${INSTANCE}
    string "Channel-6 Analog (class-1) default or alternate Input Selection"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_6_INP_SEL
    default "ADCHS_DEFAULT_CLASS1_AN6"
	---help---
	IDH_HTML_ADCHS_CHNL_6_ALT_INP_SEL
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} != "ADCHS_CHANNEL_7"
config DRV_ADCHS_ANALOG_INPUT_LEVEL_TRIGGER_IDX${INSTANCE}
    bool "Check for Level trigger; Un-check for edge trigger, for selected analog input?"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	default n
	---help---
	IDH_HTML_ADCHS_ANALOG_INPUT_TRIGGER_MODE
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} != "ADCHS_CHANNEL_7"
config DRV_ADCHS_TRIGGER_SOURCE_IDX${INSTANCE}
    string "Trigger source"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_TRIGGER_SOURCE
    default "ADCHS_TRIGGER_SOURCE_GLOBAL_SOFTWARE_EDGE"
	---help---
	IDH_HTML_ADCHS_TRIGGER_SOURCE
	---endhelp---
endif

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} != "ADCHS_CHANNEL_7"
config DRV_ADCHS_INPUT_MODE_IDX${INSTANCE}
    string "Channel Analog Input Mode"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_INPUT_MODE
    default "ADCHS_INPUT_MODE_SINGLE_ENDED_UNIPOLAR"
	---help---
	IDH_HTML_ADCHS_INPUT_MODE
	---endhelp---
endif

config DRV_ADCHS_DATA_RESOLUTION_SELECT_IDX${INSTANCE}
    string "Output Data Resolution"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_DATA_RESOLUTION
    default "ADCHS_DATA_RESOLUTION_12BIT"
	---help---
	IDH_HTML_ADCHS_DATA_RESOLUTION
	---endhelp---

config DRV_ADCHS_CHANNEL_CLOCK_DIVIDER_IDX${INSTANCE}
    int "Clock Divider (nTADC) "
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range 0 127
    default 1
	---help---
	IDH_HTML_ADCHS_CHANNEL_CLOCK_DIVIDER
	---endhelp---

config DRV_ADCHS_SAMPLE_TIME_COUNT_IDX${INSTANCE}
    int "Sample Time Count (nSAMC)"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range 0 1023
    default 1
	---help---
	IDH_HTML_ADCHS_SAMPLE_TIME_COUNT
	---endhelp---

config DRV_ADCHS_EARLY_INTERRUPT_PRIOR_CLOCK_IDX${INSTANCE}
    string "Early Interrupt Clock Count"
    depends on USE_DRV_ADC && DRV_ADCHS_INTERRUPT_MODE
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_EARLY_INTERRUPT_PRIOR_CLOCK
    default "ADCHS_EARLY_INTERRUPT_PRIOR_CLOCK_4"
	---help---
	IDH_HTML_ADCHS_EARLY_INTERRUPT_PRIOR_CLOCK
	---endhelp---

ifblock DRV_ADCHS_CHANNEL_ID_IDX${INSTANCE} != "ADCHS_CHANNEL_7"
config DRV_ADCHS_TRIGGER_SAMPLE_SELECT_IDX${INSTANCE}
    string "Trigger sample select"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADCHS
	range ADCHS_CHANNEL_TRIGGER_SAMPLING_SEL
    default "ADCHS_CHANNEL_UNSYNC_TRIGGER_UNSYNC_SAMPLING"
	---help---
	IDH_HTML_ADCHS_TRIGGER_SAMPLE_SELECT
	---endhelp---
	
endif

endif

	
