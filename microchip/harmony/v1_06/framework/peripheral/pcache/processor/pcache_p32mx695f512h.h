/* Created by plibgen $Revision: 1.31 $ */

#ifndef _PCACHE_P32MX695F512H_H
#define _PCACHE_P32MX695F512H_H

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

    PCACHE_ID_0 = 0,
    PCACHE_NUMBER_OF_MODULES

} PCACHE_MODULE_ID;

typedef enum {

    PLIB_PCACHE_PREFETCH_DISABLE = 0x00,
    PLIB_PCACHE_PREFETCH_ENABLE_CACHED_REGIONS = 0x01,
    PLIB_PCACHE_PREFETCH_ENABLE_NONCACHED_REGIONS = 0x02,
    PLIB_PCACHE_PREFETCH_ENABLE_ALL = 0x03

} PLIB_PCACHE_PREFETCH_ENABLE;

typedef enum {

    PLIB_PCACHE_DATA_DISABLE = 0x00,
    PLIB_PCACHE_DATA_1LINE = 0x01,
    PLIB_PCACHE_DATA_2LINE = 0x02,
    PLIB_PCACHE_DATA_4LINE = 0x03

} PLIB_PCACHE_DATA_ENABLE;

PLIB_INLINE SFR_TYPE* _PCACHE_WAIT_STATE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHECON;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_PREFETCH_ENABLE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHECON;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_DATA_CACHE_ENABLE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHECON;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_INVALIDATE_ON_PFM_PROGRAM_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHECON;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_INDEX_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEACC;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_ENABLE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEACC;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_TYPE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHETAG;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_LOCK_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHETAG;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_VALID_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHETAG;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_ADDR_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHETAG;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_FLASH_TYPE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHETAG;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_LINE_MASK_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEMSK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_WORD0_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEW0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_WORD1_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEW1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_WORD2_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEW2;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_WORD3_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEW3;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_LEAST_RECENTLY_USED_STATE_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHELRU;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_HIT_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEHIT;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_CACHE_MISS_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEMIS;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_TYPE* _PCACHE_PREFETCH_ABORT_VREG(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return &CHEPFABT;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_TYPE*)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WAIT_STATE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_PFMWS_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_PREFETCH_ENABLE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_PREFEN_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_DATA_CACHE_ENABLE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_DCSZ_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_INVALIDATE_ON_PFM_PROGRAM_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_CHECOH_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_INDEX_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHEACC_CHEIDX_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_ENABLE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHEACC_CHEWEN_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_TYPE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTYPE_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_LOCK_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LLOCK_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_VALID_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LVALID_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_ADDR_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTAG_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_FLASH_TYPE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTAGBOOT_MASK;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_MASK_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD0_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD1_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD2_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD3_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_LEAST_RECENTLY_USED_STATE_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_HIT_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_MISS_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_PREFETCH_ABORT_MASK(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)-1;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WAIT_STATE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_PFMWS_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_PREFETCH_ENABLE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_PREFEN_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_DATA_CACHE_ENABLE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_DCSZ_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_INVALIDATE_ON_PFM_PROGRAM_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_CHECOH_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_INDEX_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHEACC_CHEIDX_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_ENABLE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHEACC_CHEWEN_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_TYPE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTYPE_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_LOCK_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LLOCK_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_VALID_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LVALID_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_ADDR_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTAG_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_FLASH_TYPE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTAGBOOT_POSITION;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_MASK_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD0_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD1_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD2_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD3_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_LEAST_RECENTLY_USED_STATE_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_HIT_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_MISS_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_PREFETCH_ABORT_POS(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)0;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WAIT_STATE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_PFMWS_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_PREFETCH_ENABLE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_PREFEN_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_DATA_CACHE_ENABLE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_DCSZ_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_INVALIDATE_ON_PFM_PROGRAM_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHECON_CHECOH_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_INDEX_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHEACC_CHEIDX_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_ENABLE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHEACC_CHEWEN_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_TYPE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTYPE_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_LOCK_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LLOCK_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_VALID_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LVALID_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_ADDR_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTAG_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_FLASH_TYPE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return _CHETAG_LTAGBOOT_LENGTH;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_LINE_MASK_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD0_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD1_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD2_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_WORD3_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_LEAST_RECENTLY_USED_STATE_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_HIT_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_CACHE_MISS_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

PLIB_INLINE SFR_DATA _PCACHE_PREFETCH_ABORT_LEN(PCACHE_MODULE_ID i)
{
    switch (i) {
        case PCACHE_ID_0 :
            return (SFR_DATA)sizeof(SFR_DATA);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (SFR_DATA)-1;
    }
}

/* Section 2 - Feature variant inclusion */

#define PLIB_TEMPLATE PLIB_INLINE
#include "../templates/pcache_WaitState_Default.h"
#include "../templates/pcache_PrefetchEnable_Default.h"
#include "../templates/pcache_DataCacheEnable_Default.h"
#include "../templates/pcache_FlashSECInt_Unsupported.h"
#include "../templates/pcache_FlashDEDStatus_Unsupported.h"
#include "../templates/pcache_FlashSECStatus_Unsupported.h"
#include "../templates/pcache_FlashSECCount_Unsupported.h"
#include "../templates/pcache_InvalidateOnPFMProgram_Default.h"
#include "../templates/pcache_CacheLineSelect_Default.h"
#include "../templates/pcache_CacheLineType_Default.h"
#include "../templates/pcache_CacheLineLock_Default.h"
#include "../templates/pcache_CacheLineValid_Default.h"
#include "../templates/pcache_CacheLineAddr_Default.h"
#include "../templates/pcache_CacheLineFlashType_Default.h"
#include "../templates/pcache_CacheLineMask_Default.h"
#include "../templates/pcache_Word_Default.h"
#include "../templates/pcache_LeastRecentlyUsedState_Default.h"
#include "../templates/pcache_CacheHit_Default.h"
#include "../templates/pcache_CacheMiss_Default.h"
#include "../templates/pcache_PrefetchAbort_Default.h"

/* Section 3 - PLIB dispatch function definitions */

PLIB_INLINE_API bool PLIB_PCACHE_ExistsWaitState(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsWaitState_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_WaitStateSet(PCACHE_MODULE_ID index, uint32_t clocks)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_WaitStateSet_Default(index, clocks);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_WaitStateGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_WaitStateGet_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsPrefetchEnable(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsPrefetchEnable_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_PrefetchEnableSet(PCACHE_MODULE_ID index, PLIB_PCACHE_PREFETCH_ENABLE region)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_PrefetchEnableSet_Default(index, region);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API PLIB_PCACHE_PREFETCH_ENABLE PLIB_PCACHE_PrefetchEnableGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_PrefetchEnableGet_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (PLIB_PCACHE_PREFETCH_ENABLE)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsDataCacheEnable(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsDataCacheEnable_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_DataCacheEnableSet(PCACHE_MODULE_ID index, PLIB_PCACHE_DATA_ENABLE dcache_en)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_DataCacheEnableSet_Default(index, dcache_en);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API PLIB_PCACHE_DATA_ENABLE PLIB_PCACHE_DataCacheEnableGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_DataCacheEnableGet_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (PLIB_PCACHE_DATA_ENABLE)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsFlashSECInt(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsFlashSECInt_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECIntEnable(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_FlashSECIntEnable_Unsupported(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECIntDisable(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_FlashSECIntDisable_Unsupported(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsFlashDEDStatus(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsFlashDEDStatus_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_PCACHE_FlashDEDStatusGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_FlashDEDStatusGet_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_PCACHE_FlashDEDStatusClear(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_FlashDEDStatusClear_Unsupported(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsFlashSECStatus(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsFlashSECStatus_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECStatusGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_FlashSECStatusGet_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECStatusSet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_FlashSECStatusSet_Unsupported(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECStatusClear(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_FlashSECStatusClear_Unsupported(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsFlashSECCount(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsFlashSECCount_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECCountSet(PCACHE_MODULE_ID index, uint8_t count)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_FlashSECCountSet_Unsupported(index, count);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint8_t _PLIB_UNSUPPORTED PLIB_PCACHE_FlashSECCountGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_FlashSECCountGet_Unsupported(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint8_t)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsInvalidateOnPFMProgram(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsInvalidateOnPFMProgram_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_InvalidateOnPFMProgramAll(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_InvalidateOnPFMProgramAll_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_InvalidateOnPFMProgramUnlocked(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_InvalidateOnPFMProgramUnlocked_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLine(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLine_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineSelect(PCACHE_MODULE_ID index, uint32_t cache_line)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineSelect_Default(index, cache_line);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineDeselect(PCACHE_MODULE_ID index, uint32_t cache_line)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineDeselect_Default(index, cache_line);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineData(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineData_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineInst(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineInst_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_CacheLineIsInst(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheLineIsInst_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLineType(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLineType_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineLock(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineLock_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineUnlock(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineUnlock_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_CacheLineIsLocked(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheLineIsLocked_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLineLock(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLineLock_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineValid(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineValid_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineInvalid(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineInvalid_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_CacheLineIsValid(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheLineIsValid_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLineValid(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLineValid_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineAddrSet(PCACHE_MODULE_ID index, uint32_t addr)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineAddrSet_Default(index, addr);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_CacheLineAddrGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheLineAddrGet_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLineAddr(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLineAddr_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineFlashTypeBoot(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineFlashTypeBoot_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineFlashTypeInst(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineFlashTypeInst_Default(index);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_CacheLineFlashTypeIsInst(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheLineFlashTypeIsInst_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLineFlashType(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLineFlashType_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheLineMaskSet(PCACHE_MODULE_ID index, uint32_t mask)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheLineMaskSet_Default(index, mask);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_CacheLineMaskGet(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheLineMaskGet_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheLineMask(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheLineMask_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_WordRead(PCACHE_MODULE_ID index, uint32_t word)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_WordRead_Default(index, word);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_WordWrite(PCACHE_MODULE_ID index, uint32_t word, uint32_t data)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_WordWrite_Default(index, word, data);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsWord(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsWord_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_LeastRecentlyUsedStateRead(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_LeastRecentlyUsedStateRead_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsLeastRecentlyUsedState(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsLeastRecentlyUsedState_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_CacheHitRead(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheHitRead_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheHitWrite(PCACHE_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheHitWrite_Default(index, data);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheHit(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheHit_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_CacheMissRead(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_CacheMissRead_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_CacheMissWrite(PCACHE_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_CacheMissWrite_Default(index, data);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsCacheMiss(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsCacheMiss_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

PLIB_INLINE_API uint32_t PLIB_PCACHE_PrefetchAbortRead(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_PrefetchAbortRead_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (uint32_t)0;
    }
}

PLIB_INLINE_API void PLIB_PCACHE_PrefetchAbortWrite(PCACHE_MODULE_ID index, uint32_t data)
{
    switch (index) {
        case PCACHE_ID_0 :
            PCACHE_PrefetchAbortWrite_Default(index, data);
            break;
        case PCACHE_NUMBER_OF_MODULES :
        default :
            break;
    }
}

PLIB_INLINE_API bool PLIB_PCACHE_ExistsPrefetchAbort(PCACHE_MODULE_ID index)
{
    switch (index) {
        case PCACHE_ID_0 :
            return PCACHE_ExistsPrefetchAbort_Default(index);
        case PCACHE_NUMBER_OF_MODULES :
        default :
            return (bool)0;
    }
}

#endif
