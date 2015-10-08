/*******************************************************************************
  Link Layer Discovery Protocol (LLDP)

  Summary:
    Module for Microchip TCP/IP Stack

  Description:
    - LLDP implementation for Microchip TCP/IP stack
*******************************************************************************/

/*******************************************************************************
File Name:  lldp_tlv.c
Copyright © 2014 released Microchip Technology Inc.  All rights
reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND,
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

#include "tcpip/src/tcpip_private.h"
#include "lldp_private.h"
#include "lldp_tlv.h"


// proto
//

// Fixed TLVs creation
static size_t createChassisTLV(int subType, uint8_t* pBuff, size_t buffSize);
static size_t createPortTLV(int subType, uint8_t* pBuff, size_t buffSize);
static size_t createTTLTLV(int subType, uint8_t* pBuff, size_t buffSize);
static size_t createEndTLV(int subType, uint8_t* pBuff, size_t buffSize);

// Dynamic TLVs creation
static size_t createCiscoPoeTLV(int subType, uint8_t* pBuff, size_t buffSize);
static size_t createIEEEPoeTLV(int subType, uint8_t* pBuff, size_t buffSize);

static uint8_t* tl_tlvConstruct(TCPIP_LLDP_TLV_TYPE type, uint16_t tlvLen, uint8_t* pBuff);
static size_t mibConstructTlvTable(const createTLV_t* tlvEntry, int nEntries, uint8_t* pBuff, size_t buffSize); 

// TLV processing
static TCPIP_LLDP_TLV_PROCESS_RES    processEndTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType);
static TCPIP_LLDP_TLV_PROCESS_RES    processChassisIdTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType);
static TCPIP_LLDP_TLV_PROCESS_RES    processPortIdTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType);
static TCPIP_LLDP_TLV_PROCESS_RES    processTTLTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType);


static TCPIP_LLDP_TLV_PROCESS_RES    processOptionalTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType);

static TCPIP_LLDP_TLV_PROCESS_RES    processOrgSpecificTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType);

static TCPIP_LLDP_TLV_PROCESS_RES processCiscoPowerTlv(uint8_t* pData, uint16_t tlvLen, uint32_t oui, uint8_t orgSubType);
static TCPIP_LLDP_TLV_PROCESS_RES processIEEE3PowerTlv(uint8_t* pData, uint16_t tlvLen, uint32_t oui, uint8_t orgSubType);


// Helpers
static void swapEvenBytes(uint8_t *buff, uint8_t len);

// Data

// TLV processing
//

// tlv processing functions table
static const tlvProcessFuncPtr tlvProcessTbl[TCPIP_LLDP_TLV_TYPES] =
{
    processEndTlv,          // END_LLDPDU_TLV
    processChassisIdTlv,    // CHASSIS_ID_TLV
    processPortIdTlv,       // PORT_ID_TLV
    processTTLTlv,          // TIME_TO_LIVE_TLV
    processOptionalTlv,     // PORT_DESCRIPTION_TLV
    processOptionalTlv,     // SYSTEM_NAME_TLV
    processOptionalTlv,     // SYSTEM_DESCRIPTION_TLV
    processOptionalTlv,     // SYSTEM_CAPABILITIES_TLV
    processOptionalTlv,     // MANAGEMENT_ADDRESS_TLV
};



// A table to keep up with any newly added org specific TLVs
static const orgSpecificTLVs_t lldp_org_tlvTable[] = 
{
  {CISCO_OUI, (ORG_CISCO_SUBTYPE)CISCO_POWER_VIA_MDI, &processCiscoPowerTlv},
  {IEEE_3_OUI, (ORG_IEEE_3_SUBTYPE)IEEE_3_POWER_VIA_MDI, &processIEEE3PowerTlv}
};

// TLV creation

// Fixed TLVs at the beginning of any LLDPDU
// The order of these TLVs cannot be changed.
// We store the subType as part of the table for now for ease of dunction call.
// Normally each TLV should be able to manage all the subTypes by itself.
// The END_LLDPDU_TLV is added manually! 
const createTLV_t lldpCallFixedTlvTable[] =
{
//  {subType,                    createFnc}
    {CHASSIS_MAC_ADDRESS,        createChassisTLV},  // CHASSIS_ID_TLV
    {PORT_INTERFACE_NAME,        createPortTLV},     // PORT_ID_TLV
    {0,                          createTTLTLV},      // TIME_TO_LIVE_TLV
};


// End TLV at the end of any LLDPDU
const createTLV_t lldpEndTlvTable[] =
{
//  {subType,                    createFnc}
    {0,                          createEndTLV},      // END_LLDPDU_TLV
};



// Fixed Org Specific TLV creation table.
// Order of these TLVs is maintained manually for now!
// We store the subType as part of the table for now for ease of dunction call.
// Normally each TLV should be able to manage all the subTypes by itself.
const createTLV_t lldpCallOrgSpecTlvTable[] =
{
//  {subType,                   createFnc}
    {CISCO_POWER_VIA_MDI,       createCiscoPoeTLV},
    {IEEE_3_POWER_VIA_MDI,      createIEEEPoeTLV},
};

// table that gathers all the TLVs subtables
// needed for a mibContruct function
const createTLV_tbl lldpConstructTable[] = 
{
    // createTLV_t*                 // tblEntries
    {lldpCallFixedTlvTable,         sizeof(lldpCallFixedTlvTable) / sizeof(*lldpCallFixedTlvTable)},
    {lldpCallOrgSpecTlvTable,       sizeof(lldpCallOrgSpecTlvTable) / sizeof(*lldpCallOrgSpecTlvTable)},
    {lldpEndTlvTable,               sizeof(lldpEndTlvTable) / sizeof(*lldpEndTlvTable)},
};


static const TCPIP_MAC_ADDR portInterface = { {0x50, 0x4f, 0x45, 0x20, 0x50, 0x44} };


static lldp_mib_t           mib;

static orgProcessFlags    orgProcFlags;


void tlvInit(void)
{
    orgProcFlags.val = 0;
}

size_t mibConstructInfoLLDPDU(uint8_t* pBuff, size_t buffSize)
{

    const createTLV_tbl*  ctorTbl;
    int   ix;
    size_t currSize;
    size_t totalSize = 0;

    if(pBuff == 0 || buffSize == 0)
    {   // size info only
        pBuff = 0;
    }

    ctorTbl = lldpConstructTable;
    for(ix = 0; ix < sizeof(lldpConstructTable) / sizeof(*lldpConstructTable); ix++, ctorTbl++)
    {
        currSize = mibConstructTlvTable(ctorTbl->tlvTbl, ctorTbl->tblEntries, pBuff, buffSize);

        if(currSize == -1)
        {   // error/overflow
            return -1;
        }
        totalSize += currSize;
        if(pBuff)
        {
            buffSize -= currSize;
            pBuff += currSize;
        }
    }

    return totalSize;
}

// returns -1 for error
// when called with pBuff == 0 just the required size is returned
static size_t mibConstructTlvTable(const createTLV_t* tlvEntry, int nEntries, uint8_t* pBuff, size_t buffSize) 
{
    int   ix;
    size_t entryCurrSize;
    size_t tblTotalSize = 0;

    if(pBuff == 0 || buffSize == 0)
    {   // size info only
        pBuff = 0;
    }


    // start filling in the table
    for(ix = 0; ix < nEntries;  ix++, tlvEntry++)
    {
        entryCurrSize = (*tlvEntry->createFnc)(tlvEntry->subType, pBuff, buffSize);
        if(entryCurrSize == -1)
        {   // cannot build TLV/overflow
            return -1;
        }
        tblTotalSize += entryCurrSize;
        if(pBuff)
        {
            buffSize -= entryCurrSize;
            pBuff += entryCurrSize;
        }
    }

    return tblTotalSize;
}

static uint8_t* tl_tlvConstruct(TCPIP_LLDP_TLV_TYPE type, uint16_t tlvLen, uint8_t* pBuff)
{
    if(pBuff)
    {
        TCPIP_UINT16_VAL tl;

        tl.Val = type;
        tl.Val <<= 9;
        tl.Val |= tlvLen;

        *pBuff++ = tl.v[1];
        *pBuff++ = tl.v[0];
    }

    return pBuff;
}

static size_t createChassisTLV(int subType, uint8_t* pBuff, size_t buffSize)
{
    if(subType != CHASSIS_MAC_ADDRESS)
    {   // all we support for now!
        return -1;
    }

    uint16_t tlvLen = 7;    // fixed size for now
    if(pBuff)
    {
        if(buffSize < sizeof(TCPIP_LLDP_TLV) + tlvLen)
        {   // not enough room
            return -1;
        }
        pBuff = tl_tlvConstruct(CHASSIS_ID_TLV, tlvLen, pBuff); 
        *pBuff++ = subType;
        memcpy(pBuff, TCPIP_LLDP_LocalIfAddressGet(), sizeof(TCPIP_MAC_ADDR));
    }

    return sizeof(TCPIP_LLDP_TLV) + tlvLen;
}


static size_t createPortTLV(int subType, uint8_t* pBuff, size_t buffSize)
{
    if(subType != PORT_INTERFACE_NAME)
    {   // all we support for now!
        return -1;
    }

    uint16_t tlvLen = 7;    // fixed size for now
    if(pBuff)
    {
        if(buffSize < sizeof(TCPIP_LLDP_TLV) + tlvLen)
        {   // not enough room
            return -1;
        }
        pBuff = tl_tlvConstruct(PORT_ID_TLV, tlvLen, pBuff); 
        *pBuff++ = subType;
        memcpy(pBuff, portInterface.v, sizeof(portInterface));
    }

    return sizeof(TCPIP_LLDP_TLV) + tlvLen;
}

static size_t createTTLTLV(int subType, uint8_t* pBuff, size_t buffSize)
{
    // subType is irrelevant

    uint16_t tlvLen = 2;    // fixed size

    if (pBuff)
    {
        const lldp_per_port_t* pLldp_port;
        if(buffSize < sizeof(TCPIP_LLDP_TLV) + tlvLen)
        {   // not enough room
            return -1;
        }
        uint16_t ttl;
        pBuff = tl_tlvConstruct(TIME_TO_LIVE_TLV, tlvLen, pBuff); 
        pLldp_port = TCPIP_LLDP_PortGet();
        ttl = pLldp_port->tx.txTTL;
        *pBuff++ = ttl >> 8;
        *pBuff++ = ttl;
    }

    return sizeof(TCPIP_LLDP_TLV) + tlvLen;
}

static size_t createEndTLV(int subType, uint8_t* pBuff, size_t buffSize)
{
    if(pBuff)
    {
        if(buffSize < sizeof(TCPIP_LLDP_TLV))
        {   // not enough room
            return -1;
        }
        *pBuff++ = 0;
        *pBuff++ = 0;
    }

    return sizeof(TCPIP_LLDP_TLV);
}


static size_t createCiscoPoeTLV(int subType, uint8_t* pBuff, size_t buffSize)
{
    if(subType != CISCO_POWER_VIA_MDI)
    {   // all we support for now!
        return -1;
    }

    // fixed TLV data for now
    uint8_t ciscoPoeTlv[5] = { 0x00, 0x01, 0x42, 0x00, 0x05};
    uint16_t tlvLen = sizeof(ciscoPoeTlv);   

    if(pBuff)
    {
        if(buffSize < sizeof(TCPIP_LLDP_TLV) + tlvLen)
        {   // not enough room
            return -1;
        }
        pBuff = tl_tlvConstruct(ORG_SPECIFIC_TLV, tlvLen, pBuff); 
        ciscoPoeTlv[3] = (uint8_t)subType;
        if(orgProcFlags.poeEnabledPair == 1)
        {
            ciscoPoeTlv[4] = 0x0D;  // 4-wire, PD spare arch shared, PD poe on, PSE poe on
        }
        memcpy(pBuff, ciscoPoeTlv, sizeof(ciscoPoeTlv)); 
    }

    return sizeof(TCPIP_LLDP_TLV) + tlvLen;
}

static size_t createIEEEPoeTLV(int subType, uint8_t* pBuff, size_t buffSize)
{
    if(subType != IEEE_3_POWER_VIA_MDI)
    {   // all we support for now!
        return -1;
    }

    if(orgProcFlags.poeEnabledPair != 1)
    {   // nothing to set
        return 0;
    }

    // fixed TLV data for now
    //                         oui   oui   oui   sub   class pwrP  pwrC  type  reqP  reqP  allP  allP
    uint8_t ieeePoeTlv[12] = { 0x00, 0x12, 0x0f, 0x00, 0x0f, 0x01, 0x05, 0x53, 0x01, 0xfb, 0x00, 0xff};
    uint16_t tlvLen = sizeof(ieeePoeTlv);   

    if(pBuff)
    {
        if(buffSize < sizeof(TCPIP_LLDP_TLV) + tlvLen)
        {   // not enough room
            return -1;
        }
        pBuff = tl_tlvConstruct(ORG_SPECIFIC_TLV, tlvLen, pBuff); 
        ieeePoeTlv[3] = (uint8_t)subType;
        if(orgProcFlags.poeEnabledPower)
        {   // 50.5 watts are allocated
            ieeePoeTlv[10] = 0x01;
            ieeePoeTlv[11] = 0xfb;
        }
        memcpy(pBuff, ieeePoeTlv, sizeof(ieeePoeTlv)); 
    }

    return sizeof(TCPIP_LLDP_TLV) + tlvLen;
}


// TLV processing


TCPIP_LLDP_TLV_PROCESS_RES lldpProcessTlv(TCPIP_LLDP_TLV* pTlv, TCPIP_LLDP_TLV_TYPE lastType)
{
    TCPIP_LLDP_TLV_PROCESS_RES tlvRes;
    uint16_t tlvLen = pTlv->length;
    uint8_t  tlvType = pTlv->type;

    if(tlvType < TCPIP_LLDP_TLV_TYPES)
    {   // standard type
        tlvRes = (*tlvProcessTbl[tlvType])(pTlv, tlvType, tlvLen, lastType);
    }
    else if(tlvType == ORG_SPECIFIC_TLV)
    {
        tlvRes = processOrgSpecificTlv(pTlv, tlvType, tlvLen, lastType);
    } 
    else
    {
        // unhandled TLV type
        tlvRes = TLV_PROCESS_ERROR;
    }

    return tlvRes;

}




static TCPIP_LLDP_TLV_PROCESS_RES    processEndTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType)
{
    if(lastType >=  TIME_TO_LIVE_TLV && tlvLen == 0)
    {   // end of LLDPU
        return TLV_PROCESS_END;
    }

    return TLV_PROCESS_ERROR;
}

static TCPIP_LLDP_TLV_PROCESS_RES    processChassisIdTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType)
{

    if(lastType == TCPIP_LLDP_TLV_TYPE_FRAME_START && tlvLen >= 1)
    {   // valid frame
        mib.lldpdu.chassis.type = tlvType;
        mib.lldpdu.chassis.length = tlvLen;
        memcpy(mib.lldpdu.chassis.id.field, pTlv->data, tlvLen);
        swapEvenBytes((uint8_t*)&mib.lldpdu.chassis.id.ID, 6);
        
        return TLV_PROCESS_CONTINUE;
    }

    return TLV_PROCESS_ERROR;
}

static TCPIP_LLDP_TLV_PROCESS_RES    processPortIdTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType)
{

    if(lastType == CHASSIS_ID_TLV && tlvLen >= 1)
    {   // valid frame
        mib.lldpdu.port.type = tlvType;
        mib.lldpdu.port.length = tlvLen;
        memcpy(mib.lldpdu.port.id.field, pTlv->data, tlvLen);
        swapEvenBytes((uint8_t*)&mib.lldpdu.port.id.ID, 6);

        return TLV_PROCESS_CONTINUE;
    }

    return TLV_PROCESS_ERROR;
}

static TCPIP_LLDP_TLV_PROCESS_RES    processTTLTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType)
{
    if(lastType == PORT_ID_TLV && tlvLen == 2)
    {   // valid frame
        mib.lldpdu.ttl.type = tlvType;
        mib.lldpdu.ttl.length = tlvLen;
        memcpy(mib.lldpdu.ttl.t.field, pTlv->data, tlvLen);
        mib.lldpdu.ttl.t.ttlField = TCPIP_Helper_ntohs(mib.lldpdu.ttl.t.ttlField);
        mib.rxInfoTTL = (uint16_t)mib.lldpdu.ttl.t.ttlField;    // stored in reverse order

        return TLV_PROCESS_CONTINUE;
    }

    return TLV_PROCESS_ERROR;
}

static TCPIP_LLDP_TLV_PROCESS_RES    processOptionalTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType)
{
    // We are not processing any optional TLVs right now.
    // Code should be added for the ones that need to be processed
    // And the rest should be discarded

    if(lastType >= TIME_TO_LIVE_TLV )
    {   // valid frame
        return TLV_PROCESS_CONTINUE;
    }

    return TLV_PROCESS_ERROR;
}

static TCPIP_LLDP_TLV_PROCESS_RES processOrgSpecificTlv(TCPIP_LLDP_TLV* pTlv, uint8_t tlvType, uint16_t tlvLen, TCPIP_LLDP_TLV_TYPE lastType)
{
    while(lastType >= TIME_TO_LIVE_TLV && tlvLen >= 4)
    {   // valid frame
        uint32_t orgOui;
        uint8_t orgSubtype;
        int ix;
        const orgSpecificTLVs_t *hptr;
        TCPIP_LLDP_TLV_PROCESS_RES res = TLV_PROCESS_CONTINUE;

        memcpy((uint8_t*)&orgOui, pTlv->data, 3); // orgOui is 3 bytes long
        orgOui <<= 8;
        orgOui = TCPIP_Helper_ntohl(orgOui);
        orgSubtype = pTlv->data[3];

        hptr = lldp_org_tlvTable;
        for (ix = 0; ix <= sizeof(lldp_org_tlvTable) / sizeof(*lldp_org_tlvTable); ix++, hptr++)
        {
            if (hptr->oui == orgOui && hptr->subtype == orgSubtype)
            {   // found entry
                res = hptr->processOrgTlvs(pTlv->data + 4, tlvLen - 4, orgOui, orgSubtype);
                break;
            }
        }

        if(res == TLV_PROCESS_ERROR)
        {   // invalid TLV ?
            break;
        }

        return TLV_PROCESS_CONTINUE;
    }


    return TLV_PROCESS_ERROR;
}


static TCPIP_LLDP_TLV_PROCESS_RES processCiscoPowerTlv(uint8_t* pData, uint16_t len, uint32_t oui, uint8_t orgSubType)
{
    if(len == 1)
    {   // valid frame
        mib.lldpdu.cisco_4wire_poe.OUI = oui;
        mib.lldpdu.cisco_4wire_poe.type = ORG_SPECIFIC_TLV;
        mib.lldpdu.cisco_4wire_poe.length = len + 4;     // whole TLV length stored!
        mib.lldpdu.cisco_4wire_poe.subtype = orgSubType;
        mib.lldpdu.cisco_4wire_poe.capability.capabilities = *pData;
        if(mib.lldpdu.cisco_4wire_poe.capability.capabilities &0x08)
        {
           orgProcFlags.poeEnabledPair=1;
        }
        else
        {
           orgProcFlags.poeEnabledPair=0;
        }

        return TLV_PROCESS_CONTINUE;
    }

    return TLV_PROCESS_ERROR;
}

static TCPIP_LLDP_TLV_PROCESS_RES processIEEE3PowerTlv(uint8_t* pData, uint16_t len, uint32_t oui, uint8_t orgSubType)
{
    if(len == 8)
    {   // valid frame
        uint16_t reqdPower, allocPower;

        mib.lldpdu.ieee_mdi_poe.OUI = oui;
        mib.lldpdu.ieee_mdi_poe.type = ORG_SPECIFIC_TLV;
        mib.lldpdu.ieee_mdi_poe.length = len + 4;       // whole TLV length stored!
        mib.lldpdu.ieee_mdi_poe.subtype = orgSubType;
        mib.lldpdu.ieee_mdi_poe.capability.capabilities = *pData++;
        mib.lldpdu.ieee_mdi_poe.PSE_power_pair          = *pData++;
        mib.lldpdu.ieee_mdi_poe.power_class             = *pData++;
        mib.lldpdu.ieee_mdi_poe.type_src_prior.TSPval   = *pData++;

        memcpy((uint8_t*)&reqdPower, pData, 2);
        memcpy((uint8_t*)&allocPower, pData + 2, 2);
        mib.lldpdu.ieee_mdi_poe.PD_reqd_power = TCPIP_Helper_ntohs(reqdPower);
        mib.lldpdu.ieee_mdi_poe.PSE_allocated_power = TCPIP_Helper_ntohs(allocPower);

        if(mib.lldpdu.ieee_mdi_poe.PSE_allocated_power > 0x01EF)
        {
            orgProcFlags.poeEnabledPower = 1;
        }
        else
        {
           orgProcFlags.poeEnabledPower = 0;
        }
        return TLV_PROCESS_CONTINUE;
   }

    return TLV_PROCESS_ERROR;
}

bool TCPIP_LLDP_OrgProcessFlagsGet(orgProcessFlags* pFlags)
{
    if(pFlags)
    {
        pFlags->val = orgProcFlags.val;
        return true;
    }

    return false;
}

// Helpers

// length has to be even
static void swapEvenBytes(uint8_t *buff, uint8_t len)
{
    uint8_t spare, x, times;
    times = len >> 1;
    for (x = 0; x < times; x++)
    {
        spare = buff[x];
        buff[x]= buff[len-1-x];
        buff[len-1-x]=spare;
    }
}

