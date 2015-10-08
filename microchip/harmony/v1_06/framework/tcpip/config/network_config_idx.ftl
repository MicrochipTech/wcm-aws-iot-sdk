<#--
/*******************************************************************************
  network_config_idx0 Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    network_config_idx0.h.ftl

  Summary:
    network_config_idx0 Freemarker Template File

  Description:

*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->

config TCPIP_STACK_NETWORK_CONFIG_NUMBER_GT_${INSTANCE+1}
    bool
    depends on USE_TCPIP_STACK
<#if INSTANCE != 0>
	default n if TCPIP_STACK_NETWORK_CONFIG_NUMBER_GT_${INSTANCE} = n     
</#if>	
	default n if TCPIP_STACK_NETWORK_CONFIG_NUMBER = ${INSTANCE+1}
	default y

config TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    depends on USE_TCPIP_STACK 
<#if INSTANCE != 0>
	             && TCPIP_STACK_NETWORK_CONFIG_NUMBER_GT_${INSTANCE}
</#if>
    bool "Network Configuration ${INSTANCE}"
    default y

ifblock TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    
config TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE}
    string "Interface"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    range TCPIP_STACK_IF_NAME
    default "PIC32INT" if HAVE_ETH
    default "MRF24W" if USE_DRV_WIFI && DRV_WIFI_DEVICE = "MRF24WG"
    default "MRF24WN" if USE_DRV_WIFI && DRV_WIFI_DEVICE = "MRF24WN"
    default "ENCX24J600" if !HAVE_ETH && !USE_DRV_WIFI
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---

config TCPIP_NETWORK_DEFAULT_INTERFACE_ETH_MAC_NEEDED_IDX${INSTANCE}
    bool
    select USE_PIC32INT_ETH_MAC_NEEDED
    default y if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "PIC32INT"
    default n 
    
config TCPIP_NETWORK_DEFAULT_HOST_NAME_IDX${INSTANCE}
    string "Host Name"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "MCHPBOARD_E" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "PIC32INT"
    default "MCHPBOARD_E" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "ENCX24J600"
    default "MCHPBOARD_W" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24W"
    default "MCHPBOARD_W" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24WN"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---

config TCPIP_NETWORK_DEFAULT_MAC_ADDR_IDX${INSTANCE}
    string "Mac Address"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_IP_ADDRESS_IDX${INSTANCE}
    string "IP Address"
    default "192.168.100.11${INSTANCE+5}" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "PIC32INT"
    default "192.168.100.12${INSTANCE}" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "ENCX24J600"
    default "0.0.0.0" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24W" && (TCPIP_STACK_USE_DHCP_CLIENT || TCPIP_STACK_USE_DHCP_SERVER)
    default "0.0.0.0" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24WN" && (TCPIP_STACK_USE_DHCP_CLIENT || TCPIP_STACK_USE_DHCP_SERVER)
    default "192.168.1.2${INSTANCE}" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24W" && (!TCPIP_STACK_USE_DHCP_CLIENT && !TCPIP_STACK_USE_DHCP_SERVER)
    default "192.168.1.2${INSTANCE}" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24WN" && (!TCPIP_STACK_USE_DHCP_CLIENT && !TCPIP_STACK_USE_DHCP_SERVER)
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_IP_MASK_IDX${INSTANCE}
    string "Network Mask"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "255.255.255.0"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_GATEWAY_IDX${INSTANCE}
    string "Default Gateway"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "192.168.100.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "PIC32INT"
    default "192.168.100.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "ENCX24J600"
    default "192.168.1.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24W"
    default "192.168.1.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24WN"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_DNS_IDX${INSTANCE}
    string "Primary DNS"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "192.168.100.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "PIC32INT"
    default "192.168.100.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "ENCX24J600"
    default "192.168.1.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24W"
    default "192.168.1.1" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24WN"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_SECOND_DNS_IDX${INSTANCE}
    string "Secondary DNS"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "0.0.0.0"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_POWER_MODE_IDX${INSTANCE}
    string "Power Mode"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "full"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_INTERFACE_FLAGS_IDX${INSTANCE}
    string "Network Interface Flags"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "TCPIP_NETWORK_CONFIG_DHCP_CLIENT_ON"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_MAC_DRIVER_IDX${INSTANCE}
    string "Network MAC Driver"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default "DRV_ETHMAC_PIC32MACObject" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "PIC32INT"
    default "DRV_ENCX24J600_MACObject" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "ENCX24J600"
    default "DRV_MRF24W_MACObject" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24W"
    default "WDRV_MRF24WN_MACObject" if TCPIP_NETWORK_DEFAULT_INTERFACE_NAME_IDX${INSTANCE} = "MRF24WN"
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---

config TCPIP_NETWORK_DEFAULT_IPV6_ADDRESS_IDX${INSTANCE}
    string "Default IPv6 Address"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---

config TCPIP_NETWORK_DEFAULT_IPV6_PREFIX_LENGTH_IDX${INSTANCE}
    int "Default IPv6 Prefix Length"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    default 0
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---
    
config TCPIP_NETWORK_DEFAULT_IPV6_GATEWAY_IDX${INSTANCE}
    string "Default IPv6 Gateway"
    depends on TCPIP_STACK_NETWORK_CONFIG_IDX${INSTANCE}
    ---help---
    IDH_HTML_TCPIP_NETWORK_CONFIG
    ---endhelp---

endif
