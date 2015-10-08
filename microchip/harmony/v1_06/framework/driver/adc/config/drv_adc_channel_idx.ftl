config DRV_ADC_CHANNEL_INSTANCES_NUMBER_GT_${INSTANCE+1}
    bool
    depends on USE_DRV_ADC
<#if INSTANCE != 0>
	default n if DRV_ADC_CHANNEL_INSTANCES_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if DRV_ADC_CHANNEL_INSTANCES_NUMBER = ${INSTANCE+1}
	default y
	
config DRV_ADC_CHANNEL_INST_IDX${INSTANCE}
    depends on USE_DRV_ADC 
<#if INSTANCE != 0>
	             && DRV_ADC_CHANNEL_INSTANCES_NUMBER_GT_${INSTANCE}
</#if>
    bool "ADC Analog Channel Instance ${INSTANCE}"
    default y

ifblock DRV_ADC_CHANNEL_INST_IDX${INSTANCE}

choice

    prompt "Select Type"
    depends on USE_DRV_ADC	
    default DRV_ADC_TYPE_DEDICATED_IDX${INSTANCE}

config DRV_ADC_TYPE_DEDICATED_IDX${INSTANCE}
    bool "Dedicated Channel"
    depends on USE_DRV_ADC	
	
config DRV_ADC_TYPE_SCAN_IDX${INSTANCE}
    bool "Scan Channel"
    depends on USE_DRV_ADC
	depends on DRV_ADC_SCAN_MODE

config DRV_ADC_TYPE_ESCAN_IDX${INSTANCE}
    depends on USE_DRV_ADC
    depends on DRV_ADC_SCAN_MODE
    depends on HAVE_ADC
    depends on HAVE_ADC_ESCAN
    bool "Extended Scan Channel"

endchoice

choice 

    prompt "Select Mux"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADC
	depends on DRV_ADC_TYPE_DEDICATED_IDX${INSTANCE}
	depends on DRV_ADC_ALTS_MODE = "ADC_SAMPLING_MODE_ALTERNATE_INPUT"
    default DRV_ADC_SEL_MUXA_IDX${INSTANCE}	
	
config DRV_ADC_SEL_MUXA_IDX${INSTANCE}
    bool "MUX A Input"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADC
    depends on DRV_ADC_TYPE_DEDICATED_IDX${INSTANCE}

config DRV_ADC_SEL_MUXB_IDX${INSTANCE}
    bool "MUX B Input"
    depends on USE_DRV_ADC
	depends on DRV_ADC_TYPE_ADC
    depends on DRV_ADC_TYPE_DEDICATED_IDX${INSTANCE}

endchoice

config DRV_ADC_POSITIVE_CHANNEL_NUMBER_IDX${INSTANCE}
    string "Select Dedicated Analog Channel"
    range ADC_INPUTS_POSITIVE
    depends on USE_DRV_ADC
    depends on HAVE_ADC
    depends on DRV_ADC_TYPE_DEDICATED_IDX${INSTANCE}
    default "ADC_INPUT_POSITIVE_AN0"

config DRV_ADC_SCAN_CHANNEL_NUMBER_IDX${INSTANCE}
    string "Select Analog Channel to Scan"
    range ADC_INPUTS_SCAN
    depends on USE_DRV_ADC
    depends on HAVE_ADC
    depends on DRV_ADC_TYPE_SCAN_IDX${INSTANCE}
    default "ADC_INPUT_SCAN_AN0"

config DRV_ADC_ESCAN_CHANNEL_NUMBER_IDX${INSTANCE}
    string "Select Analog Channel to Scan"
    range ADC_INPUTS_SCAN_EXTENDED
    depends on USE_DRV_ADC
    depends on HAVE_ADC
    depends on DRV_ADC_TYPE_ESCAN_IDX${INSTANCE}
    default "ADC_INPUT_SCAN_AN32"

config DRV_ADC_CHANNEL_NUMBER_IDX${INSTANCE}
    string "Select Analog Channel"
    range ADCP_INPUT_ID
    depends on USE_DRV_ADC
    depends on HAVE_ADCP
    default "ADCP_AN0"

endif


	
