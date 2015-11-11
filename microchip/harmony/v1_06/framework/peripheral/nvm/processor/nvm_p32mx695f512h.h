/* Created by plibgen $Revision: 1.31 $ */

#ifndef _NVM_P32MX695F512H_H
#define _NVM_P32MX695F512H_H

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

    NVM_ID_0 = 0,
    NVM_NUMBER_OF_MODULES

} NVM_MODULE_ID;

typedef enum {

    WORD_PROGRAM_OPERATION = 0x1,
    ROW_PROGRAM_OPERATION = 0x3,
    PAGE_ERASE_OPERATION = 0x4,
    FLASH_ERASE_OPERATION = 0x5,
    NO_OPERATION = 0x0

} NVM_OPERATION_MODE;

typedef enum {

    NVM_BOOT_MEMORY_AREA_NONE

} NVM_BOOT_MEMORY_AREA;

typedef enum {

    NVM_BOOT_MEMORY_PAGE_NONE

} NVM_BOOT_MEMORY_PAGE;

typedef enum {

    NVM_FLASH_SWAP_LOCK_TYPE_NONE

} NVM_FLASH_SWAP_LOCK_TYPE;

typedef enum {

    EEPROM_OPERATION_MODE_NONE

} EEPROM_OPERATION_MODE;

typedef enum {

    EEPROM_ERROR_NONE

} EEPROM_ERROR;

PLIB_INLINE SFR_TYPE* _NVM_OPERATION_MODE_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMCON;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_LOW_VOLTAGE_STATUS_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMCON;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_LOW_VOLTAGE_ERROR_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMCON;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_WRITE_ERROR_STATUS_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMCON;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_MEMORY_MODIFY_CONTROL_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMCON;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_WRITE_OPERATION_CONTROL_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMCON;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_KEY_SEQUENCE_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMKEY;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_ADDRESS_MODIFY_CONTROL_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMADDR;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_PROVIDE_DATA_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMDATA;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _NVM_SOURCE_ADDRESS_SELECT_VREG(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return &NVMSRCADDR;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_OPERATION_MODE_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_NVMOP_MASK;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_LOW_VOLTAGE_STATUS_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_LVDSTAT_MASK;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_LOW_VOLTAGE_ERROR_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_LVDERR_MASK;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_WRITE_ERROR_STATUS_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WRERR_MASK;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_MEMORY_MODIFY_CONTROL_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WREN_MASK;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_WRITE_OPERATION_CONTROL_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WR_MASK;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_KEY_SEQUENCE_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)-1;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_ADDRESS_MODIFY_CONTROL_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)-1;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_PROVIDE_DATA_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)-1;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_SOURCE_ADDRESS_SELECT_MASK(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)-1;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_OPERATION_MODE_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_NVMOP_POSITION;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_LOW_VOLTAGE_STATUS_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_LVDSTAT_POSITION;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_LOW_VOLTAGE_ERROR_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_LVDERR_POSITION;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_WRITE_ERROR_STATUS_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WRERR_POSITION;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_MEMORY_MODIFY_CONTROL_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WREN_POSITION;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_WRITE_OPERATION_CONTROL_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WR_POSITION;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_KEY_SEQUENCE_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)0;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_ADDRESS_MODIFY_CONTROL_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)0;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_PROVIDE_DATA_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)0;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_SOURCE_ADDRESS_SELECT_POS(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)0;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_OPERATION_MODE_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_NVMOP_LENGTH;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_LOW_VOLTAGE_STATUS_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_LVDSTAT_LENGTH;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_LOW_VOLTAGE_ERROR_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_LVDERR_LENGTH;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_WRITE_ERROR_STATUS_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WRERR_LENGTH;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_MEMORY_MODIFY_CONTROL_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WREN_LENGTH;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_WRITE_OPERATION_CONTROL_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return _NVMCON_WR_LENGTH;
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_KEY_SEQUENCE_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_ADDRESS_MODIFY_CONTROL_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_PROVIDE_DATA_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _NVM_SOURCE_ADDRESS_SELECT_LEN(NVM_MODULE_ID i)
{
    switch (i) {
        case NVM_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/nvm_WriteErrorStatus_Default.h"
#include "../templates/nvm_MemoryModificationControl_Default.h"
#include "../templates/nvm_OperationMode_Default.h"
#include "../templates/nvm_AddressModifyControl_Default.h"
#include "../templates/nvm_ProvideData_Default.h"
#include "../templates/nvm_WriteOperation_Default.h"
#include "../templates/nvm_KeySequence_Default.h"
#include "../templates/nvm_SourceAddress_Default.h"
#include "../templates/nvm_LowVoltageStatus_Default.h"
#include "../templates/nvm_LowVoltageError_Default.h"
#include "../templates/nvm_FlashBankRegionSelect_Unsupported.h"
#include "../templates/nvm_BootFlashBankRegion_Unsupported.h"
#include "../templates/nvm_ProvideQuadData_Unsupported.h"
#include "../templates/nvm_LockPFMSelect_Unsupported.h"
#include "../templates/nvm_FlashWPMemoryRangeProvide_Unsupported.h"
#include "../templates/nvm_LockBootSelect_Unsupported.h"
#include "../templates/nvm_BootPageWriteProtect_Unsupported.h"
#include "../templates/nvm_SwapLockControl_Unsupported.h"
#include "../templates/nvm_EEPROMEnableControl_Unsupported.h"
#include "../templates/nvm_EEPROMStopInIdleControl_Unsupported.h"
#include "../templates/nvm_EEPROMOperationModeControl_Unsupported.h"
#include "../templates/nvm_EEPROMAddressControl_Unsupported.h"
#include "../templates/nvm_EEPROMDataControl_Unsupported.h"
#include "../templates/nvm_EEPROMKeySequence_Unsupported.h"
#include "../templates/nvm_EEPROMEnableOperationControl_Unsupported.h"
#include "../templates/nvm_EEPROMStartOperationControl_Unsupported.h"
#include "../templates/nvm_EEPROMLongWriteStatus_Unsupported.h"
#include "../templates/nvm_EEPROMOperationAbortControl_Unsupported.h"
#include "../templates/nvm_EEPROMErrorStatus_Unsupported.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_NVM_ExistsWriteErrorStatus(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsWriteErrorStatus_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_WriteOperationHasTerminated(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_WriteOperationHasTerminated_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsMemoryModificationControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsMemoryModificationControl_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_MemoryModifyEnable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_MemoryModifyEnable_Default(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_NVM_MemoryModifyInhibit(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_MemoryModifyInhibit_Default(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsOperationMode(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsOperationMode_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_MemoryOperationSelect(NVM_MODULE_ID index, NVM_OPERATION_MODE operationmode)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_MemoryOperationSelect_Default(index, operationmode);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsAddressModifyControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsAddressModifyControl_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_FlashAddressToModify(NVM_MODULE_ID index, uint32_t address)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashAddressToModify_Default(index, address);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsProvideData(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsProvideData_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_FlashProvideData(NVM_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashProvideData_Default(index, data);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsWriteOperation(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsWriteOperation_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_NVM_FlashRead(NVM_MODULE_ID index, uint32_t address)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_FlashRead_Default(index, address);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_FlashWriteStart(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashWriteStart_Default(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_NVM_FlashEraseStart(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashEraseStart_Default(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_FlashWriteCycleHasCompleted(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_FlashWriteCycleHasCompleted_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsKeySequence(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsKeySequence_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_FlashWriteKeySequence(NVM_MODULE_ID index, uint32_t keysequence)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashWriteKeySequence_Default(index, keysequence);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsSourceAddress(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsSourceAddress_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_NVM_DataBlockSourceAddress(NVM_MODULE_ID index, uint32_t address)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_DataBlockSourceAddress_Default(index, address);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsLowVoltageStatus(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsLowVoltageStatus_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_LowVoltageEventIsActive(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_LowVoltageEventIsActive_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsLowVoltageError(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsLowVoltageError_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_LowVoltageIsDetected(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_LowVoltageIsDetected_Default(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsFlashBankRegionSelect(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsFlashBankRegionSelect_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_ProgramFlashBank1LowerRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_ProgramFlashBank1LowerRegion_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_ProgramFlashBank2LowerRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_ProgramFlashBank2LowerRegion_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_ProgramFlashBank2IsLowerRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ProgramFlashBank2IsLowerRegion_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsBootFlashBankRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsBootFlashBankRegion_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_BootFlashBank1LowerRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_BootFlashBank1LowerRegion_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_BootFlashBank2LowerRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_BootFlashBank2LowerRegion_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_BootFlashBank2IsLowerRegion(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_BootFlashBank2IsLowerRegion_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsProvideQuadData(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsProvideQuadData_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_FlashProvideQuadData(NVM_MODULE_ID index, uint32_t* data)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashProvideQuadData_Unsupported(index, data);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsLockPFMSelect(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsLockPFMSelect_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_LockProgramFlashMemory(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_LockProgramFlashMemory_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_IsProgramFlashMemoryLocked(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_IsProgramFlashMemoryLocked_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsFlashWPMemoryRangeProvide(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsFlashWPMemoryRangeProvide_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_FlashWriteProtectMemoryAreaRange(NVM_MODULE_ID index, uint32_t address)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashWriteProtectMemoryAreaRange_Unsupported(index, address);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsLockBootSelect(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsLockBootSelect_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_LockBootMemory(NVM_MODULE_ID index, NVM_BOOT_MEMORY_AREA memoryArea)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_LockBootMemory_Unsupported(index, memoryArea);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_IsBootMemoryLocked(NVM_MODULE_ID index, NVM_BOOT_MEMORY_AREA memoryArea)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_IsBootMemoryLocked_Unsupported(index, memoryArea);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsBootPageWriteProtect(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsBootPageWriteProtect_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_BootPageWriteProtectionEnable(NVM_MODULE_ID index, NVM_BOOT_MEMORY_PAGE bootPage)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_BootPageWriteProtectionEnable_Unsupported(index, bootPage);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_BootPageWriteProtectionDisable(NVM_MODULE_ID index, NVM_BOOT_MEMORY_PAGE bootPage)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_BootPageWriteProtectionDisable_Unsupported(index, bootPage);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_IsBootPageWriteProtected(NVM_MODULE_ID index, NVM_BOOT_MEMORY_PAGE bootPage)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_IsBootPageWriteProtected_Unsupported(index, bootPage);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsSwapLockControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsSwapLockControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_FlashSwapLockSelect(NVM_MODULE_ID index, NVM_FLASH_SWAP_LOCK_TYPE lockType)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_FlashSwapLockSelect_Unsupported(index, lockType);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API NVM_FLASH_SWAP_LOCK_TYPE _PLIB_UNSUPPORTED PLIB_NVM_FlashSwapLockStatusGet(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_FlashSwapLockStatusGet_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (NVM_FLASH_SWAP_LOCK_TYPE)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMEnableControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMEnableControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMEnable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMEnable_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMDisable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMDisable_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMIsReady(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMIsReady_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMStopInIdleControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMStopInIdleControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMStopInIdleEnable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMStopInIdleEnable_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMStopInIdleDisable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMStopInIdleDisable_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMStopInIdleIsEnabled(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMStopInIdleIsEnabled_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMOperationModeControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMOperationModeControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMOperationSelect(NVM_MODULE_ID index, EEPROM_OPERATION_MODE mode)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMOperationSelect_Unsupported(index, mode);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMAddressControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMAddressControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMAddress(NVM_MODULE_ID index, uint32_t address)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMAddress_Unsupported(index, address);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMDataControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMDataControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMDataToWrite(NVM_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMDataToWrite_Unsupported(index, data);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t _PLIB_UNSUPPORTED PLIB_NVM_EEPROMRead(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMRead_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMKeySequence(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMKeySequence_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMKeySequenceWrite(NVM_MODULE_ID index, uint32_t keysequence)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMKeySequenceWrite_Unsupported(index, keysequence);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMEnableOperationControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMEnableOperationControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMWriteEnable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMWriteEnable_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMWriteIsEnabled(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMWriteIsEnabled_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMReadEnable(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMReadEnable_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMReadIsEnabled(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMReadIsEnabled_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMStartOperationControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMStartOperationControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMReadStart(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMReadStart_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMWriteStart(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMWriteStart_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMEraseStart(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMEraseStart_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMOperationHasCompleted(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMOperationHasCompleted_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMLongWriteStatus(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMLongWriteStatus_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_NVM_EEPROMNextWriteCycleIsLong(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMNextWriteCycleIsLong_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMOperationAbortControl(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMOperationAbortControl_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMOperationAbort(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMOperationAbort_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_NVM_ExistsEEPROMErrorStatus(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_ExistsEEPROMErrorStatus_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API EEPROM_ERROR _PLIB_UNSUPPORTED PLIB_NVM_EEPROMErrorGet(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            return NVM_EEPROMErrorGet_Unsupported(index);
        case NVM_NUMBER_OF_MODULES :
        default :
            return (EEPROM_ERROR)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_NVM_EEPROMErrorClear(NVM_MODULE_ID index)
{
    switch (index) {
        case NVM_ID_0 :
            NVM_EEPROMErrorClear_Unsupported(index);
            break;
        case NVM_NUMBER_OF_MODULES :
        default :
            break;
    }
}

#endif
