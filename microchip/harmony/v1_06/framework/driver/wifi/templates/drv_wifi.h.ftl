<#--
/*******************************************************************************
  Wi-Fi Driver Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    drv_wifi.h.ftl

  Summary:
    Wi-Fi Driver Freemarker Template File

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

/*** Wi-Fi Driver Configuration ***/
<#if CONFIG_DRV_WIFI_DEVICE == "MRF24WG">
#define WF_CONFIG_MHC
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
#define WIFI_USE_RTOS
</#if><#-- CONFIG_3RDPARTY_RTOS_USED -->

#define DRV_WIFI_ASSERT(condition, msg) WF_AssertFunction(condition, msg, __FILE__, __LINE__)

<#if CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '0'>
#define DRV_WIFI_SPI_INDEX 0
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx0
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '1'>
#define DRV_WIFI_SPI_INDEX 1
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx1
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '2'>
#define DRV_WIFI_SPI_INDEX 2
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx2
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '3'>
#define DRV_WIFI_SPI_INDEX 3
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx3
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '4'>
#define DRV_WIFI_SPI_INDEX 4
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx4
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '5'>
#define DRV_WIFI_SPI_INDEX 5
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx5
</#if>

<#if CONFIG_DRV_WIFI_STORE_CONFIG_IN_NVM>
#define DRV_WIFI_NVM_SPACE_ENABLE
#define DRV_WIFI_NVM_SPACE_ADDR (${CONFIG_DRV_WIFI_NVM_START_ADDRESS}*1024)
</#if>

<#if CONFIG_BSP_PIC32MX795_PIM_E16 == true>
#define EX16

#define MRF_INT_SOURCE INT_SOURCE_EXTERNAL_1
#define MRF_INT_VECTOR INT_VECTOR_INT1

// IO mapping for general control pins, including CS, RESET and HIBERNATE
// MRF24W in SPI 1 slot
#define WF_CS_PORT_CHANNEL PORT_CHANNEL_D
#define WF_CS_BIT_POS      9

#define WF_RESET_PORT_CHANNEL PORT_CHANNEL_F
#define WF_RESET_BIT_POS      0

#define WF_HIBERNATE_PORT_CHANNEL PORT_CHANNEL_F
#define WF_HIBERNATE_BIT_POS      1

#define WF_INT_PRIORITY     3
#define WF_INT_SUBPRIORITY  1
#define WF_INT_PORT_CHANNEL PORT_CHANNEL_E
#define WF_INT_BIT_POS      8
<#elseif CONFIG_BSP_PIC32MX_ETH_SK == true>
#define MRF24W_USE_CN_INT

#define MRF_INT_SOURCE INT_SOURCE_CHANGE_NOTICE
#define MRF_INT_VECTOR INT_VECTOR_CN

// IO mapping for general control pins, including CS, RESET and HIBERNATE
// MRF24W in SPI 1 slot
#define WF_CS_PORT_CHANNEL PORT_CHANNEL_B
#define WF_CS_BIT_POS      2

#define WF_RESET_PORT_CHANNEL PORT_CHANNEL_F
#define WF_RESET_BIT_POS      0

#define WF_HIBERNATE_PORT_CHANNEL PORT_CHANNEL_F
#define WF_HIBERNATE_BIT_POS      1

#define WF_INT_PRIORITY     3
#define WF_INT_SUBPRIORITY  1
#define WF_INT_PORT_CHANNEL PORT_CHANNEL_G
#define WF_INT_BIT_POS      7

// For CN9(GPIO_G7) as extern interrupt, it is better to use another GPIO (GPIO_E0) to read interrupt high or low. 
// We need a jump line to connect GPIO_E0 to GPIO_G7. 
//#define WF_VERIFY_EINT_BY_ANOTHER_GPIO
#if defined(WF_VERIFY_EINT_BY_ANOTHER_GPIO)
// Use GPIO_E0. Please use a jump line to connect GPIO_E0 to GPIO_G7. 
#define WF_INT_PORT_CHANNEL_READ PORT_CHANNEL_E
#define WF_INT_BIT_POS_READ      0
#else
// Still read GPIO_G7. 
#define WF_INT_PORT_CHANNEL_READ PORT_CHANNEL_G
#define WF_INT_BIT_POS_READ      7
#endif
<#elseif CONFIG_BSP_PIC32MZ_EC_SK == true>
#define MRF_INT_SOURCE INT_SOURCE_EXTERNAL_1
#define MRF_INT_VECTOR INT_VECTOR_INT1

// IO mapping for general control pins, including CS, RESET and HIBERNATE
// MRF24W in SPI 1 slot
#define WF_CS_PORT_CHANNEL PORT_CHANNEL_E
#define WF_CS_BIT_POS      9

#define WF_RESET_PORT_CHANNEL PORT_CHANNEL_F
#define WF_RESET_BIT_POS      0

#define WF_HIBERNATE_PORT_CHANNEL PORT_CHANNEL_F
#define WF_HIBERNATE_BIT_POS      1

#define WF_INT_PRIORITY     3
#define WF_INT_SUBPRIORITY  1
#define WF_INT_PORT_CHANNEL PORT_CHANNEL_E
#define WF_INT_BIT_POS      8
<#elseif CONFIG_BSP_PIC32MZ_EC_SK_MEB2 == true>
#define MRF_INT_SOURCE INT_SOURCE_EXTERNAL_0
#define MRF_INT_VECTOR INT_VECTOR_INT0

// IO mapping for general control pins, including CS, RESET and HIBERNATE
// MRF24W in SPI 4 slot
#define WF_CS_PORT_CHANNEL PORT_CHANNEL_E
#define WF_CS_BIT_POS      9

#define WF_RESET_PORT_CHANNEL PORT_CHANNEL_J
#define WF_RESET_BIT_POS      0

#define WF_HIBERNATE_PORT_CHANNEL PORT_CHANNEL_H
#define WF_HIBERNATE_BIT_POS      14

#define WF_INT_PRIORITY     3
#define WF_INT_SUBPRIORITY  1
#define WF_INT_PORT_CHANNEL PORT_CHANNEL_D
#define WF_INT_BIT_POS      0
<#elseif CONFIG_BSP_WIFI_G_DB == true>
#define MRF_INT_SOURCE INT_SOURCE_EXTERNAL_1
#define MRF_INT_VECTOR INT_VECTOR_INT1

// IO mapping for general control pins, including CS, RESET and HIBERNATE
// MRF24W in SPI2 slot
#define WF_CS_PORT_CHANNEL PORT_CHANNEL_G
#define WF_CS_BIT_POS      9

#define WF_RESET_PORT_CHANNEL PORT_CHANNEL_D
#define WF_RESET_BIT_POS      1

#define WF_HIBERNATE_PORT_CHANNEL PORT_CHANNEL_E
#define WF_HIBERNATE_BIT_POS      4

#define WF_INT_PRIORITY     3
#define WF_INT_SUBPRIORITY  1
#define WF_INT_PORT_CHANNEL PORT_CHANNEL_D
#define WF_INT_BIT_POS      8
<#else>
#error "You are using a hardware configuration that doesn't have out-of-box Wi-Fi driver support. Please check the datasheet and configure the following Pins to make the Wi-Fi driver work."
//#define MRF_INT_SOURCE
//#define MRF_INT_VECTOR

//#define WF_CS_PORT_CHANNEL
//#define WF_CS_BIT_POS

//#define WF_RESET_PORT_CHANNEL
//#define WF_RESET_BIT_POS

//#define WF_HIBERNATE_PORT_CHANNEL
//#define WF_HIBERNATE_BIT_POS

//#define WF_INT_PRIORITY
//#define WF_INT_SUBPRIORITY
//#define WF_INT_PORT_CHANNEL
//#define WF_INT_BIT_POS
</#if><#-- CONFIG_BSP -->

<#if CONFIG_DRV_WIFI_NETWORK_TYPE == "Infrastructure">
#define WF_DEFAULT_NETWORK_TYPE        DRV_WIFI_NETWORK_TYPE_INFRASTRUCTURE
<#if CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WPS Push Button">
#define WF_DEFAULT_SSID_NAME           ""
<#else>
#define WF_DEFAULT_SSID_NAME           "${CONFIG_DRV_WIFI_SSID_NAME}"
</#if>
#define WF_DEFAULT_LIST_RETRY_COUNT    (DRV_WIFI_RETRY_FOREVER) /* Number (1..255) of times to try to connect to the SSID when using Infrastructure network type */
#define WF_DEFAULT_CHANNEL_LIST        {} /* Channel list for Domain - use default in module */

<#if CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "Open">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_OPEN
#define WF_DEFAULT_WEP_PHRASE          "WEP Phrase" // default WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WEP-40 (64-bit)">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WEP_40
#define WF_DEFAULT_WEP_PHRASE          "${CONFIG_DRV_WIFI_WEP_PASS_PHRASE}" // customized WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "${CONFIG_DRV_WIFI_WEP_KEY_40}" // customized WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WEP-104 (128-bit)">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WEP_104
#define WF_DEFAULT_WEP_PHRASE          "${CONFIG_DRV_WIFI_WEP_PASS_PHRASE}" // customized WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "${CONFIG_DRV_WIFI_WEP_KEY_104}" // customized WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WPA Auto Personal">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WPA_AUTO_WITH_PASS_PHRASE
#define WF_DEFAULT_WEP_PHRASE          "WEP Phrase" // default WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "${CONFIG_DRV_WIFI_WPA_PASS_PHRASE}" // customized WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WPS Push Button">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WPS_PUSH_BUTTON
#define WF_DEFAULT_WEP_PHRASE          "WEP Phrase" // default WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WPS PIN">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WPS_PIN
#define WF_DEFAULT_WEP_PHRASE          "WEP Phrase" // default WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "${CONFIG_DRV_WIFI_WPS_PIN}" // customized WPS PIN
</#if><#-- CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE -->

<#if CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WPS Push Button" || CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE == "WPS PIN">
#define WF_SAVE_WPS_CREDENTIALS        DRV_WIFI_ENABLED
<#else>
#define WF_SAVE_WPS_CREDENTIALS        DRV_WIFI_DISABLED
</#if><#-- CONFIG_DRV_WIFI_SECURITY_MODE_INFRASTRUCTURE -->

#define WF_CHECK_LINK_STATUS           WF_DISABLED /* Gets the MRF to check the link status relying on Tx failures. */
#define WF_LINK_LOST_THRESHOLD         40          /* Consecutive Tx transmission failures to be considered the AP is gone away. */

/* 
 * MRF24W FW has a built-in connection manager, and it is enabled by default.
 * If you want to run your own connection manager in host side, you should
 * disable the FW connection manager to avoid possible conflict between the two.
 * Especially these two APIs can be affected if you do not disable it.
 * A) uint16_t DRV_WIFI_Disconnect(void)
 * B) uint16_t DRV_WIFI_Scan(bool scanAll)
 * These APIs will return failure when the conflict occurs.
 */
#define WF_MODULE_CONNECTION_MANAGER   DRV_WIFI_ENABLED

#define WF_DEFAULT_PS_POLL             DRV_WIFI_DISABLED /* DRV_WIFI_ENABLED or DRV_WIFI_DISABLED */
#define WF_SOFTWARE_MULTICAST_FILTER   DRV_WIFI_ENABLED

<#if CONFIG_DRV_WIFI_FIRMWARE_UPDATE_METHOD == "OTA">
#define WF_UPDATE_FIRMWARE_OTA
<#elseif CONFIG_DRV_WIFI_FIRMWARE_UPDATE_METHOD == "XMODEM">
#define WF_UPDATE_FIRMWARE_XMODEM
</#if><#-- CONFIG_DRV_WIFI_FIRMWARE_UPDATE_METHOD -->
<#elseif CONFIG_DRV_WIFI_NETWORK_TYPE == "Ad-Hoc">
#define WF_DEFAULT_NETWORK_TYPE        DRV_WIFI_NETWORK_TYPE_ADHOC
#define WF_DEFAULT_SSID_NAME           "${CONFIG_DRV_WIFI_SSID_NAME}"
#define WF_DEFAULT_LIST_RETRY_COUNT    (DRV_WIFI_RETRY_ADHOC) /* Number (1..254) of times to try to connect to the SSID when using Ad/Hoc network type */
#define WF_DEFAULT_CHANNEL_LIST        {}                     /* Ad-hoc network channel - use default channel for Domain in module */

<#if CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP == "Open">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_OPEN
#define WF_DEFAULT_WEP_PHRASE          "WEP Phrase" // default WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP == "WEP-40 (64-bit)">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WEP_40
#define WF_DEFAULT_WEP_PHRASE          "${CONFIG_DRV_WIFI_WEP_PASS_PHRASE}" // customized WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "${CONFIG_DRV_WIFI_WEP_KEY_40}" // customized WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP == "WEP-104 (128-bit)">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WEP_104
#define WF_DEFAULT_WEP_PHRASE          "${CONFIG_DRV_WIFI_WEP_PASS_PHRASE}" // customized WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "${CONFIG_DRV_WIFI_WEP_KEY_104}" // customized WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
</#if><#-- CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP -->

#define WF_SAVE_WPS_CREDENTIALS        DRV_WIFI_DISABLED

#define DRV_WIFI_DEFAULT_ADHOC_PRESCAN DRV_WIFI_DISABLED

#define WF_CHECK_LINK_STATUS           WF_DISABLED /* Gets the MRF to check the link status relying on Tx failures. */
#define WF_LINK_LOST_THRESHOLD         40          /* Consecutive Tx transmission failures to be considered the AP is gone away. */

/* 
 * MRF24W FW has a built-in connection manager, and it is enabled by default.
 * If you want to run your own connection manager in host side, you should
 * disable the FW connection manager to avoid possible conflict between the two.
 * Especially these two APIs can be affected if you do not disable it.
 * A) uint16_t DRV_WIFI_Disconnect(void)
 * B) uint16_t DRV_WIFI_Scan(bool scanAll)
 * These APIs will return failure when the conflict occurs.
 */
#define WF_MODULE_CONNECTION_MANAGER   DRV_WIFI_ENABLED

#define WF_DEFAULT_PS_POLL             DRV_WIFI_DISABLED /* PS_POLL not supported in Ad-Hoc - must be set to DRV_WIFI_DISABLED */
#define WF_SOFTWARE_MULTICAST_FILTER   DRV_WIFI_ENABLED

#define WF_ENABLE_STATIC_IP
<#elseif CONFIG_DRV_WIFI_NETWORK_TYPE == "Soft AP">
#define WF_DEFAULT_NETWORK_TYPE        DRV_WIFI_NETWORK_TYPE_SOFT_AP
#define WF_DEFAULT_SSID_NAME           "${CONFIG_DRV_WIFI_SSID_NAME}"
#define WF_DEFAULT_LIST_RETRY_COUNT    (DRV_WIFI_RETRY_ADHOC) /* Dummy, not used */
#define WF_DEFAULT_CHANNEL_LIST        {6}                    /* Set SoftAP network channel */

<#if CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP == "Open">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_OPEN
#define WF_DEFAULT_WEP_PHRASE          "WEP Phrase" // default WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP == "WEP-40 (64-bit)">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WEP_40
#define WF_DEFAULT_WEP_PHRASE          "${CONFIG_DRV_WIFI_WEP_PASS_PHRASE}" // customized WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "${CONFIG_DRV_WIFI_WEP_KEY_40}" // customized WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "90E96780C739409DA50034FCAA" // default WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP == "WEP-104 (128-bit)">
#define WF_DEFAULT_WIFI_SECURITY_MODE  DRV_WIFI_SECURITY_WEP_104
#define WF_DEFAULT_WEP_PHRASE          "${CONFIG_DRV_WIFI_WEP_PASS_PHRASE}" // customized WEP passphrase
#define WF_DEFAULT_WEP_KEY_40          "5AFB6C8E77" // default WEP40 key
#define WF_DEFAULT_WEP_KEY_104         "${CONFIG_DRV_WIFI_WEP_KEY_104}" // customized WEP104 key
#define WF_DEFAULT_PSK_PHRASE          "Microchip 802.11 Secret PSK Password" // default WPA passphrase
#define WF_DEFAULT_WPS_PIN             "12390212" // default WPS PIN
</#if><#-- CONFIG_DRV_WIFI_SECURITY_MODE_ADHOC_SOFTAP -->

#define WF_SAVE_WPS_CREDENTIALS        DRV_WIFI_DISABLED

#define WF_CHECK_LINK_STATUS           WF_DISABLED /* Gets the MRF to check the link status relying on Tx failures. */
#define WF_LINK_LOST_THRESHOLD         40          /* Consecutive Tx transmission failures to be considered the AP is gone away. */
#define WF_SOFTAP_SEND_KEEP_ALIVE      WF_DISABLED /* Gets SoftAP to send keep alive packets to clients. */
#define WF_SOFTAP_LINK_LOST_THRESHOLD  40          /* Consecutive null packet transmission failures to be considered the client STA is gone away. */

/* 
 * MRF24W FW has a built-in connection manager, and it is enabled by default.
 * If you want to run your own connection manager in host side, you should
 * disable the FW connection manager to avoid possible conflict between the two.
 * Especially these two APIs can be affected if you do not disable it.
 * A) uint16_t DRV_WIFI_Disconnect(void)
 * B) uint16_t DRV_WIFI_Scan(bool scanAll)
 * These APIs will return failure when the conflict occurs.
 */
#define WF_MODULE_CONNECTION_MANAGER   DRV_WIFI_ENABLED

#define WF_DEFAULT_PS_POLL             DRV_WIFI_DISABLED /* PS_POLL not supported in SoftAP - must be set to DRV_WIFI_DISABLED */
#define WF_SOFTWARE_MULTICAST_FILTER   DRV_WIFI_ENABLED

#define WF_ENABLE_STATIC_IP
</#if><#-- CONFIG_DRV_WIFI_NETWORK_TYPE -->
</#if><#-- CONFIG_DRV_WIFI_DEVICE == "MRF24WG" -->

<#if CONFIG_DRV_WIFI_DEVICE == "MRF24WN">
<#if CONFIG_3RDPARTY_RTOS_USED == "FreeRTOS_V8.x.x">
#define WIFI_USE_RTOS
</#if><#-- CONFIG_3RDPARTY_RTOS_USED -->

<#if CONFIG_DRV_WIFIN_USE_OTA == true>
#define WDRV_EXT_INIT_TASK_STACK_SIZE 512u
#define WDRV_EXT_INIT_TASK_PRIO 6u
#define WDRV_EXT_MAIN_TASK_STACK_SIZE 2048u
#define WDRV_EXT_MAIN_TASK_PRIO 7u
<#else>
#define WDRV_EXT_INIT_TASK_STACK_SIZE ${CONFIG_DRV_WIFIN_RTOS_INIT_TASK_SIZE}u
#define WDRV_EXT_INIT_TASK_PRIO ${CONFIG_DRV_WIFIN_RTOS_INIT_TASK_PRIORITY}u
#define WDRV_EXT_MAIN_TASK_STACK_SIZE ${CONFIG_DRV_WIFIN_RTOS_MAIN_TASK_SIZE}u
#define WDRV_EXT_MAIN_TASK_PRIO ${CONFIG_DRV_WIFIN_RTOS_MAIN_TASK_PRIORITY}u
</#if><#-- CONFIG_DRV_WIFIN_USE_OTA -->

#define WDRV_ASSERT(condition, msg) WDRV_Assert(condition, msg, __FILE__, __LINE__)

<#if CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '0'>
#define DRV_WIFI_SPI_INDEX 0
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx0
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '1'>
#define DRV_WIFI_SPI_INDEX 1
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx1
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '2'>
#define DRV_WIFI_SPI_INDEX 2
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx2
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '3'>
#define DRV_WIFI_SPI_INDEX 3
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx3
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '4'>
#define DRV_WIFI_SPI_INDEX 4
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx4
<#elseif CONFIG_DRV_WIFI_SPI_INSTANCE_INDEX == '5'>
#define DRV_WIFI_SPI_INDEX 5
#define DRV_WIFI_SPI_INSTANCE sysObj.spiObjectIdx5
</#if>

<#if CONFIG_DRV_WIFI_STORE_CONFIG_IN_NVM>
#define DRV_WIFI_NVM_SPACE_ENABLE
#define DRV_WIFI_NVM_SPACE_ADDR (${CONFIG_DRV_WIFI_NVM_START_ADDRESS}*1024)
</#if>

<#if CONFIG_BSP_PIC32MZ_EC_SK == true>
#define MRF_INT_SOURCE INT_SOURCE_EXTERNAL_1
#define MRF_INT_VECTOR INT_VECTOR_INT1

// IO mapping for general control pins, including CS, RESET and HIBERNATE
// MRF24W in SPI 1 slot
#define WF_CS_PORT_CHANNEL PORT_CHANNEL_E
#define WF_CS_BIT_POS      9

#define WF_RESET_PORT_CHANNEL PORT_CHANNEL_F
#define WF_RESET_BIT_POS      0

#define WF_HIBERNATE_PORT_CHANNEL PORT_CHANNEL_F
#define WF_HIBERNATE_BIT_POS      1

#define WF_INT_PRIORITY     3
#define WF_INT_SUBPRIORITY  1
#define WF_INT_PORT_CHANNEL PORT_CHANNEL_E
#define WF_INT_BIT_POS      8
<#else>
#error "You are using a hardware configuration that doesn't have out-of-box Wi-Fi driver support. Please check the datasheet and configure the following Pins to make the Wi-Fi driver work."
//#define MRF_INT_SOURCE
//#define MRF_INT_VECTOR

//#define WF_CS_PORT_CHANNEL
//#define WF_CS_BIT_POS

//#define WF_RESET_PORT_CHANNEL
//#define WF_RESET_BIT_POS

//#define WF_HIBERNATE_PORT_CHANNEL
//#define WF_HIBERNATE_BIT_POS

//#define WF_INT_PRIORITY
//#define WF_INT_SUBPRIORITY
//#define WF_INT_PORT_CHANNEL
//#define WF_INT_BIT_POS
</#if><#-- CONFIG_BSP -->

<#if CONFIG_DRV_WIFIN_NETWORK_TYPE == "Infrastructure">
#define WDRV_DEFAULT_NETWORK_TYPE WDRV_NETWORK_TYPE_INFRASTRUCTURE
<#if CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WPS Push Button">
#define WDRV_DEFAULT_SSID_NAME ""
<#else>
#define WDRV_DEFAULT_SSID_NAME "${CONFIG_DRV_WIFIN_SSID_NAME}"
</#if>

<#if CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "Open">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_OPEN
#define WDRV_DEFAULT_WEP_KEYS_40 "5AFB6C8E77" // default WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "90E96780C739409DA50034FCAA" // default WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "Microchip 802.11 Secret PSK Password" // default WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WEP-40 (64-bit)">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_WEP_40
#define WDRV_DEFAULT_WEP_KEYS_40 "${CONFIG_DRV_WIFIN_WEP_KEY_40}" // customized WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "90E96780C739409DA50034FCAA" // default WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "Microchip 802.11 Secret PSK Password" // default WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WEP-104 (128-bit)">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_WEP_104
#define WDRV_DEFAULT_WEP_KEYS_40 "5AFB6C8E77" // default WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "${CONFIG_DRV_WIFIN_WEP_KEY_104}" // customized WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "Microchip 802.11 Secret PSK Password" // default WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WPA-PSK">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_WPA_WITH_PASS_PHRASE
#define WDRV_DEFAULT_WEP_KEYS_40 "5AFB6C8E77" // default WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "90E96780C739409DA50034FCAA" // default WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "${CONFIG_DRV_WIFIN_WPA_PASS_PHRASE}" // customized WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WPA2-PSK">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_WPA2_WITH_PASS_PHRASE
#define WDRV_DEFAULT_WEP_KEYS_40 "5AFB6C8E77" // default WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "90E96780C739409DA50034FCAA" // default WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "${CONFIG_DRV_WIFIN_WPA_PASS_PHRASE}" // customized WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WPS Push Button">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_WPS_PUSH_BUTTON
#define WDRV_DEFAULT_WEP_KEYS_40 "5AFB6C8E77" // default WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "90E96780C739409DA50034FCAA" // default WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "Microchip 802.11 Secret PSK Password" // default WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "12390212" // default WPS PIN
<#elseif CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE == "WPS PIN">
#define WDRV_DEFAULT_WIFI_SECURITY_MODE WDRV_SECURITY_WPS_PIN
#define WDRV_DEFAULT_WEP_KEYS_40 "5AFB6C8E77" // default WEP40 key
#define WDRV_DEFAULT_WEP_KEYS_104 "90E96780C739409DA50034FCAA" // default WEP104 key
#define WDRV_DEFAULT_PSK_PHRASE "Microchip 802.11 Secret PSK Password" // default WPA-PSK or WPA2-PSK passphrase
#define WDRV_DEFAULT_WPS_PIN "${CONFIG_DRV_WIFIN_WPS_PIN}" // customized WPS PIN
</#if><#-- CONFIG_DRV_WIFIN_SECURITY_MODE_INFRASTRUCTURE -->

#define WDRV_DEFAULT_CHANNEL 6
#define WDRV_DEFAULT_POWER_SAVE WDRV_FUNC_DISABLED

<#if CONFIG_DRV_WIFIN_USE_OTA == true>
#define WDRV_OTA_FILE_NAME "${CONFIG_DRV_WIFIN_OTA_FILE_NAME}"
#define WDRV_OTA_SERVER_IP_ADDR_STR "${CONFIG_DRV_WIFIN_OTA_SERVER_IP_STR}"
</#if><#-- CONFIG_DRV_WIFIN_USE_OTA -->
</#if><#-- CONFIG_DRV_WIFIN_NETWORK_TYPE -->
</#if><#-- CONFIG_DRV_WIFI_DEVICE == "MRF24WN" -->
<#--
/*******************************************************************************
 End of File
*/
-->
