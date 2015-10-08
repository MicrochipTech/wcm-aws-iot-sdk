/* Created by plibgen $Revision: 1.31 $ */

#ifndef _POWER_P32MX695F512H_H
#define _POWER_P32MX695F512H_H

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

    POWER_ID_0 = 0,
    POWER_NUMBER_OF_MODULES

} POWER_MODULE_ID;

typedef enum {

    POWER_MODULE_NONE

} POWER_MODULE;

typedef enum {

    DEEP_SLEEP_MODULE_NONE

} DEEP_SLEEP_MODULE;

typedef enum {

    DEEP_SLEEP_WAKE_UP_EVENT_NONE

} DEEP_SLEEP_WAKE_UP_EVENT;

typedef enum {

    DEEP_SLEEP_EVENT_NONE

} DEEP_SLEEP_EVENT;

typedef enum {

    DEEP_SLEEP_GPR_NONE

} DEEP_SLEEP_GPR;

PLIB_INLINE SFR_TYPE* _POWER_VOLTAGE_REGULATOR_CONTROL_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_SLEEP_STATUS_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_IDLE_STATUS_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_ENABLE_CMR_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_POSTSCALAR_EXTR_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_ENABLE_SWR_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_TIMER_WDTO_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_TIMER_BOR_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _POWER_POSTSCALAR_POR_VREG(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return &RCON;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_VOLTAGE_REGULATOR_CONTROL_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_VREGS_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_SLEEP_STATUS_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_SLEEP_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_IDLE_STATUS_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_IDLE_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_ENABLE_CMR_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_CMR_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_POSTSCALAR_EXTR_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_EXTR_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_ENABLE_SWR_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_SWR_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_TIMER_WDTO_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_WDTO_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_TIMER_BOR_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_BOR_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_POSTSCALAR_POR_MASK(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_POR_MASK;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_VOLTAGE_REGULATOR_CONTROL_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_VREGS_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_SLEEP_STATUS_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_SLEEP_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_IDLE_STATUS_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_IDLE_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_ENABLE_CMR_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_CMR_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_POSTSCALAR_EXTR_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_EXTR_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_ENABLE_SWR_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_SWR_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_TIMER_WDTO_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_WDTO_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_TIMER_BOR_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_BOR_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_POSTSCALAR_POR_POS(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_POR_POSITION;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_VOLTAGE_REGULATOR_CONTROL_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_VREGS_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_SLEEP_STATUS_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_SLEEP_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_IDLE_STATUS_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_IDLE_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_ENABLE_CMR_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_CMR_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_POSTSCALAR_EXTR_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_EXTR_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_ENABLE_SWR_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_SWR_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_TIMER_WDTO_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_WDTO_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_TIMER_BOR_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_BOR_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _POWER_POSTSCALAR_POR_LEN(POWER_MODULE_ID i)
{
    switch (i) {
        case POWER_ID_0 :
            return _RCON_POR_LENGTH;
        case POWER_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/power_PeripheralModuleControl_Unsupported.h"
#include "../templates/power_VoltageRegulatorControl_Default.h"
#include "../templates/power_SleepStatus_Default.h"
#include "../templates/power_IdleStatus_Default.h"
#include "../templates/power_HighVoltageOnVDD1V8_Unsupported.h"
#include "../templates/power_DeepSleepModeOccurrence_Unsupported.h"
#include "../templates/power_DeepSleepModeControl_Unsupported.h"
#include "../templates/power_DeepSleepGPRsRetentionControl_Unsupported.h"
#include "../templates/power_DeepSleepModuleControl_Unsupported.h"
#include "../templates/power_DeepSleepWakeupEventControl_Unsupported.h"
#include "../templates/power_DeepSleepPortPinsStateControl_Unsupported.h"
#include "../templates/power_DeepSleepEventStatus_Unsupported.h"
#include "../templates/power_DeepSleepGPROperation_Unsupported.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_POWER_ExistsPeripheralModuleControl(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsPeripheralModuleControl_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_PeripheralModuleDisable(POWER_MODULE_ID index, POWER_MODULE source)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_PeripheralModuleDisable_Unsupported(index, source);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_PeripheralModuleEnable(POWER_MODULE_ID index, POWER_MODULE source)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_PeripheralModuleEnable_Unsupported(index, source);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_POWER_PeripheralModuleIsEnabled(POWER_MODULE_ID index, POWER_MODULE source)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_PeripheralModuleIsEnabled_Unsupported(index, source);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsVoltageRegulatorControl(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsVoltageRegulatorControl_Default(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_POWER_VoltageRegulatorEnable(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_VoltageRegulatorEnable_Default(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_POWER_VoltageRegulatorDisable(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_VoltageRegulatorDisable_Default(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_VoltageRegulatorIsEnabled(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_VoltageRegulatorIsEnabled_Default(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsSleepStatus(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsSleepStatus_Default(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_POWER_DeviceWasInSleepMode(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_DeviceWasInSleepMode_Default(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_POWER_ClearSleepStatus(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_ClearSleepStatus_Default(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsIdleStatus(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsIdleStatus_Default(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_POWER_DeviceWasInIdleMode(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_DeviceWasInIdleMode_Default(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_POWER_ClearIdleStatus(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_ClearIdleStatus_Default(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsHighVoltageOnVDD1V8(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsHighVoltageOnVDD1V8_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_POWER_HighVoltageOnVDD1V8HasOccurred(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_HighVoltageOnVDD1V8HasOccurred_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepModeOccurrence(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepModeOccurrence_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepModeHasOccurred(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_DeepSleepModeHasOccurred_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepStatusClear(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepStatusClear_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepMode(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepMode_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepModeEnable(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepModeEnable_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepModeDisable(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepModeDisable_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepModeIsEnabled(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_DeepSleepModeIsEnabled_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepGPRsRetentionControl(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepGPRsRetentionControl_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepGPRsRetentionEnable(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepGPRsRetentionEnable_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepGPRsRetentionDisable(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepGPRsRetentionDisable_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepModuleControl(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepModuleControl_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepModuleEnable(POWER_MODULE_ID index, DEEP_SLEEP_MODULE module)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepModuleEnable_Unsupported(index, module);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepModuleDisable(POWER_MODULE_ID index, DEEP_SLEEP_MODULE module)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepModuleDisable_Unsupported(index, module);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepWakeupEventControl(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepWakeupEventControl_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepWakeupEventEnable(POWER_MODULE_ID index, DEEP_SLEEP_WAKE_UP_EVENT event)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepWakeupEventEnable_Unsupported(index, event);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepWakeupEventDisable(POWER_MODULE_ID index, DEEP_SLEEP_WAKE_UP_EVENT event)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepWakeupEventDisable_Unsupported(index, event);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepPortPinsStateControl(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepPortPinsStateControl_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepPortPinsStateRetain(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepPortPinsStateRetain_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepPortPinsStateRelease(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepPortPinsStateRelease_Unsupported(index);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepEventStatus(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepEventStatus_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API DEEP_SLEEP_EVENT _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepEventStatusGet(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_DeepSleepEventStatusGet_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (DEEP_SLEEP_EVENT)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepEventStatusClear(POWER_MODULE_ID index, DEEP_SLEEP_EVENT event)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepEventStatusClear_Unsupported(index, event);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_POWER_ExistsDeepSleepGPROperation(POWER_MODULE_ID index)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_ExistsDeepSleepGPROperation_Unsupported(index);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepGPRWrite(POWER_MODULE_ID index, DEEP_SLEEP_GPR gpr, uint32_t data)
{
    switch (index) {
        case POWER_ID_0 :
            POWER_DeepSleepGPRWrite_Unsupported(index, gpr, data);
            break;
        case POWER_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t _PLIB_UNSUPPORTED PLIB_POWER_DeepSleepGPRRead(POWER_MODULE_ID index, DEEP_SLEEP_GPR gpr)
{
    switch (index) {
        case POWER_ID_0 :
            return POWER_DeepSleepGPRRead_Unsupported(index, gpr);
        case POWER_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

#endif
