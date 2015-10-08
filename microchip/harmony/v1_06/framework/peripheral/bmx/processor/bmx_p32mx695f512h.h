/* Created by plibgen $Revision: 1.31 $ */

#ifndef _BMX_P32MX695F512H_H
#define _BMX_P32MX695F512H_H

/* Section 1 - Enumerate instances, define constants, VREGs */

#include <xc.h>
#include <stdbool.h>

#include "peripheral/peripheral_common_32bit.h"

/* Default definition used for all API dispatch functions */
#ifndef PLIB_INLINE_API
    #define PLIB_INLINE_API extern inline
#endif

/* Default definition used for all other functions */
#ifndef PLIB_INLINE
    #define PLIB_INLINE extern inline
#endif

typedef enum {

    BMX_ID_0 = 0,
    BMX_NUMBER_OF_MODULES

} BMX_MODULE_ID;

PLIB_INLINE SFR_TYPE* _BMX_ARB_MODE_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_DRM_WS_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BUS_EXCEPTION_INST_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BUS_EXCEPTION_DATA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BUS_EXCEPTION_DMA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BUS_EXCEPTION_ICD_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BUS_EXCEPTION_IXI_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_PFM_CACHE_DMA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXCON;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXDKPBA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXDKPBA;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXDUDBA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXDUDBA;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXDUPBA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXDUPBA;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXPUPBA_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXPUPBA;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXDRMSZ_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXDRMSZ;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXPFMSZ_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXPFMSZ;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _BMX_BMXBOOTSZ_VREG(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return &BMXBOOTSZ;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_ARB_MODE_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXARB_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_DRM_WS_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXWSDRM_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_INST_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRIS_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_DATA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRDS_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_DMA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRDMA_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_ICD_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRICD_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_IXI_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRIXI_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_PFM_CACHE_DMA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXCHEDMA_MASK;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDKPBA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDUDBA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDUPBA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXPUPBA_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDRMSZ_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXPFMSZ_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXBOOTSZ_MASK(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)-1;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_ARB_MODE_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXARB_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_DRM_WS_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXWSDRM_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_INST_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRIS_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_DATA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRDS_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_DMA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRDMA_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_ICD_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRICD_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_IXI_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRIXI_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_PFM_CACHE_DMA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXCHEDMA_POSITION;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDKPBA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDUDBA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDUPBA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXPUPBA_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDRMSZ_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXPFMSZ_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXBOOTSZ_POS(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)0;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_ARB_MODE_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXARB_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_DRM_WS_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXWSDRM_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_INST_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRIS_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_DATA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRDS_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_DMA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRDMA_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_ICD_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRICD_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BUS_EXCEPTION_IXI_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXERRIXI_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_PFM_CACHE_DMA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return _BMXCON_BMXCHEDMA_LENGTH;
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDKPBA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDUDBA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDUPBA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXPUPBA_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXDRMSZ_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXPFMSZ_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _BMX_BMXBOOTSZ_LEN(BMX_MODULE_ID i)
{
    switch (i) {
        case BMX_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/bmx_ArbitrationMode_Default.h"
#include "../templates/bmx_DataRamWaitState_Default.h"
#include "../templates/bmx_ProgramFlashMemoryCacheDma_Default.h"
#include "../templates/bmx_BusExceptionIXI_Default.h"
#include "../templates/bmx_BusExceptionICD_Default.h"
#include "../templates/bmx_BusExceptionDMA_Default.h"
#include "../templates/bmx_BusExceptionData_Default.h"
#include "../templates/bmx_BusExceptionInstruction_Default.h"
#include "../templates/bmx_ProgramFlashPartition_Default.h"
#include "../templates/bmx_DataRAMPartition_Default.h"
#include "../templates/bmx_ProgramFlashMemorySize_Default.h"
#include "../templates/bmx_DataRAMSize_Default.h"
#include "../templates/bmx_ProgramFlashBootSize_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_BMX_ExistsArbitrationMode(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsArbitrationMode_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_ArbitrationModeSet(BMX_MODULE_ID index, PLIB_BMX_ARB_MODE mode)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_ArbitrationModeSet_Default(index, mode);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API PLIB_BMX_ARB_MODE PLIB_BMX_ArbitrationModeGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ArbitrationModeGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (PLIB_BMX_ARB_MODE)0;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsDataRamWaitState(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsDataRamWaitState_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_DataRamWaitStateSet(BMX_MODULE_ID index, PLIB_BMX_DATA_RAM_WAIT_STATES wait)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_DataRamWaitStateSet_Default(index, wait);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API PLIB_BMX_DATA_RAM_WAIT_STATES PLIB_BMX_DataRamWaitStateGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_DataRamWaitStateGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (PLIB_BMX_DATA_RAM_WAIT_STATES)0;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsProgramFlashMemoryCacheDma(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsProgramFlashMemoryCacheDma_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_ProgramFlashMemoryCacheDmaEnable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_ProgramFlashMemoryCacheDmaEnable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_BMX_ProgramFlashMemoryCacheDmaDisable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_ProgramFlashMemoryCacheDmaDisable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsBusExceptionIXI(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsBusExceptionIXI_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionIXIEnable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionIXIEnable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionIXIDisable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionIXIDisable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsBusExceptionICD(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsBusExceptionICD_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionICDEnable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionICDEnable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionICDDisable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionICDDisable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsBusExceptionDMA(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsBusExceptionDMA_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionDMAEnable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionDMAEnable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionDMADisable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionDMADisable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsBusExceptionData(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsBusExceptionData_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionDataEnable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionDataEnable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionDataDisable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionDataDisable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsBusExceptionInstruction(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsBusExceptionInstruction_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionInstructionEnable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionInstructionEnable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_BMX_BusExceptionInstructionDisable(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_BusExceptionInstructionDisable_Default(index);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsProgramFlashPartition(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsProgramFlashPartition_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_ProgramFlashPartitionGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ProgramFlashPartitionGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_ProgramFlashPartitionSet(BMX_MODULE_ID index, size_t user_size)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_ProgramFlashPartitionSet_Default(index, user_size);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsDataRAMPartition(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsDataRAMPartition_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_BMX_DataRAMPartitionSet(BMX_MODULE_ID index, size_t kernProgOffset, size_t userDataOffset, size_t userProgOffset)
{
    switch (index) {
        case BMX_ID_0 :
            BMX_DataRAMPartitionSet_Default(index, kernProgOffset, userDataOffset, userProgOffset);
            break;
        case BMX_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_DataRAMKernelProgramOffsetGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_DataRAMKernelProgramOffsetGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_DataRAMUserDataOffsetGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_DataRAMUserDataOffsetGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_DataRAMUserProgramOffsetGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_DataRAMUserProgramOffsetGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsProgramFlashMemorySize(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsProgramFlashMemorySize_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_ProgramFlashMemorySizeGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ProgramFlashMemorySizeGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsDataRAMSize(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsDataRAMSize_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_DataRAMSizeGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_DataRAMSizeGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

PLIB_INLINE_API bool PLIB_BMX_ExistsProgramFlashBootSize(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ExistsProgramFlashBootSize_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API size_t PLIB_BMX_ProgramFlashBootSizeGet(BMX_MODULE_ID index)
{
    switch (index) {
        case BMX_ID_0 :
            return BMX_ProgramFlashBootSizeGet_Default(index);
        case BMX_NUMBER_OF_MODULES :
        default :
            return (size_t)0;
    }
}

#endif
