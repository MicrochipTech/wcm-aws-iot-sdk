/*************************************************************************
  Company: 
    Microchip Technology Inc.
  
  File Name:
    wdrv_mrf24wn_api.h
	
  Summary:
  MRF24WN interface functions
	
  Description:
  MRF24WN interface functions                                           
  *************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright © 2012 released Microchip Technology Inc. All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/
// DOM-IGNORE-END

#ifndef _WDRV_MRF24WN_API_H
#define _WDRV_MRF24WN_API_H

/*******************************************************************************
  Function:
        int32_t WDRV_EXT_Initialize(CallBacks const * const CB);

  Summary:
    Initializes the MRF24WN Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function initializes the MRF24W driver, making it ready for
    clients to use.

  Precondition:
    None.

  Parameters:
    CB - Pointer to callback functions

  Returns:
    If successful returns 0, else non-zero value. 

  Remarks:
    None.

 *******************************************************************************/
int32_t WDRV_EXT_Initialize(CallBacks const * const CB);

/*******************************************************************************
  Function:
        void WDRV_EXT_Deinitialize(void);

  Summary:
    Deinitializes the MRF24WN Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function deinitializes the MRF24WN driver.
    
  Precondition:
    None.

  Returns:
    None.

  Remarks:
    None
 *******************************************************************************/
void WDRV_EXT_Deinitialize(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_DataSend(uint16_t segSize, uint8_t *p_segData);

  Summary:
    Sends data packets to MRF24WN module.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Sends data packets to MRF24WN module.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    seqSize - data size
    p_seqData - pointer to the data buffer

  Returns:
    If successful returns 0, else non-zero value. 

  Remarks:
    None.

 *******************************************************************************/
uint32_t WDRV_EXT_DataSend(uint16_t segSize, uint8_t *p_segData);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSSIDSet(uint8_t *ssid, uint16_t len);

  Summary:
    Sets the SSID.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Sets the SSID and SSID Length. 

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    ssid -      Pointer to SSID buffer
    len -  number of bytes in SSID

  Returns:
    If successful returns 0, else non-zero value. 

  Remarks:
    Do not include a string terminator in the SSID length. SSIDs are
    case-sensitive. SSID length must be less than or equal to 32 
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSSIDSet(uint8_t *ssid, uint16_t len);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdNetModeBSSSet(void);

  Summary:
    Sets the Wi-Fi network type to Infrastruture.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi network type to Infrastructure.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    None.

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdNetModeBSSSet(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdNetModeIBSSSet(void);

  Summary:
    Sets the Wi-Fi network type to Adhoc.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi network type to Adhoc.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    None.

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdNetModeIBSSSet(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdNetModeAPSet(void);

  Summary:
    Sets the Wi-Fi network type to SoftAP.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi network type to SoftAP.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    None.

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdNetModeAPSet(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdConnect(void);

  Summary:
    Directs the MRF24WN to connect to a Wi-Fi network.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function causes the MRF24WN to connect to a Wi-Fi network. Upon
    connection, or a failure to connect, an event will be generated.

  Precondition:
    Wi-Fi initialization must be complete and relevant connection parameters
    must have been set.

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdConnect(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdDisconnect(void);

  Summary:
    Directs the MRF24WN to disconnect from a Wi-Fi network.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function causes the MRF24WN to disconnect from a Wi-Fi network. 

  Precondition:
    Wi-Fi initialization must be complete and a connection must be in
    progress.

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdDisconnect(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSecNoneSet(void);

  Summary:
    Sets Wi-Fi security to open (no security).
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi security to open. One can only connect to
    an AP that is running in open mode.

  Precondition:
    Wi-Fi initialization must be complete. Must be in an unconnected state.

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSecNoneSet(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSecWEPSet(uint8_t *key, uint16_t len);

  Summary:
    Sets Wi-Fi security to use WEP.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi security to WEP. One can only connect to
    an AP that is running the same WEP mode.

  Precondition:
    Wi-Fi initialization must be complete. Must be in an unconnected state.

  Parameters:
    key - pointer to the WEP key buffer
    len - WEP key length

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSecWEPSet(uint8_t *key, uint16_t len);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSecWPASet(uint8_t *key, uint16_t len);

  Summary:
    Sets Wi-Fi security to use WPA.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi security to WPA. One can only
    connect to an AP that is running the same WPA mode.

  Precondition:
    Wi-Fi initialization must be complete. Must be in an unconnected state.

  Parameters:
    key - pointer to the WPA key buffer
    len -  WPA key length

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSecWPASet(uint8_t *key, uint16_t len);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSecWPA2Set(uint8_t *key, uint16_t len);

  Summary:
    Sets Wi-Fi security to use WPA2.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi security to WPA2. One can only
    connect to an AP that is running the same WPA2 mode.

  Precondition:
    Wi-Fi initialization must be complete. Must be in an unconnected state.

  Parameters:
    key - pointer to the WPA2 key buffer
    len -  WPA2 key length

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSecWPA2Set(uint8_t *key, uint16_t len);

/*******************************************************************************
  Function:
        int32_t WDRV_EXT_CmdSecWpsSet(bool pinMode, uint8_t *key, uint16_t keyLen);

  Summary:
    Sets Wi-Fi security to use WPS.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function sets the Wi-Fi security to WPS. One can only connect to
    an AP that supports WPS.

  Precondition:
    Wi-Fi initialization must be complete. Must be in an unconnected state.

  Parameters:
    pinMode - 0: PBC mode, 1: PIN mode
    key : pointer of the PIN buffer
    keyLen :  PIN length

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSecWpsSet(bool pinMode, uint8_t *key, uint16_t keyLen);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdPowerSavePut(bool enable);

  Summary:
    Puts the module in IEEE power save mode.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Puts the module in IEEE power save mode.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    enable -      true will put the module in IEEE power save mode.

  Returns:
    If successful returns 0, else non-zero value. 

  Remarks:
    This works only with Infrastructure mode.  Do not call this in other modes. 
 *******************************************************************************/
uint32_t WDRV_EXT_CmdPowerSavePut(bool enable);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSSIDGet(uint8_t *ssid, uint8_t *length);

  Summary:
    Gets the SSID.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Gets the SSID and SSID Length.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    ssid -      Pointer to buffer where SSID will be written
    length -  number of bytes in SSID

  Returns:
     If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSSIDGet(uint8_t *ssid, uint8_t *length);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdConnectContextChannelGet(uint16_t *bssChannel);

  Summary:
    Gets the AP channel
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function gets the current AP channel.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    bssChannel -  pointer where the current AP channel will be written

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdConnectContextChannelGet(uint16_t *bssChannel);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdConnectContextBssidGet(uint8_t *bssId);

  Summary:
    Gets the BSSID 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function gets the current AP's BSSID.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    bssId-  pointer where the current AP's BSSID will be written

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None
 *******************************************************************************/
uint32_t WDRV_EXT_CmdConnectContextBssidGet(uint8_t *bssId);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdMacAddressGet(uint8_t *MacAddr);

  Summary:
    Retrieves the MRF24WN MAC address.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function retrieves the MRF24WN MAC address.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    MacAddr -  Pointer where mac address will be written (must point to a
             6 byte buffer)

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
uint32_t WDRV_EXT_CmdMacAddressGet(uint8_t *MacAddr);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdPowerSaveGet(bool *enabled);

  Summary:
    Retrieves current power save status.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Retrieves current power save status.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    enabled -  Pointer where the current power save status will be written.

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
uint32_t WDRV_EXT_CmdPowerSaveGet(bool *enabled);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdSecGet(uint32_t *SecurityType);

  Summary:
    Gets the current Wi-Fi security type.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function gets the current Wi-Fi security type.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
   SecurityType - pointer where the security type will be written

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
uint32_t WDRV_EXT_CmdSecGet(uint32_t *SecurityType);

/*******************************************************************************
  Function:
        WDRV_EXT_CmdChannelSet(uint16_t channel);

  Summary:
    Sets the channel to operate on.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Sets the channel to operate on.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    channel -      target channel

  Returns:
    If successful returns 0, else non-zero value. 

  Remarks:
    This works only with SoftAP mode.  Do not call this in other modes. 
 *******************************************************************************/
uint32_t WDRV_EXT_CmdChannelSet(uint16_t channel);

/*******************************************************************************
  Function:
        int32_t WDRV_EXT_CmdRFTest(int32_t argc, char* argv[]);

  Summary:
    Performs RF test for regulatory test purpsoe. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Performs RF test for regulatory test purpsoe. 

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
    argc - argument count.
    argv - argument vector.

  Example:

  Use the arguments in this order :
  
  rate packet_count packet_size channel header_type
  rate: 
    0 = 1mbps, 1 = 2mbps, 2  = 5.5mbps, 3 = 11mbps, 4 = 6mbps, 5 = 9mbps, 6 = 12mbps, 7 = 18mbps,
    8 = 24mbps, 9 = 36mbps, 10 = 48mbps, 11 = 54mbps, 12 = 6.5mbps, 13 = 13mbps, 14 = 19.5mbps, 
    15 = 26mbps, 16 = 39mbps, 17 = 52mbps, 18 = 58.5mbps
  packet_count: number of transmits 1 - 14
  packet_size: payload size 0 to 1400
  channel: 1 - 14
  header_type:  0 - beacon frame; 1 - QOS data frame; 2 - 4 address data frame

  Returns:
    If successful returns 0, else non-zero value. 

  Remarks:
    None.
 *******************************************************************************/
int32_t WDRV_EXT_CmdRFTest(int32_t argc, char* argv[]);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdScanStart(void);

  Summary:
    Directs MRF24WN to start scan.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Directs MRF24WN to start scan.

  Precondition:
    Wi-Fi initialization must be complete.

  Parameters:
   None.

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
uint32_t WDRV_EXT_CmdScanStart(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_CmdScanGet(uint16_t *numOfResults);

  Summary:
    Read the number of scan results from MRF24WN.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Read the number of scan results from MRF24WN.

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    numOfResults -    pointer where the number of scan results will be written

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
uint32_t WDRV_EXT_CmdScanGet(uint16_t *numOfResults);

/*******************************************************************************
  Function:
        void WDRV_EXT_ScanGetResult(uint8_t listIndex, WDRV_SCAN_RESULT *p_scanResult);

  Summary:
    Read selected scan results back from MRF24WN.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    After a scan has completed this function is used to read one scan
    result at a time from the MRF24WGN.

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    listIndex -     Index (0 based list) of the scan entry to retrieve.
    p_scanResult -  Pointer to where scan result is written. 

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_EXT_ScanGetResult(uint8_t listIndex, WDRV_SCAN_RESULT *p_scanResult);

/*******************************************************************************
  Function:
        void WDRV_EXT_ScanDoneSet(void);

  Summary:
    Indicates scan is completed.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Indicates scan is completed.

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_EXT_ScanDoneSet(void);

/*******************************************************************************
  Function:
        void WDRV_EXT_ScanIsInProgress(void);

  Summary:
    Check whether host scan is now in progress or not. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Check whether host scan is now in progress or not. 

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    None.

  Returns:
    If it is in progress, returns true. Otherwise false. 

  Remarks:
    None.
 *******************************************************************************/
bool WDRV_EXT_ScanIsInProgress(void);

/*******************************************************************************
  Function:
        void WDRV_EXT_WPSResultsRead(WDRV_CONFIG_DATA *config, uint32_t *status);

  Summary:
    Read WPS process results back from MRF24WN and update the configuration data.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    After WPS process has completed this function is used to read WPS process
    results from the MRF24WGN and update the configuration data.

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    config -  Pointer to where configuration data will be updated.  
    status - Pointer to where WPS process status will be written.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_EXT_WPSResultsRead(WDRV_CONFIG_DATA *config, uint32_t *status);

/*******************************************************************************
  Function:
        void WDRV_EXT_CmdFWUpdate(void);

  Summary:
    Directs the module to start FW download and upgrade. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Directs the module to start FW download and upgrade. 

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    None.

  Returns:
    None.

  Remarks:
    None.
 *******************************************************************************/

void WDRV_EXT_CmdFWUpdate(void);

/*******************************************************************************
  Function:
        uint32_t WDRV_EXT_PowerUpDown(uint32_t up);

  Summary:
    Power down or up MRF24WN
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Power down or up MRF24WN

  Precondition:
    Wi-Fi initialization must be complete. 

  Parameters:
    up - 1: power up, 0: power down

  Returns:
    If successful returns 0, else non-zero value.

  Remarks:
    None.
 *******************************************************************************/
uint32_t WDRV_EXT_PowerUpDown(uint32_t up);

/*******************************************************************************
  Function:
        void WDRV_EXT_HWInterruptHandler(void const *pointer);

  Summary:
    Wi-Fi driver (MRF24WN specific) interrupt service routine.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function is Wi-Fi driver (MRF24WN specific) interrupt service routine.

  Precondition:
    Wi-Fi initialization must be complete.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_EXT_HWInterruptHandler(void const *pointer);

/*******************************************************************************
  Function:
        void WDRV_INTR_SourceEnable(void);

  Summary:
    Enables interrupt from the module. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Enables interrupt from the module. 

  Precondition:
    Wi-Fi initialization must be complete.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_INTR_SourceEnable(void);

/*******************************************************************************
  Function:
        void WDRV_INTR_SourceDisable(void);

  Summary:
    Disables interrupt from the module. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Disables interrupt from the module. 

  Precondition:
    Wi-Fi initialization must be complete.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_INTR_SourceDisable(void);

/*******************************************************************************
  Function:
        void WDRV_INTR_Init(void);

  Summary:
    Initializes interrupt for Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function initializes interrupt for Wi-Fi driver.

  Precondition:
    TCP/IP stack should be initialized.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_INTR_Init(void);

/*******************************************************************************
  Function:
        void WDRV_INTR_Deinit(void);

  Summary:
    Deinitializes interrupt for Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function deinitializes interrupt for Wi-Fi driver.

  Precondition:
    TCP/IP stack should be initialized.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_INTR_Deinit(void);

/*******************************************************************************
  Function:
        void WDRV_SPI_Out(uint8_t const * const bufOut, uint16_t OutSize);

  Summary:
    Sends data out to the module through SPI bus.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Sends data out to the module through SPI bus.

  Precondition:
    TCP/IP stack should be initialized.

  Parameters:
    bufOut - buffer pointer of output data.
    OutSize - the data size.
    
  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_SPI_Out(uint8_t * const bufOut, uint16_t OutSize);

/*******************************************************************************
  Function:
        void WDRV_SPI_In(uint8_t const * const OutBuf, uint16_t OutSize,
            uint8_t * const InBuf, uint16_t InSize);

  Summary:
    Receives data from the module through SPI bus.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Receives data from the module through SPI bus.

  Precondition:
    TCP/IP stack should be initialized.

  Parameters:
    bufOut - buffer pointer of output command.
    OutSize - the command size.
    InBuf - buffer pointer of input data.
    InSize - the input data size.
    
  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_SPI_In(uint8_t * const OutBuf, uint16_t OutSize, uint8_t * const InBuf, uint16_t InSize);

/*******************************************************************************
  Function:
        void WDRV_SPI_Init(void);

  Summary:
    Initializes SPI object for Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function initializes SPI object for Wi-Fi driver.

  Precondition:
    TCP/IP stack should be initialized.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_SPI_Init(void);

/*******************************************************************************
  Function:
        void WDRV_SPI_Deinit(void);

  Summary:
    Deinitializes SPI object for Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function deinitializes SPI object for Wi-Fi driver.

  Precondition:
    TCP/IP stack should be initialized.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_SPI_Deinit(void);

/*******************************************************************************
  Function:
        void WDRV_GPIO_Init(int board);   

  Summary:
    Initializes GPIO object for Wi-Fi driver.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function initializes GPIO object for Wi-Fi driver.

  Precondition:
    TCP/IP stack should be initialized.

  Parameters:
    board - MCHP dev kit type, for instance, MZ/ESK.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_GPIO_Init(int board); 

/*******************************************************************************
  Function:
        void WDRV_GPIO_OutLow(int board); 

  Summary:
    Asserts power down signal. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Asserts power down signal. 

  Precondition:
    TCP/IP stack should be initialized.

  Parameters:
    board - MCHP dev kit type, for instance, MZ/ESK.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_GPIO_OutLow(int board);    

/*******************************************************************************
  Function:
        void WDRV_GPIO_OutHigh(int board);  

  Summary:
    Negates power down signal. 
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Negates power down signal. 

  Precondition:
    TCP/IP stack should be initialized.

  Parameters:
    board - MCHP dev kit type, for instance, MZ/ESK.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_GPIO_OutHigh(int board);    

/*******************************************************************************
  Function:
        bool WDRV_CLI_Init(void);

  Summary:
    Initializes console CLI interface.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    Initializes console CLI interface.

  Precondition:
    TCP/IP stack should be initialized.

  Parameters:
    None.

  Return:
    If successful returns true, else non-zero false.

  Remarks:
    None.
 *******************************************************************************/
bool WDRV_CLI_Init(void);

/*******************************************************************************
  Function:
        void WDRV_MRF24WN_ISR(SYS_MODULE_OBJ index)

  Summary:
    Wi-Fi driver (MRF24WN specific) interrupt service routine.
    <p><b>Implementation:</b> Dynamic</p>

  Description:
    This function is Wi-Fi driver (MRF24WN specific) interrupt service routine.

  Precondition:
    Wi-Fi initialization must be complete.

  Return:
    None.

  Remarks:
    None.
 *******************************************************************************/
void WDRV_MRF24WN_ISR(SYS_MODULE_OBJ index);

/*******************************************************************************
Wi-Fi Commands Help

iwconfig
  Wi-Fi configuration
  Usage:
    iwconfig
    iwconfig ssid <ssid>
    iwconfig mode <mode>
    iwconfig security <security_mode> <key>/<pin>
    iwconfig power <enable/disable>
    iwconfig scan
    iwconfig scanget <scan_index>
  <ssid>:
    32 characters string - no blank or space allowed in this demo
  <mode>:
    managed/idle
  <security_mode>:
    open/wep40/wep104/wpa/wpa2/pin/pbc
    No blank or space allowed in <key> in current console commands
    Ex: iwconfig security open
        iwconfig security wep40 5AFB6C8E77
        iwconfig security wep104 90E96780C739409DA50034FCAA
        iwconfig security wpa microchip_psk
        iwconfig security wpa2 microchip_psk
        iwconfig security pin 12390212
        iwconfig security pbc

rftest
  RF test

mac
  Get MAC address

readconf
  Read from storage

erase conf
  Erase storage

saveconf
  Save storage

ota
  Over The Air FW update
 *******************************************************************************/

#endif /* _WDRV_MRF24WN_API_H */
