/*******************************************************************************
  USB Host HID Keyboard driver implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_keyboard.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host HID Keyboard driver.

  Description:
    This file contains the USB host HID Keyboard driver implementation. This file
    should be included in the project if USB HID Keyboard devices are to be supported.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital  signal  controller
that is integrated into your product or third party  product  (pursuant  to  the
sublicense terms in the accompanying license agreement).

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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "usb/usb_host_hid_keyboard.h"
#include "usb/src/usb_host_hid_keyboard_local.h"
#include "system/debug/sys_debug.h"
#include "system/tmr/sys_tmr.h"
#include "usb/usb_host_hid.h"
#include <p32xxxx.h>

/* Keyboard driver information on a per instance basis */
USB_HOST_HID_KEYBOARD_DATA_OBJ keyboardData
        [USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER];
USB_HOST_HID_KEYBOARD_EVENT_HANDLER appKeyboardHandler;


// *****************************************************************************
/* Function:
    USB_HOST_HID_KEYBOARD_RESULT USB_HOST_HID_KEYBOARD_EventHandlerSet
    (
        USB_HOST_HID_KEYBOARD_EVENT_HANDLER appKeyboardEventHandler
    )
 
  Summary:
   Function registers application event handler with USB HID Keyboard driver
  
  Description:
   Function registers application event handler with USB HID Keyboard driver
  
  Remarks:
    Function registered should be of type USB_HOST_HID_KEYBOARD_EVENT_HANDLER.
*/

USB_HOST_HID_KEYBOARD_RESULT USB_HOST_HID_KEYBOARD_EventHandlerSet
(
    USB_HOST_HID_KEYBOARD_EVENT_HANDLER appKeyboardEventHandler
)
{
    if(NULL == appKeyboardEventHandler)
    {
        return USB_HOST_HID_KEYBOARD_RESULT_INVALID_PARAMETER;
    }
    else
    {
        appKeyboardHandler = appKeyboardEventHandler;
    }
    return USB_HOST_HID_KEYBOARD_RESULT_SUCCESS;
} /* End of USB_HOST_HID_KEYBOARD_EventHandlerSet() */


// *****************************************************************************
/* Function:
    void _USB_HOST_HID_KEYBOARD_EventHandler
    (
        USB_HOST_HID_OBJ_HANDLE handle,
        USB_HOST_HID_EVENT event,
        void * eventData
    )
 
  Summary:
    Keyboard driver event handler function registered with USB HID client driver
  
  Description:
    Keyboard driver event handler function registered with USB HID client driver
  
  Remarks:
    This is a local function and should not be called by application directly.
*/

void _USB_HOST_HID_KEYBOARD_EventHandler
(
    USB_HOST_HID_OBJ_HANDLE handle,
    USB_HOST_HID_EVENT event,
    void * eventData
)
{
    /* Start  of local variables */
    uint8_t loop = 0;
    /* End of local variables */
    
    if(handle != USB_HOST_HID_OBJ_HANDLE_INVALID)
    {
        switch(event)
        {
            case USB_HOST_HID_EVENT_ATTACH:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(!keyboardData[loop].inUse)
                    {
                        /* Grab the pool */                    
                        keyboardData[loop].inUse = true;
                        /* Reset necessary data structures */
                        keyboardData[loop].handle = handle;
                        keyboardData[loop].index = 0;
                        keyboardData[loop].counter = 0;
                        keyboardData[loop].state = USB_HOST_HID_KEYBOARD_ATTACHED;
                        memset(&keyboardData[loop].appData, 0,
                                sizeof(USB_HOST_HID_KEYBOARD_DATA));
                        memset(&keyboardData[loop].lastKeyCode, 0,
                                sizeof(keyboardData[loop].lastKeyCode));
                        memset((void *)keyboardData[loop].buffer, 0,
                                sizeof(keyboardData[loop].buffer));
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(appKeyboardHandler != NULL)
                    {
                        appKeyboardHandler((USB_HOST_HID_KEYBOARD_HANDLE)handle,
                                USB_HOST_HID_KEYBOARD_EVENT_ATTACH,
                                NULL);
                    }
                }
                break;
            case USB_HOST_HID_EVENT_DETACH:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(keyboardData[loop].inUse && 
                            (keyboardData[loop].handle == handle))
                    {
                        /* Release the pool object */
                        keyboardData[loop].inUse = false;
                        keyboardData[loop].state = USB_HOST_HID_KEYBOARD_DETACHED;
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(appKeyboardHandler != NULL)
                    {
                        appKeyboardHandler((USB_HOST_HID_KEYBOARD_HANDLE)handle,
                                USB_HOST_HID_KEYBOARD_EVENT_DETACH,
                                NULL);
                    }
                }
                break;
            case USB_HOST_HID_EVENT_REPORT_RECEIVED:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(keyboardData[loop].inUse && 
                            (keyboardData[loop].handle == handle))
                    {
                        /* Found the Keyboard data object */
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    memcpy((void *)keyboardData[loop].buffer[keyboardData[loop].index].data,
                                    (const void *)eventData, 64);
                    
                    keyboardData[loop].state = 
                                    USB_HOST_HID_KEYBOARD_REPORT_PROCESS;
                    keyboardData[loop].buffer[keyboardData[loop].index].tobeDone
                            = true;
                    
                    keyboardData[loop].index++;
                    if(keyboardData[loop].index ==
                            _USB_HOST_HID_KEYBOARD_BUFFER_QUEUE_SIZE)
                    {
                        /* Reset it to 0 for next iteration */
                        keyboardData[loop].index = 0;
                        keyboardData[loop].buffer[keyboardData[loop].index].tobeDone
                            = false;
                    }
                }
                break;
            default:
                break;
        }
    }
} /* End of _USB_HOST_HID_KEYBOARD_EventHandler() */


// *****************************************************************************
/* Function:
    void _USB_HOST_HID_KEYBOARD_Task(USB_HOST_HID_OBJ_HANDLE handle)
 
  Summary:
    Keyboard driver task routine function registered with USB HID client driver
  
  Description:
    Keyboard driver task routine function registered with USB HID client driver
  
  Remarks:
    This is a local function and should not be called by application directly.
*/

void _USB_HOST_HID_KEYBOARD_Task(USB_HOST_HID_OBJ_HANDLE handle)
{
    /* Start of local variables */
    USB_HOST_HID_LOCAL_ITEM localItem;
    USB_HOST_HID_GLOBAL_ITEM globalItem;
    USB_HOST_HID_MAIN_ITEM mainItem;
    
    int64_t keyboardDataBufferTemp = 0;
    int8_t * keyboardDataBuffer = NULL;
    int8_t *ptr = NULL;
    int8_t dataTemp[64];
    
    uint32_t reportOffset = 0;
    uint32_t currentReportOffsetTemp = 0;
    uint32_t usage = 0;
    uint32_t count = 0;
    
    uint8_t index = 1;
    uint8_t loop = 0;
    uint8_t i = 0;
    uint8_t keyboardIndex = 0;
    
    uint8_t counter = 0;
    bool lastKeyFound = false;
    bool tobeDone = false;
    
    USB_HOST_HID_RESULT result = USB_HOST_HID_RESULT_FAILURE;
    /* End of local variables */
    
    if(handle == USB_HOST_HID_OBJ_HANDLE_INVALID)
    {
        return;
    }
    for(keyboardIndex = 0; keyboardIndex < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        keyboardIndex++)
    {
        if(keyboardData[keyboardIndex].inUse && (keyboardData[keyboardIndex].handle == handle))
        {
            /* Found the Keyboard data object */
            break;
        }
    }
    if(keyboardIndex == USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
    {
        /* Keyboard index corresponding to the handle not found */
        return;
    }
    mainItem.globalItem = &globalItem;
    mainItem.localItem = &localItem;
    
    switch(keyboardData[keyboardIndex].state)
    {
        case USB_HOST_HID_KEYBOARD_DETACHED:
            break;
        case USB_HOST_HID_KEYBOARD_ATTACHED:
            break;
        case USB_HOST_HID_KEYBOARD_REPORT_PROCESS:
            tobeDone = false;
            counter = keyboardData[keyboardIndex].counter;
            if(counter == _USB_HOST_HID_KEYBOARD_BUFFER_QUEUE_SIZE)
            {
                keyboardData[keyboardIndex].counter = 0;
                counter = 0;
            }
            
            if(keyboardData[keyboardIndex].buffer[counter].tobeDone)
            {
                /* Keep a temp backup of the data */
                memcpy(&dataTemp,(const void *)keyboardData[keyboardIndex].buffer
                        [counter].data, 64);
                tobeDone = true;
            }
            
            if(tobeDone)
            {
                keyboardData[keyboardIndex].counter++;
                /* Reset global items only once as they are applicable through out */
                memset(&globalItem, 0,
                            (size_t)sizeof(USB_HOST_HID_GLOBAL_ITEM));
                /* Reset the app Data otherwise key count will be an issue. Also
                 data from last report if exists will lead to false key press
                 or release event */
                memset(&(keyboardData[keyboardIndex].appData), 0,
                        (size_t)sizeof(USB_HOST_HID_KEYBOARD_DATA));
                do
                {
                    /* Reset the field data except global items */
                    memset(&localItem, 0, 
                            (size_t)sizeof(USB_HOST_HID_LOCAL_ITEM));
                    memset(&(mainItem.data), 0,
                            (size_t)sizeof(USB_HID_MAIN_ITEM_OPTIONAL_DATA));
                    mainItem.tag = 0;

                    /* Start enquiring for the main items starting from
                     * index = 1. index = 0 is invalid to HID client driver. */
                    result = USB_HOST_HID_MainItemGet(handle,index,&mainItem);

                    if(result == USB_HOST_HID_RESULT_SUCCESS)
                    {
                        /* Copy the data as while processing the last field
                         we have changed the data for numbered report.*/
                        memcpy((void *)keyboardData[keyboardIndex].buffer
                                [counter].data,
                                (const void *)&dataTemp, 64);
                        
                        if(mainItem.tag ==
                                USB_HID_MAIN_ITEM_TAG_BEGIN_COLLECTION)
                        {                
                            /* Do not change report offset as they do not
                             * create data fields. Just reset the global item
                             * data */
                            memset(&globalItem, 0,
                                    (size_t)sizeof(USB_HOST_HID_GLOBAL_ITEM));
                        }/* Main item = BEGIN COLLECTION */
                        
                        else if(mainItem.tag == 
                                USB_HID_MAIN_ITEM_TAG_END_COLLECTION)
                        {
                            /* Do not change report offset as they do not
                             *  create data fields*/
                        }/* Main item = END COLLECTION */
                        
                        else if(mainItem.tag == USB_HID_MAIN_ITEM_TAG_INPUT)
                        {
                            if(((mainItem.globalItem)->reportCount == 0))
                            {
                                index++;
                                continue;
                            }

                            if(!((mainItem.globalItem)->reportID == 0))
                            {
                                /* Numbered report */
                                if(keyboardData[keyboardIndex].buffer
                                        [counter].data[0] !=
                                        (mainItem.globalItem)->reportID)
                                {
                                        /* Report ID does not match. No point in
                                           parsing this data */
                                        index++;
                                        continue;
                                }
                                /* Numbered Report. Shift right by 1 byte */
                                for(loop = 0; loop < 64; loop ++)
                                {
                                    if(loop == 63)
                                    {
                                        keyboardData[keyboardIndex].buffer
                                                [counter].data[loop] = 0;
                                        break;
                                    }
                                    keyboardData[keyboardIndex].buffer
                                            [counter].data[loop] = 
                                            keyboardData[keyboardIndex].buffer
                                            [counter].data[loop + 1];
                                    
                                }
                            } /* end of if numbered report */
                            
                            keyboardDataBuffer = (int8_t *)keyboardData[keyboardIndex].buffer
                                                    [counter].data;
                            
                            currentReportOffsetTemp = reportOffset;
                            reportOffset = reportOffset + 
                                    (mainItem.globalItem)->reportCount *
                                    (mainItem.globalItem)->reportSize;
                            
                            /* Keyboard keys handling logic*/
                            if(((0xFF00 & (mainItem.globalItem)->usagePage) 
                                    == USB_HID_USAGE_PAGE_KEYBOARD_KEYPAD) || 
                                    ((0x00FF & (mainItem.globalItem)->usagePage)
                                        == USB_HID_USAGE_PAGE_KEYBOARD_KEYPAD))
                            {
                                if(!(mainItem.data.inputOptionalData.isConstant) &&
                                        (mainItem.data.inputOptionalData.isVariable))
                                {
                                    /* Modifier byte */
                                    if(mainItem.localItem->usageMinMax.valid)
                                    {
                                        usage = mainItem.localItem->usageMinMax.min;
                                        while(usage <= 
                                                (mainItem.localItem->usageMinMax.max))
                                        {
                                            keyboardDataBuffer = (int8_t *)keyboardData[keyboardIndex].buffer
                                                    [counter].data;

                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_CONTROL)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.leftControl = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_SHIFT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.leftShift = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_ALT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.leftAlt = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_GUI)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.leftGui = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_CONTROL)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.rightControl = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_SHIFT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.rightShift = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_ALT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.rightAlt = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            if((0x00FF & usage) == USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_GUI)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + currentReportOffsetTemp/8;
                                                keyboardData[keyboardIndex].appData.modifierKeysData.rightGui = 
                                                    (bool)(((*keyboardDataBuffer) >> (currentReportOffsetTemp % 8))
                                                            & 0x01);
                                            }
                                            /* Move to the next usage */
                                            usage++;
                                            /* Update the report offset */
                                            currentReportOffsetTemp = 
                                                currentReportOffsetTemp + 
                                                (mainItem.globalItem)->reportSize;
                                        } /* end of while(all usages) */
                                    } /* Usage Min Max present */
                                } /* end of if Modifier bytes */

                                else if(!(mainItem.data.inputOptionalData.isConstant)
                                        && !(mainItem.data.inputOptionalData.isVariable))
                                {
                                    /* Non Modifier keys */
                                    
                                    /* Starting from currentReportOffsetTemp compare
                                     report size of data for report count times.
                                     If present in current it is KEY PRESS event.
                                     If it is present in past but not present in
                                     current, it is KEY RELEASE */
                                    
                                    do
                                    {
                                        count++;
                                        ptr = keyboardDataBuffer + currentReportOffsetTemp/8;
                                        
                                        keyboardDataBufferTemp = 0;
                                        if((mainItem.globalItem)->reportSize >= 8)
                                        {
                                            for (i = 0; i < (mainItem.globalItem)->reportSize/8; i++)
                                            {
                                                keyboardDataBufferTemp |= ptr[i] << (i * 8);
                                            }
                                            /* The reason why we do this is we
                                             are copying some bits extra. Hence
                                             now we need to remove those bits*/
                                            keyboardDataBufferTemp >>= (currentReportOffsetTemp % 8);
                                            keyboardDataBufferTemp &= 0xFFFFFFFF;
                                        }
                                        else
                                        {
                                            keyboardDataBufferTemp |= ptr[0];
                                            keyboardDataBufferTemp = 
                                                    keyboardDataBufferTemp << (8 - (mainItem.globalItem)->reportSize);
                                            keyboardDataBufferTemp =
                                                    keyboardDataBufferTemp >> (8 - (mainItem.globalItem)->reportSize);

                                        }
                                        if((keyboardDataBufferTemp != USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED)
                                                &&(keyboardDataBufferTemp != USB_HID_KEYBOARD_KEYPAD_KEYBOARD_ERROR_ROLL_OVER)
                                                &&(keyboardDataBufferTemp != USB_HID_KEYBOARD_KEYPAD_KEYBOARD_POST_FAIL)
                                                &&(keyboardDataBufferTemp != USB_HID_KEYBOARD_KEYPAD_KEYBOARD_ERROR_UNDEFINED))
                                        {
                                            /* Valid key press detected */
                                            keyboardData[keyboardIndex].appData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].appData.nNonModifierKeysData].keyCode
                                                    = (USB_HID_KEYBOARD_KEYPAD) keyboardDataBufferTemp;
                                            keyboardData[keyboardIndex].appData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].appData.nNonModifierKeysData].event
                                                    = USB_HID_KEY_PRESSED;
                                            
                                            keyboardData[keyboardIndex].appData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].appData.nNonModifierKeysData].sysCount
                                                    = SYS_TMR_SystemCountGet();
                                            keyboardData[keyboardIndex].appData.nNonModifierKeysData++;
                                        }
                                    
                                        /* Update the report offset */
                                        currentReportOffsetTemp = 
                                            currentReportOffsetTemp + 
                                            (mainItem.globalItem)->reportSize;
                                        
                                    } while(count < (mainItem.globalItem)->reportCount);
                                    
                                    for(count = 0; count < 6; count++)
                                    {
                                        /* If it is present in the past
                                         but not in current, it is key release */
                                        if(keyboardData[keyboardIndex].lastKeyCode[count]  >
                                                USB_HID_KEYBOARD_KEYPAD_KEYBOARD_ERROR_UNDEFINED)
                                        {
                                            for(i=0; i < 6; i++)
                                            {
                                                if(keyboardData[keyboardIndex].appData.nonModifierKeysData[i].keyCode
                                                        == keyboardData[keyboardIndex].lastKeyCode[count])
                                                {
                                                    lastKeyFound = true;
                                                    break;
                                                }
                                            }
                                            if(lastKeyFound == false)
                                            {
                                                keyboardData[keyboardIndex].appData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].appData.nNonModifierKeysData].keyCode
                                                    = (USB_HID_KEYBOARD_KEYPAD)keyboardData[keyboardIndex].lastKeyCode[count];
                                                keyboardData[keyboardIndex].appData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].appData.nNonModifierKeysData].event
                                                    = USB_HID_KEY_RELEASED;
                                                keyboardData[keyboardIndex].appData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].appData.nNonModifierKeysData].event
                                                    = SYS_TMR_SystemCountGet();
                                                keyboardData[keyboardIndex].appData.nNonModifierKeysData++;
                                            }
                                            else
                                            {
                                                /* Reset the flag to false for
                                                 next iteration */
                                                lastKeyFound = false;
                                            }
                                        }
                                    }
                                    for(count = 0; count < 6; count++)
                                    {
                                        /* Save the present key state for next
                                         * processing */
                                        keyboardData[keyboardIndex].lastKeyCode[count] = 0;
                                        if(keyboardData[keyboardIndex].appData
                                                .nonModifierKeysData[count].event == USB_HID_KEY_PRESSED)
                                        {
                                            keyboardData[keyboardIndex].lastKeyCode[count] =
                                                keyboardData[keyboardIndex].appData.nonModifierKeysData[count].keyCode;
                                        }
                                    }
                                }

                            } /* Keyboard/Keypad page */
                        }/* Main item = INPUT */

                        else if(mainItem.tag ==
                                    USB_HID_MAIN_ITEM_TAG_OUTPUT)
                        {
                            /* Change output report offset as they
                             * create data fields*/
                        }
                        else if(mainItem.tag ==
                                    USB_HID_MAIN_ITEM_TAG_FEATURE)
                        {
                            /* Change feature report offset as they
                             * create data fields*/
                        }
                    }/* Field found */

                    index++;
                    
                } while(result == USB_HOST_HID_RESULT_SUCCESS);
                
                keyboardData[keyboardIndex].buffer[counter].tobeDone = false;
                
                if(appKeyboardHandler != NULL)
                {
                    appKeyboardHandler((USB_HOST_HID_KEYBOARD_HANDLE)handle,
                                USB_HOST_HID_KEYBOARD_EVENT_REPORT_RECEIVED,
                                &keyboardData[keyboardIndex].appData);
                }

            }/* end of report processing */
            break;
        default:
            break;
    }
}/* End of _USB_HOST_HID_KEYBOARD_Task() */

