/*******************************************************************************
  SPI Driver Functions for Dynamic Standard Buffer Driver Tasks Functions

  Company:
    Microchip Technology Inc.

  File Name:
    drv_spi_dynamic_master_dma_tasks.c

  Summary:
    SPI driver tasks functions

  Description:
    The SPI device driver provides a simple interface to manage the SPI
    modules on Microchip microcontrollers. This file contains implemenation
    for the SPI driver.
    
  Remarks:
  This file is generated from framework/driver/spi/template/drv_spi_dynamic_ebm_tasks.c.ftl
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
//DOM-IGNORE-END

#include "driver/spi/src/dynamic/drv_spi_internal.h"
#include <stdbool.h>

<#macro DRV_SPI_MASTER_DMA_TASKS_MACRO
            DRV_BIT_WIDTH 
            DRV_ISR>  
    <#if DRV_BIT_WIDTH=="8">
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASendData8BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendDataPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    <#elseif DRV_BIT_WIDTH=="16">
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASendData16BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendData16BitPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    <#else>
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASendData32BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendData32BitPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    </#if>
{
    struct DRV_SPI_DRIVER_OBJECT * pDrvInstance = (struct DRV_SPI_DRIVER_OBJECT * )contextHandle;
    SYS_DMA_ChannelDisable(pDrvInstance->txDmaChannelHandle);
    register DRV_SPI_JOB_OBJECT * currentJob = pDrvInstance->currentJob;
    register SPI_MODULE_ID spiId = pDrvInstance->spiId;    

    if (event != SYS_DMA_TRANSFER_EVENT_COMPLETE)
    {
        //handle Error
        return;
    }
    
    if (currentJob == NULL)
    {
        return;
    }
    
    /* Check to see if we'll send the next chunk */
    if (currentJob->dataLeftToTx > pDrvInstance->txDmaThreshold)
    {
        // Start a DMA transfer
        size_t txSize = MIN(PLIB_DMA_MAX_TRF_SIZE, currentJob->dataLeftToTx);
        void * bufferAddr = (void *)&(currentJob->txBuffer[currentJob->dataTxed]);
    <#if DRV_BIT_WIDTH=="8">
        SYS_DMA_ChannelTransferAdd(pDrvInstance->txDmaChannelHandle, bufferAddr, txSize, PLIB_SPI_BufferAddressGet(spiId), 1, 1);
    <#elseif DRV_BIT_WIDTH=="16">
        SYS_DMA_ChannelTransferAdd(pDrvInstance->txDmaChannelHandle, bufferAddr, txSize, PLIB_SPI_BufferAddressGet(spiId), 2, 2);
    <#else>
        SYS_DMA_ChannelTransferAdd(pDrvInstance->txDmaChannelHandle, bufferAddr, txSize, PLIB_SPI_BufferAddressGet(spiId), 4, 4);
    </#if>
        currentJob->dataTxed += txSize;
        currentJob->dataLeftToTx -= txSize;
        SYS_DMA_ChannelEnable(pDrvInstance->txDmaChannelHandle);
        SYS_DMA_ChannelForceStart(pDrvInstance->txDmaChannelHandle);
    }
    else
    {
        currentJob->txDMAProgressStage = DRV_SPI_DMA_COMPLETE;
    <#if DRV_ISR==true>
        SYS_INT_SourceEnable(pDrvInstance->txInterruptSource);
    </#if>
    }
}

    <#if DRV_BIT_WIDTH=="8">
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASendDummy8BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendDummyPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    <#elseif DRV_BIT_WIDTH=="16">
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASendDummy16BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendDummy16BitPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    <#else>
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASendDummy32BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendDummy32BitPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    </#if>
{
    struct DRV_SPI_DRIVER_OBJECT * pDrvInstance = (struct DRV_SPI_DRIVER_OBJECT * )contextHandle;
    SYS_DMA_ChannelDisable(pDrvInstance->txDmaChannelHandle);
    register DRV_SPI_JOB_OBJECT * currentJob = pDrvInstance->currentJob;
    register SPI_MODULE_ID spiId = pDrvInstance->spiId;    

    if (event != SYS_DMA_TRANSFER_EVENT_COMPLETE)
    {
        //handle Error
        return;
    }
    
    if (currentJob == NULL)
    {
        return;
    }
    
    /* Check to see if we'll send the next chunk */
    if (currentJob->dummyLeftToTx > pDrvInstance->txDmaThreshold)
    {
        // Start a DMA transfer
        size_t txSize = MIN(PLIB_DMA_MAX_TRF_SIZE, currentJob->dummyLeftToTx);
        void * bufferAddr = (void *)&(currentJob->txBuffer[currentJob->dataTxed]);
    <#if DRV_BIT_WIDTH=="8">
        SYS_DMA_ChannelTransferAdd(pDrvInstance->txDmaChannelHandle, sSPITxDummyBuffer, txSize, PLIB_SPI_BufferAddressGet(spiId), 1, 1);
    <#elseif DRV_BIT_WIDTH=="16">
        SYS_DMA_ChannelTransferAdd(pDrvInstance->txDmaChannelHandle, sSPITxDummyBuffer, txSize, PLIB_SPI_BufferAddressGet(spiId), 2, 2);
    <#else>
        SYS_DMA_ChannelTransferAdd(pDrvInstance->txDmaChannelHandle, sSPITxDummyBuffer, txSize, PLIB_SPI_BufferAddressGet(spiId), 4, 4);
    </#if>
        currentJob->dummyTxed += txSize;
        currentJob->dummyLeftToTx -= txSize;
        SYS_DMA_ChannelEnable(pDrvInstance->txDmaChannelHandle);
        SYS_DMA_ChannelForceStart(pDrvInstance->txDmaChannelHandle);
    }
    else
    {
        currentJob->txDMAProgressStage = DRV_SPI_DMA_COMPLETE;
    <#if DRV_ISR==true>
        SYS_INT_SourceEnable(pDrvInstance->txInterruptSource);
    </#if>
    }
}

    <#if DRV_BIT_WIDTH=="8">
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASend8BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASendPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    <#elseif DRV_BIT_WIDTH=="16">
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASend16BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASend16BitPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    <#else>
        <#if DRV_ISR==true>
void DRV_SPI_MasterDMASend32BitISR(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        <#else>
void DRV_SPI_MasterDMASend32BitPolled(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
        </#if>
    </#if>
{
    struct DRV_SPI_DRIVER_OBJECT * pDrvInstance = (struct DRV_SPI_DRIVER_OBJECT * )contextHandle;
    register DRV_SPI_JOB_OBJECT * currentJob = pDrvInstance->currentJob;

    switch(currentJob->txDMAProgressStage)
    {
        case DRV_SPI_DMA_DATA_INPROGRESS:
    <#if DRV_BIT_WIDTH=="8">
        <#if DRV_ISR==true>
            DRV_SPI_MasterDMASendData8BitISR(event, handle, contextHandle);
        <#else>
            DRV_SPI_MasterDMASendDataPolled(event, handle, contextHandle);
        </#if>
    <#elseif DRV_BIT_WIDTH=="16">
        <#if DRV_ISR==true>
            DRV_SPI_MasterDMASendData16BitISR(event, handle, contextHandle);
        <#else>
            DRV_SPI_MasterDMASendData16BitPolled(event, handle, contextHandle);
        </#if>
    <#else>
        <#if DRV_ISR==true>
            DRV_SPI_MasterDMASendData32BitISR(event, handle, contextHandle);
        <#else>
            DRV_SPI_MasterDMASendData32BitPolled(event, handle, contextHandle);
        </#if>
    </#if>
            break;
        case DRV_SPI_DMA_DUMMY_INPROGRESS:
    <#if DRV_BIT_WIDTH=="8">
        <#if DRV_ISR==true>
            DRV_SPI_MasterDMASendDummy8BitISR(event, handle, contextHandle);
        <#else>
            DRV_SPI_MasterDMASendDummyPolled(event, handle, contextHandle);
        </#if>
    <#elseif DRV_BIT_WIDTH=="16">
        <#if DRV_ISR==true>
            DRV_SPI_MasterDMASendDummy16BitISR(event, handle, contextHandle);
        <#else>
            DRV_SPI_MasterDMASendDummy16BitPolled(event, handle, contextHandle);
        </#if>
    <#else>
        <#if DRV_ISR==true>
            DRV_SPI_MasterDMASendDummy32BitISR(event, handle, contextHandle);
        <#else>
            DRV_SPI_MasterDMASendDummy32BitPolled(event, handle, contextHandle);
        </#if>
    </#if>
            break;
        default:
            break;
    }
}
</#macro>    


<#if CONFIG_DRV_SPI_USE_ISR_MODE==true &&  CONFIG_DRV_SPI_USE_8BIT_MODE==true>
    <@DRV_SPI_MASTER_DMA_TASKS_MACRO
        DRV_BIT_WIDTH="8"
        DRV_ISR=true
     />
</#if>
<#if CONFIG_DRV_SPI_USE_POLLED_MODE==true &&  CONFIG_DRV_SPI_USE_8BIT_MODE==true>
    <@DRV_SPI_MASTER_DMA_TASKS_MACRO
        DRV_BIT_WIDTH="8"
        DRV_ISR=false
     />
</#if>
<#if CONFIG_DRV_SPI_USE_ISR_MODE==true &&  CONFIG_DRV_SPI_USE_16BIT_MODE==true>
    <@DRV_SPI_MASTER_DMA_TASKS_MACRO
        DRV_BIT_WIDTH="16"
        DRV_ISR=true
     />
</#if>
<#if CONFIG_DRV_SPI_USE_POLLED_MODE==true &&  CONFIG_DRV_SPI_USE_16BIT_MODE==true>
    <@DRV_SPI_MASTER_DMA_TASKS_MACRO
        DRV_BIT_WIDTH="16"
        DRV_ISR=false
     />
</#if>
<#if CONFIG_DRV_SPI_USE_ISR_MODE==true &&  CONFIG_DRV_SPI_USE_32BIT_MODE==true>
    <@DRV_SPI_MASTER_DMA_TASKS_MACRO
        DRV_BIT_WIDTH="32"
        DRV_ISR=true
     />
</#if>
<#if CONFIG_DRV_SPI_USE_POLLED_MODE==true &&  CONFIG_DRV_SPI_USE_32BIT_MODE==true>
    <@DRV_SPI_MASTER_DMA_TASKS_MACRO
        DRV_BIT_WIDTH="32"
        DRV_ISR=false
     />
</#if>