/*******************************************************************************
  MRF24W SPI Driver

  File Name:
    drv_wifi_spi.c

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

#include "driver/spi/drv_spi.h"
#include "tcpip/src/tcpip_private.h"

/*
// BRG = (clockFrequency/(2 * baudRate)) - 1
// This is standard formula for calculating baud rate.
*/

static SYS_MODULE_OBJ *p_drvSPIObject = &DRV_WIFI_SPI_INSTANCE;

static DRV_HANDLE drvSPIHandle;
static DRV_SPI_BUFFER_HANDLE spiBufferHandleTx, spiBufferHandleRx;

bool DRV_WIFI_SpiInit()
{
#if defined(PLIB_PORTS_ExistsPortsDirection)
    if (PLIB_PORTS_ExistsPortsDirection(PORTS_ID_0))
    {
        WF_CS_Init();
    }
#endif

    WF_CS_Deassert(); // Disable chip select before initialization

    drvSPIHandle = DRV_SPI_Open(DRV_WIFI_SPI_INDEX, DRV_IO_INTENT_READWRITE|DRV_IO_INTENT_BLOCKING);
    if (drvSPIHandle == (DRV_SPI_BUFFER_HANDLE)NULL)
    {
        //DRV_WIFI_ASSERT(false, "SPI open error");
        return false;
    }

    return true;
}

void DRV_WIFI_SpiClose()
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
        //DRV_WIFI_ASSERT(false, "SPI write error");
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

bool IspSpi_Rx(unsigned char *buf, int size)
{
    SYS_INT_PROCESSOR_STATUS intStatusSaving;

    if (size==0) {
        return true;
    }

    intStatusSaving = SYS_INT_StatusGetAndDisable(); // Disable all interrupts

    spiBufferHandleRx = DRV_SPI_BufferAddRead(drvSPIHandle, buf, size, 0, 0);
    if (spiBufferHandleRx == (DRV_SPI_BUFFER_HANDLE)NULL)
    {
        //DRV_WIFI_ASSERT(false, "SPI read error");
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

void IspSpi_TxAndRx(unsigned char *p_txBuf, int txLen, unsigned char *p_rxBuf, int rxLen)
{
    IspSpi_Tx(p_txBuf, txLen);
    IspSpi_Rx(p_rxBuf, rxLen);
}

//DOM-IGNORE-END
