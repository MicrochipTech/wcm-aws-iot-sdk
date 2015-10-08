/*******************************************************************************
  MRF24W SPI Driver

  File Name: 
    wdrv_mrf24wn_spi.c  
  
  Summary:
    Module for Microchip TCP/IP Stack
    
  Description:
    Supports SPI communications to the MRF24W module
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <sys/attribs.h>

#include "driver/spi/drv_spi.h"
#include "system/clk/sys_clk.h"
#include "system/clk/src/sys_clk_local.h"
#include "system/int/sys_int.h"
#include "peripheral/dma/plib_dma.h"

#include "wdrv_mrf24wn_common.h"

/*
// BRG = (clockFrequency/(2 * baudRate)) - 1
// This is standard formula for calculating baud rate.
*/

static SYS_MODULE_OBJ *p_drvSPIObject = &DRV_WIFI_SPI_INSTANCE;

static DRV_HANDLE drvSPIHandle;
static DRV_SPI_BUFFER_HANDLE spiBufferHandleTx, spiBufferHandleRx;

static void CS_Init(void)      
{
#if defined(PLIB_PORTS_ExistsPortsDirection)
    if (PLIB_PORTS_ExistsPortsDirection(PORTS_ID_0))
    {
        PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, WF_CS_PORT_CHANNEL, WF_CS_BIT_POS);
    }
#endif
}

static void CS_Assert()
{
    SYS_PORTS_PinClear(PORTS_ID_0, WF_CS_PORT_CHANNEL, WF_CS_BIT_POS);
}

static void CS_Deassert()
{
    SYS_PORTS_PinSet(PORTS_ID_0, WF_CS_PORT_CHANNEL, WF_CS_BIT_POS);
}

bool DRV_WIFI_SpiInit(void)
{
    drvSPIHandle = DRV_SPI_Open(DRV_WIFI_SPI_INDEX, DRV_IO_INTENT_READWRITE|DRV_IO_INTENT_BLOCKING);
    if (drvSPIHandle == (DRV_SPI_BUFFER_HANDLE)NULL)
    {
        //WDRV_ASSERT(false, "SPI open error");
        return false;
    }

    // Without the SPIFE bit set we can't run at 20MHz.  Not sure why as this bit
    // seems like it is only applicable in SPI frame mode, which we are not using.
    //SPI1CONSET = 0x00000200;
    return true;
}

void DRV_WIFI_SpiClose(void)
{
    DRV_SPI_Close(drvSPIHandle);
}

bool IspSpi_Tx(unsigned char *buf, int size)
{
    SYS_INT_PROCESSOR_STATUS intStatusSaving;

    if (size == 0) {
        return true;
    }

    intStatusSaving = SYS_INT_StatusGetAndDisable(); // Disable all interrupts

    spiBufferHandleTx = DRV_SPI_BufferAddWrite(drvSPIHandle, buf, size, 0, 0);
    if (spiBufferHandleTx == (DRV_SPI_BUFFER_HANDLE)NULL)
    {
        // WDRV_ASSERT(false, "SPI write error");
        SYS_INT_StatusRestore(intStatusSaving); // Restore interrupts
        return false;
    }

    while (DRV_SPI_BUFFER_EVENT_COMPLETE != DRV_SPI_BufferStatus(spiBufferHandleTx)) // Check for the successful data transfer
    {
        DRV_SPI_Tasks(*p_drvSPIObject);
    }

    SYS_INT_StatusRestore(intStatusSaving); // Restore interrupts
    return true;
}

bool IspSpi_Rx(unsigned char * const buf, int size)
{
    SYS_INT_PROCESSOR_STATUS intStatusSaving;

    if (size == 0) {
        return true;
    }

    intStatusSaving = SYS_INT_StatusGetAndDisable(); // Disable all interrupts

    spiBufferHandleRx = DRV_SPI_BufferAddRead(drvSPIHandle, buf, size, 0, 0);
    if (spiBufferHandleRx == (DRV_SPI_BUFFER_HANDLE)NULL)
    {
        // WDRV_ASSERT(false, "SPI read error");
        SYS_INT_StatusRestore(intStatusSaving); // Restore interrupts
        return false;
    }

    while (DRV_SPI_BUFFER_EVENT_COMPLETE != DRV_SPI_BufferStatus(spiBufferHandleRx)) // Check for the successful data transfer
    {
        DRV_SPI_Tasks(*p_drvSPIObject);
    }

    SYS_INT_StatusRestore(intStatusSaving); // Restore interrupts

    return true;
}

void IspSpi_TxAndRx(unsigned char *p_txBuf, int txLen, unsigned char * const p_rxBuf, int rxLen)
{
    IspSpi_Tx(p_txBuf, txLen);
    IspSpi_Rx(p_rxBuf, rxLen);
}

void WDRV_SPI_Out(uint8_t * const bufOut, uint16_t OutSize)
{
    uint8_t rxBuf[4];
    bool intEnabled = SYS_INT_SourceDisable(MRF_INT_SOURCE);

    WDRV_DISABLE_INTR();
    CS_Assert();
    IspSpi_TxAndRx(bufOut, OutSize, rxBuf, 0);
    CS_Deassert();
    WDRV_ENABLE_INTR();
    if(intEnabled)
        WDRV_INTR_SourceEnable();
}


void WDRV_SPI_In(uint8_t * const OutBuf, uint16_t OutSize, uint8_t * const InBuf, uint16_t InSize)
{
    bool intEnabled = SYS_INT_SourceDisable(MRF_INT_SOURCE);

    WDRV_DISABLE_INTR();
    CS_Assert();
    IspSpi_TxAndRx(OutBuf, OutSize, InBuf, InSize);
    CS_Deassert();
    WDRV_ENABLE_INTR();
    if(intEnabled)
        WDRV_INTR_SourceEnable();
}

void WDRV_SPI_Init(void)
{   
    // Initialize IO for WF chip select    
    CS_Init();     
    CS_Deassert(); // Disable chip select before initialization
    DRV_WIFI_SpiInit(); 
}

void WDRV_SPI_Deinit(void)
{
    DRV_WIFI_SpiClose();
}

//DOM-IGNORE-END
