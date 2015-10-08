/*******************************************************************************
 USART Driver Configuration Template

  Company:
    Microchip Technology Inc.

  File Name:
    drv_usart_config_template.h

  Summary:
    USART Driver Configuration Template.

  Description:
    These file provides the list of all the configurations that can be used with
    the driver. This file should not be included in the driver.

*******************************************************************************/
//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2012 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _DRV_USART_CONFIG_TEMPLATE_H
#define _DRV_USART_CONFIG_TEMPLATE_H

//DOM-IGNORE-BEGIN
#error "This is a configuration template file.  Do not include it directly."
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Core Functionality Configuration Options
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USART driver objects configuration

  Summary
    Sets up the maximum number of hardware instances that can be supported

  Description
    Sets up the maximum number of hardware instances that can be supported.
    It is recommended that this number be set exactly equal to the number of
    USART modules that are needed by the application. Hardware Instance
    support consumes RAM memory space. 
    If this macro is not defined, then the driver will be built statically.

  Remarks:
    None
*/

#define DRV_USART_INSTANCES_NUMBER  2

// *****************************************************************************
/* USART Client Count Configuration

  Summary
    Sets up the maximum number of clients that can be connected to any hardware
    instance.

  Description
    Sets up the maximum number of clients that can be connected to any hardware
    instance. This value represents the total number of clients to be supported
    across all hardware instances. So if USART1 will be accessed by 2 clients
    and USART2 will accessed by 3 clients, then this number should be 5. 
    It is recommended that this be set exactly equal to the number of expected
    clients. Client support consumes RAM memory space.
    If this macro is not defined and the DRV_USART_INSTANCES_NUMBER macro is not
    defined, then the driver will be built for static - single client operation.
    If this macro is defined and the DRV_USART_INSTANCES_NUMBER macro is not
    defined, then the driver will be built for static - multi client operation.

  Remarks:
    None
*/

#define DRV_USART_CLIENTS_NUMBER   4

// *****************************************************************************
/* USART Interrupt Mode Operation Control

  Summary:
    Macro controls interrupt based operation of the driver

  Description:
    This macro controls the interrupt based operation of the driver. The 
    possible values it can take are
    - true - Enables the interrupt mode
    - false - Enables the polling mode
    If the macro value is true, then Interrupt Service Routine for the interrupt
    should be defined in the application. The DRV_USART_Tasks() routine should
    be called in the ISR. While using the USART driver with DMA, this flag
    should always be true.

  Remarks:
    None
*/

#define DRV_USART_INTERRUPT_MODE  true

// *****************************************************************************
/* USART Driver Instance combined queue depth.

  Summary:
    Number of entries of all queues in all instances of the driver.
  
  Description:
    This macro defines the number of entries of all queues in all instances of
    the driver.
    
    Each hardware instance supports a buffer queue for transmit and receive
    operations. The size of queue is specified either in driver
    initialization (for dynamic build) or by macros (for static build). The
    hardware instance transmit buffer queue will queue transmit buffers
    submitted by the DRV_USART_BufferAddWrite() function.  The hardware instance
    receive buffer queue will queue receive buffers submitted by the
    DRV_USART_BufferAddRead() function. 
    
    A buffer queue will contain buffer queue entries, each related to a
    BufferAdd request.  This configuration macro defines total number of buffer
    entries that will be available for use between all USART driver hardware
    instances. The buffer queue entries are allocated to individual hardware
    instances as requested by hardware instances. Once the request is processed,
    the buffer queue entry is free for use by other hardware instances.

    The total number of buffer entries in the system determines the ability of
    the driver to service non blocking read and write requests. If a free buffer
    entry is not available, the driver will not add the request and will return
    an invalid buffer handle. More the number of buffer entries, greater the
    ability of the driver to service and add requests to its queue. A hardware
    instance additionally can queue up as many buffer entries as  specified by
    its transmit and receive buffer queue size. 

    As an example, consider the case of static single client driver application
    where full duplex non blocking operation is desired without queuing, the
    minimum transmit queue depth and minimum receive queue depth should be 1.
    Hence the total number of buffer entries should be 2.
   
    As an example, consider the case of a dynamic driver (say 2 instances) where
    instance 1 will queue up to 3 write requests and up to 2 read requests, and
    instance 2 will queue up to 2 write requests and up to 6 read requests, the
    value of this macro should be 13 (2 + 3 + 2 + 6).

  Remarks:
    None
*/

#define  DRV_USART_QUEUE_DEPTH_COMBINED   16

// *****************************************************************************
/* USART Driver Buffer Queue Support 

  Summary:
    Specifies if the Buffer Queue support should be enabled. 
  
  Description:
    This macro defines if Buffer Queue support should be enabled. Setting this
    macro to true will enable buffer queue support and all buffer related driver
    function. The driver should be built along with the drv_usart_buffer_queue.c
    file which contains the functional implementation for buffer queues. If
    buffer queue operation is enabled, then DRV_USART_BYTE_MODEL_SUPPORT should
    not be true. If this macro is set to false, the behavior of the USART Driver
    Buffer Queue API is not defined. While using the USART driver with DMA, the
    driver supports Buffer Queue Data transfer model irrespective of the value
    of this configuration macro.

  Remarks:
    None
*/

#define DRV_USART_BUFFER_QUEUE_SUPPORT  true
    
// *****************************************************************************
/* USART Driver Byte Model Support

  Summary:
    Specifies if the Byte Model support should be enabled. 
  
  Description:
    This macro defines if Byte Model support should be enabled. Setting this
    macro to true will enable byte model support and all byte operation related
    driver functions. The driver should be built along with the
    drv_usart_byte_model.c file which contains the functional implementation for
    byte model operation.  If byte model operation is enabled, then driver will
    not support buffer queue and read write models. The behaviour of the byte
    mode API when this macro is set to false is not defined.

  Remarks:
    None
*/

#define DRV_USART_BYTE_MODEL_SUPPORT  false

// *****************************************************************************
/* USART Driver Read Write Model Support

  Summary:
    Specifies if the Read Write Model support should be enabled. 
  
  Description:
    This macro defines if Read Write Model support should be enabled. Setting
    this macro to true will enable read write model support and all read write
    related driver functions. The driver should be built along with the
    drv_usart_read_write.c file which contains the functional implementation for
    byte model operation.  If read write model operation is enabled, then
    DRV_USART_BYTE_MODEL_SUPPORT should not be true. The behaviour of the Read
    Write Model API when this macro is set to false is not defined. 

  Remarks:
    None
*/

#define DRV_USART_READ_WRITE_MODEL_SUPPORT  true

// *****************************************************************************
// *****************************************************************************
// Section: Static Overrides
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Index - Used for static drivers

  Summary:
    USART Static Index selection

  Description:
    USART Static Index selection for the driver object reference. This macro
    defines the driver index in case of static and static multi-client 
    build. For example, if this macro is set to DRV_USART_INDEX_2, then 
    static driver APIs would be DRV_USART2_Initialize(), DRV_USART2_Open() etc.
    When building static drivers, this macro should be different for
    each static build of the USART driver that needs to be included in the
    project.

  Remarks:
    This index is required to make a reference to the driver object
*/

#define DRV_USART_INDEX		DRV_USART_INDEX_2

// *****************************************************************************
/* USART Peripheral Library Module ID

  Summary:
    Configures the USART PLIB Module ID 

  Description:
    This macro configures the PLIB ID if the driver is built statically. This 
    value will override the usartID member of the DRV_USART_INIT initialization
    data structure. In that when the driver is built statically, the 
    usartID member of the DRV_USART_INIT data structure will be ignored by the
    driver initialization routine and this macro will be considered. This should
    be set to the PLIB ID of USART module (USART_ID_1, USART_ID_2 and so on). 

  Remarks:
    None
*/

#define DRV_USART_PERIPHERAL_ID   USART_ID_2

// *****************************************************************************
/* Transmit Interrupt Source

  Summary:
    Macro to define the Transmit interrupt source in case of static driver

  Description:
    Macro to define the TX interrupt source in case of static driver. The
    interrupt source defined by this macro will override the txInterruptSource
    member of the DRV_USB_INIT initialization data structure in the driver
    initialization routine. This value should be set to the USART module
    transmit interrupt enumeration in the Interrupt PLIB for the
    microcontroller.

  Remarks:
    None.
*/

#define DRV_USART_INTERRUPT_SOURCE_TRANSMIT	    INT_SOURCE_USART_2_TRANSMIT

// *****************************************************************************
/* Transmit DMA Channel Interrupt Source

  Summary:
    Macro to define the Transmit DMA Channel interrupt source in case of static
    driver

  Description:
    Macro to define the TX DMA Channel interrupt source in case of static
    driver. The interrupt source defined by this macro will override the
    dmaInterruptTransmit member of the DRV_USB_INIT initialization
    data structure in the driver initialization routine. This value should be
    set to the DMA channel interrupt enumeration in the Interrupt PLIB for the
    microcontroller.

  Remarks:
    None.
*/

#define DRV_USART_INTERRUPT_SOURCE_TRANSMIT_DMA

// *****************************************************************************
/* Receive DMA Channel Interrupt Source

  Summary:
    Macro to define the Receive DMA Channel interrupt source in case of static
    driver

  Description:
    Macro to define the Receive DMA Channel interrupt source in case of static
    driver. The interrupt source defined by this macro will override the
    dmaInterruptReceive member of the DRV_USB_INIT initialization
    data structure in the driver initialization routine. This value should be
    set to the DMA channel interrupt enumeration in the Interrupt PLIB for the
    microcontroller.

  Remarks:
    None.
*/

#define DRV_USART_INTERRUPT_SOURCE_RECEIVE_DMA

// *****************************************************************************
/* USART Driver Receive DMA Channel 

  Summary:
    Macro to defines the USART Driver Receive DMA Channel  in case of static
    driver

  Description:
    Macro to define the USART Receive DMA Channel  in case of static driver. The
    DMA channel defined by this macro will override the dmaReceive member
    of the DRV_USB_INIT initialization data structure in the driver
    initialization routine. This value should be set to the DMA channel in the
    DMA PLIB for the microcontroller.

  Remarks:
    None.
*/

#define DRV_USART_RECEIVE_DMA 

// *****************************************************************************
/* USART Driver Transmit DMA Channel 

  Summary:
    Macro to defines the USART Driver Transmit DMA Channel  in case of static
    driver

  Description:
    Macro to define the USART Transmit DMA Channel  in case of static driver. The
    DMA channel defined by this macro will override the dmaTransmit member
    of the DRV_USB_INIT initialization data structure in the driver
    initialization routine. This value should be set to the DMA channel in the
    DMA PLIB for the microcontroller.

  Remarks:
    None.
*/

#define DRV_USART_TRANSMIT_DMA 

// *****************************************************************************
/* Receive Interrupt Source

  Summary:
    Macro to define the Receive interrupt source in case of static driver

  Description:
    Macro to define the Receive interrupt source in case of static driver. The
    interrupt source defined by this macro will override the rxInterruptSource
    member of the DRV_USB_INIT initialization data structure in the driver
    initialization routine. This value should be set to the USART module
    receive interrupt enumeration in the Interrupt PLIB for the
    microcontroller.

  Remarks:
    None.
*/

#define DRV_USART_INTERRUPT_SOURCE_RECEIVE			INT_SOURCE_USART_2_RECEIVE

// *****************************************************************************
/* Error Interrupt Source

  Summary:
    Defines the interrupt source for the error interrupt

  Description:
    Macro to define the Error interrupt source in case of static driver. The
    interrupt source defined by this macro will override the errorInterruptSource
    member of the DRV_USB_INIT initialization data structure in the driver
    initialization routine. This value should be set to the USART module error
    interrupt enumeration in the Interrupt PLIB for the microcontroller.

  Remarks:
    None
*/

#define DRV_USART_INTERRUPT_SOURCE_ERROR		INT_SOURCE_USART_2_ERROR

// *****************************************************************************
/* USART Baud Rate Control

  Summary:
    The USART baud rate for static build of the driver.

  Description:
    The USART baud rate for static build of the driver. When the driver is built
    for static mode, this macro overrides the baud rate member in the driver
    initialization data structure.

  Remarks:
    None
*/

#define DRV_USART_BAUD_RATE                         9600

#endif // #ifndef _DRV_USART_CONFIG_TEMPLATE_H

/*******************************************************************************
 End of File
*/
