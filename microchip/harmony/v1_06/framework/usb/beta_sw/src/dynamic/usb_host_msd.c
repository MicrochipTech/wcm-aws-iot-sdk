/*******************************************************************************
 USB HOST MSD Class Driver

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_msd.c

  Summary:
    USB MSD Host class driver.

  Description:
    USB MSD Host class driver.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
 Copyright (c) 2013 released Microchip Technology Inc.  All rights reserved.

 Microchip licenses to you the right to use, modify, copy and distribute
 Software only when embedded on a Microchip microcontroller or digital signal
 controller that is integrated into your product or third party product
 (pursuant to the sublicense terms in the accompanying license agreement).

 You should refer to the license agreement accompanying this Software for
 additional information regarding your rights and obligations.

 SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "usb/beta_sw/usb_host.h"
#include "usb/beta_sw/src/usb_host_local.h"
#include "usb/beta_sw/usb_host_msd.h"
#include "usb/beta_sw/src/usb_host_msd_local.h"
#include "usb/beta_sw/usb_host_scsi.h"
#include "system/debug/sys_debug.h"



// *****************************************************************************
// *****************************************************************************
// Section: File Scope or Global Data Types
// *****************************************************************************
// *****************************************************************************

USB_HOST_SCSI_INIT  usbHostScsiInit ;

// *****************************************************************************
/* MSD Host driver structure

  Summary:
    Defines the Host class driver structure required for the host layer.

  Description:
    This data type defines the host driver structure required for the
    host layer.

  Remarks:
    This structure is private to the USB Host stack.
 */

USB_HOST_CLASS_DRIVER msdDriver =
{
     /* MSD device level init function */
    .initializeDeviceDriver = NULL ,

    /* MSD interface level init function */
    .initializeInterfaceDriver = _USB_HOST_MSD_InterfaceInitialize ,

    /* MSD de-init function */
    .deInitializeDriver = _USB_HOST_MSD_DeInitialize ,

    /* MSD tasks function */
    .task           = _USB_HOST_MSD_Tasks

};

// *****************************************************************************
/* MSD Host Instance structure

  Summary:
    Defines the MSD instance(s).

  Description:
    This data type defines the MSD instance(s). The number of instances is
    defined by the application using USB_HOST_MSD_INSTANCES_NUMBER.

  Remarks:
    This structure is private to the MSD.
 */

USB_HOST_MSD_INSTANCE  gUSBHostMSDInstance[USB_HOST_MSD_INSTANCES_NUMBER];


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************

// ******************************************************************************
/* Function:
    USB_ERROR   _USB_HOST_MSD_InterfaceInitialize( USB_HOST_DEVICE_ID id ,
                                                   uint8_t interfaceId,
                                                   uint8_t *msdInstance
                                                   uint8_t  speed)

  Summary:
    USB Host MSD function called by the host layer.

  Description:
    USB Host MSD function called by the host layer after interface descriptor
    values are matching with TPL list.

  Remarks:
    This is local function and should not be called directly by the application.
*/
USB_ERROR     _USB_HOST_MSD_InterfaceInitialize
( 
    USB_HOST_DEVICE_ID deviceId ,
    uint8_t interfaceId,
    uint8_t *msdInstance,
    USB_SPEED speed
)

{
    USB_HOST_QUERY           msdQuery;
    USB_ERROR                status = USB_ERROR_NONE ;
    USB_INTERFACE_DESCRIPTOR *interfaceDesc;
    USB_ENDPOINT_DESCRIPTOR  *bulkInEndpointDesc ,
                             *bulkOutEndpointDesc ;
    USB_ENDPOINT_DESCRIPTOR  controlEndpointDesc;
    USB_HOST_MSD_INSTANCE    *msdInstnceInfo;
    uint8_t                  instanceCount;

    /* Find the free space for MSD instance */
    for( instanceCount = 0 ; instanceCount < USB_HOST_MSD_INSTANCES_NUMBER ;
                                                 instanceCount ++)
    {
        msdInstnceInfo = &(gUSBHostMSDInstance[instanceCount]);
        if (msdInstnceInfo->assigned == false )
        {
            /* Find free slot */
            break;
        }
    }
	/* validate the instance */
    if ( instanceCount >= USB_HOST_MSD_INSTANCES_NUMBER)
    {
        /* Assert on invalid MSD index */
        SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_MSD_InterfaceInitialize : Invalid MSD instance number" );
        return USB_ERROR_HOST_DEVICE_INSTANCE_INVALID;
    }

    /* count is the instance number return to host */
    *msdInstance = instanceCount;
    /* Mark flag as use */
    msdInstnceInfo->assigned = true;

    /* store the Device address in instanceInfo */
    msdInstnceInfo->msdDeviceId = deviceId;

    /* save instance number */
    msdInstnceInfo->instanceNumber = instanceCount;

    /* MSD interface Number */
    msdInstnceInfo->interfaceNumber = interfaceId;
    /* Fill the query */
    msdQuery.id       = deviceId;
    msdQuery.class    = USB_HOST_MSD_CLASS;
    msdQuery.subClass = USB_HOST_MSD_SUBCLASS;
    msdQuery.protocol = USB_HOST_MSD_PROTOCOL;
    /* class match */
    msdQuery.flags    = USB_HOST_MATCH_CLASS_SUBCLASS_PROTOCOL;
    msdQuery.interfaceNumber = interfaceId;
    msdQuery.alternateSettingNumber  = 0x00;

    /* Find interface */
    status = _USB_HOST_FindInterface(&(msdQuery), &(interfaceDesc));

    if ( USB_ERROR_NONE != status )
    {
         SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
           " _USB_HOST_MSD_InterfaceInitialize : Interface not found " );
        return ( USB_ERROR_HOST_INTERFACE_NOT_FOUND );
    }
    /* Get control pipe information */
    controlEndpointDesc.transferType =  USB_TRANSFER_TYPE_CONTROL ;
    msdInstnceInfo->controlPipeHandle =  
            _USB_HOST_PipeSetup ( deviceId ,  &controlEndpointDesc , speed );

    if ( USB_HOST_PIPE_HANDLE_INVALID == msdInstnceInfo->controlPipeHandle )
    {
         SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
          " _USB_HOST_MSD_InterfaceInitialize : control pipe handle not found ");
         return ( USB_ERROR_HOST_DEVICE_INSTANCE_INVALID );
    }
    /* Fill Control IRP */
    msdInstnceInfo->controlIRP.callback = _USB_HOST_MSD_ControlTransferComplete;

    msdInstnceInfo->controlIRP.userData = (uintptr_t )msdInstnceInfo;
   /* Fine bulkIn endpoint descriptor */
    msdQuery.alternateSettingNumber = interfaceDesc->bAlternateSetting;

    msdQuery.endpointType      = USB_TRANSFER_TYPE_BULK;
    msdQuery.endpointDirection = USB_DATA_DIRECTION_DEVICE_TO_HOST ;
    msdQuery.flags =    USB_HOST_MATCH_ENDPOINT_TYPE |
                        USB_HOST_MATCH_ENDPOINT_DIRECTION;
    /* Find the Bulk in  endpoint descriptor */
    status = _USB_HOST_FindEndpoint (&(msdQuery), &(bulkInEndpointDesc));
    if(  USB_ERROR_NONE  != status  )
    {
         SYS_DEBUG_MESSAGE ( SYS_ERROR_INFO ,
          " _USB_HOST_MSD_InterfaceInitialize : bulk in pipe handle not found ");
        return USB_ERROR_HOST_ENDPOINT_INVALID;
    }

    msdInstnceInfo->bulkInEndpoint = bulkInEndpointDesc->bEndpointAddress;
    /* Create Bulkin pipe */
    msdInstnceInfo->bulkInPipeHandle = _USB_HOST_PipeSetup
                                ( deviceId ,  bulkInEndpointDesc ,speed );
    if( msdInstnceInfo->bulkInPipeHandle == USB_HOST_PIPE_HANDLE_INVALID )
    {
        SYS_DEBUG ( 0 , "Bulk in  pipe is not created " );
        return USB_ERROR_HOST_PIPE_INVALID;
    }
    /* Fill Bulkin IRP */
    msdInstnceInfo->bulkInIRP.callback =
                            (void * )_USB_HOST_MSD_BulkTransferComplete;

    msdInstnceInfo->bulkInIRP.size = bulkInEndpointDesc->wMaxPacketSize ;
    msdInstnceInfo->bulkInIRP.userData = (uintptr_t)msdInstnceInfo;
     /* Fill the msdQuery to find bulkOut endpoint */
    msdQuery.alternateSettingNumber = interfaceDesc->bAlternateSetting;

    msdQuery.endpointType = USB_TRANSFER_TYPE_BULK;
    msdQuery.endpointDirection = USB_DATA_DIRECTION_HOST_TO_DEVICE;
    msdQuery.flags = USB_HOST_MATCH_ENDPOINT_TYPE |
                     USB_HOST_MATCH_ENDPOINT_DIRECTION;
    /* Find the bulkout endpoint descriptor */
    status = _USB_HOST_FindEndpoint (&(msdQuery), &(bulkOutEndpointDesc));

    msdInstnceInfo->bulkOutEndpoint = bulkOutEndpointDesc->bEndpointAddress ;

    /* Create Bulkout pipe */
    msdInstnceInfo->bulkOutPipeHandle = _USB_HOST_PipeSetup
                                         ( deviceId ,  bulkOutEndpointDesc , speed );
    /* Fill Bulk out piep */
    msdInstnceInfo->bulkOutIRP.callback = _USB_HOST_MSD_BulkTransferComplete;

    msdInstnceInfo->bulkOutIRP.size = bulkOutEndpointDesc->wMaxPacketSize ;
    msdInstnceInfo->bulkOutIRP.userData = (uintptr_t )msdInstnceInfo;

     /* find logical units available in device */
    _USB_HOST_MSD_getMaxLun ( msdInstnceInfo );
    
    msdInstnceInfo->state = USB_HOST_MSD_WAIT;

    return status;
}
/*************************************************************************/

/* Function:
   void  _USB_HOST_MSD_DeInitialize(USB_HOST_DEVICE_ID id )

  Summary:
    DeInitialize the MSD host driver.

  Description:
    After deenumeration Host will Deinitialize the MSD driver

  Parameters:
     USB_HOST_DEVICE_ID                           id ,

  Returns:
        void
*/

void  _USB_HOST_MSD_DeInitialize(USB_HOST_DEVICE_ID id )
 {

      USB_HOST_MSD_INSTANCE *msdInstnceInfo;
      uint8_t msdInstance;
      uint32_t buffer;
	  /* Find instance */
      msdInstance = _USB_HOST_MSD_findInstance( id );
      msdInstnceInfo = &(gUSBHostMSDInstance[msdInstance]);
      /* Close MSD device related pipes */
      _USB_HOST_PipeClose( msdInstnceInfo->bulkInPipeHandle );
      _USB_HOST_PipeClose( msdInstnceInfo->bulkOutPipeHandle );

      msdInstnceInfo->bulkInPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;

      msdInstnceInfo->bulkOutPipeHandle = USB_HOST_PIPE_HANDLE_INVALID;
       /* close the instance */
      msdInstnceInfo->assigned = 0x0;

      msdInstnceInfo->msdCommand->commandStatus =
                                        USB_MSD_TRANSFER_FAIL;
      USB_HOST_SCSI_DeInitialize(msdInstnceInfo->instanceNumber ,
              (SYS_MODULE_INIT  *) &usbHostScsiInit );
     // memset(msdInstnceInfo , 0 ,sizeof( USB_HOST_MSD_INSTANCE));
	 /* inform to the msd device detach event to the application */
     msdInstnceInfo->appEventCallBack ( msdInstnceInfo->instanceNumber ,
                                USB_HOST_MSD_EVENT_DETACH ,
                                &buffer );
     msdInstnceInfo->state = USB_HOST_MSD_WAIT ;

 }

/*************************************************************************/
/* Function:
   USB_HOST_MSD_HANDLE  USB_HOST_MSD_Open( uint8_t  instance )

  Summary:
   Application opens a MSD instance.

  Description:
    After deenumeration Host will Deinitialize the MSD driver

  Parameters:
  uint8_t                    instance;

  Returns:
   USB_HOST_MSD_HANDLE         msdHandle
*/

USB_HOST_MSD_HANDLE  USB_HOST_MSD_Open( uint8_t  instance )
{
    USB_HOST_MSD_INSTANCE  *msdInstnceInfo;
    USB_HOST_MSD_HANDLE msdHandle;
    /* Get instance driver information */
    msdInstnceInfo = &(gUSBHostMSDInstance[instance]);
    if(msdInstnceInfo->assigned == 0x01 )
        msdHandle = (USB_HOST_MSD_HANDLE  )msdInstnceInfo;
    else
        msdHandle = (USB_HOST_MSD_HANDLE  ) NULL;
    return msdHandle;
}
/****************************************************************************/
/* Function:
   void  USB_HOST_MSD_Close( USB_HOST_MSD_HANDLE msdHandle )

  Summary:
    Closes handle to an instance of the USB MSD driver.

  Description:
   This function closes an opened handle to an instance of the USB host msdlayer,
   invalidating the handle.

   Precondition:
    USB_HOST_MSD_Open must have been called to obtain a valid opened device handle.

  Parameters:
    USB_HOST_MSD_HANDLE msdHandle

  Returns:
      void
*/
void  USB_HOST_MSD_Close( USB_HOST_MSD_HANDLE msdHandle )
{
    USB_HOST_MSD_INSTANCE  *msdInstnceInfo;
    msdInstnceInfo = (USB_HOST_MSD_INSTANCE  *)msdHandle;
    //Invalidate the handle
    msdHandle = (USB_HOST_MSD_HANDLE  ) NULL;
}



USB_ERROR USB_HOST_MSD_EventHandlerSet ( 
                                  USB_HOST_MSD_EVENT_HANDLER appMSDHandler )
{

    USB_HOST_MSD_INDEX        MSDInstance;
    USB_HOST_MSD_INSTANCE    *msdInstnceInfo;


    /* check if the given event handler is valid */
    if ( appMSDHandler )
    {

        for ( MSDInstance = 0 ; MSDInstance < USB_HOST_MSD_INSTANCES_NUMBER ; MSDInstance++ )
        {
            msdInstnceInfo = &(gUSBHostMSDInstance[MSDInstance]);
            /* update the event handler for this instance */
            msdInstnceInfo->appEventCallBack = appMSDHandler ;

        }
       
        /* return success */
        return USB_ERROR_NONE;
    }

    return USB_HOST_MSD_INSTANCE_INVALID;
}
/*************************************************************************/

/* Function:
   uint8_t  _USB_HOST_MSD_findInstance(USB_HOST_DEVICE_ID id )

  Summary:
    Find the MSD instance

  Description:


  Parameters:
      USB_HOST_DEVICE_ID id

  Returns:
    uint8_t           instance
*/

uint8_t  _USB_HOST_MSD_findInstance(USB_HOST_DEVICE_ID id )
{
     USB_HOST_MSD_INSTANCE *msdInstnceInfo;
     uint8_t                  count;
    for(count = 0 ; count < USB_HOST_MSD_INSTANCES_NUMBER ; count ++)
    {
         msdInstnceInfo = &(gUSBHostMSDInstance[count]);
         if ( msdInstnceInfo->msdDeviceId == id )
         {
             return count;
         }
    }
    return USB_HOST_MSD_INSTANCE_INVALID ;
}
/*************************************************************************/

/* Function:
   uint8_t  _USB_HOST_MSD_getDriveNumber( USB_HOST_DEVICE_ID  id )

  Summary:
    Find the drveNumber for theis MSD instance

  Description:


  Parameters:
      USB_HOST_DEVICE_ID id

  Returns:
    uint8_t           driveNumber
*/

uint8_t  _USB_HOST_MSD_getDriveNumber( USB_HOST_DEVICE_ID  id )
{
    USB_HOST_MSD_INSTANCE *msdInstnceInfo;
    uint8_t          count;
      for( count = 0 ; count < USB_HOST_MSD_INSTANCES_NUMBER ; count ++)
    {
        msdInstnceInfo = &(gUSBHostMSDInstance[count]);
        {
            return msdInstnceInfo->driveNumber ;
        }
   }
       return USB_HOST_MSD_INSTANCE_INVALID;
}

/*************************************************************************/

/* Function:
   USB_HOST_MSD_INSTANCE *   _USB_HOST_MSD_getDriveInfo  ( uint8_t driveNumber )

  Summary:
  Get the Driver information for this driveNumber

  Description:


  Parameters:
      uint8_t                       driveNumber

  Returns:
    USB_HOST_MSD_INSTANCE           driveNumber
*/

USB_HOST_MSD_INSTANCE *   _USB_HOST_MSD_getDriveInfo  ( uint8_t driveNumber )
{
    USB_HOST_MSD_INSTANCE *msdInstnceInfo;
    uint8_t                  count;
	/* find instance driver info */
    for(count = 0 ; count < USB_HOST_MSD_INSTANCES_NUMBER ; count ++)
    {
        msdInstnceInfo = &(gUSBHostMSDInstance[count]);
        if( (msdInstnceInfo->driveNumber ==  driveNumber))
        {
            return msdInstnceInfo ;
        }
    }
     return NULL;
}

/*************************************************************************/
/* Function:
   USB_HOST_MSD_RESULT  _USB_HOST_MSD_getMaxLun( USB_HOST_MSD_INSTANCE *msdInstnceInfo )

  Summary:
   This function request the LUN number of MSD device.

  Description:

  Parameters:
     USB_HOST_MSD_INSTANCE          *msdInstnceInfo

  Returns:
    USB_HOST_MSD_RESULT
*/

 USB_HOST_MSD_RESULT  _USB_HOST_MSD_getMaxLun(
                        USB_HOST_MSD_INSTANCE *msdInstnceInfo )
{
    USB_ERROR status;

    status = USB_ERROR_NONE ;

  

	/* Fill setup packet */
    msdInstnceInfo->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_DEVICE_TO_HOST |
                                                  USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE ); //interface , Host to device , class
    msdInstnceInfo->setupPacket.bRequest =  USB_MSD_GET_MAX_LUN ;

    msdInstnceInfo->setupPacket.wValue = 0x0000;

    msdInstnceInfo->setupPacket.wIndex = msdInstnceInfo->interfaceNumber;

    msdInstnceInfo->setupPacket.wLength = 0x01;

  
	/* Fill IRP */
    msdInstnceInfo->controlIRP.data = &( msdInstnceInfo->logicalUnitNumber ) ;
    msdInstnceInfo->controlIRP.flags = 0x00;
    msdInstnceInfo->controlIRP.size = 0x1;
    msdInstnceInfo->controlIRP.setup = &( msdInstnceInfo->setupPacket ) ;
    /* keep track of setup request */
    msdInstnceInfo->setupRequest = USB_MSD_GET_MAX_LUN ;
    /* submit IRP */
    status = _USB_HOST_IRPSubmit ( msdInstnceInfo->controlPipeHandle,
                                &(msdInstnceInfo->controlIRP));
    if ( status != USB_ERROR_NONE)
        return USB_HOST_MSD_ERROR;

    /* success */
    return USB_HOST_MSD_RESULT_OK;
}
/*************************************************************************/

/* Function:
   USB_HOST_MSD_RESULT  _USB_HOST_MSD_reset( USB_HOST_MSD_INSTANCE *msdInstnceInfo )

  Summary:
  Reset the Mass storage device

  Description:


  Parameters:
     USB_HOST_MSD_INSTANCE *msdInstnceInfo

  Returns:
    USB_HOST_MSD_RESULT
*/

USB_HOST_MSD_RESULT  _USB_HOST_MSD_reset( USB_HOST_MSD_INSTANCE *msdInstnceInfo )
{

    msdInstnceInfo->setupPacket.bmRequestType  = ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                                                  USB_SETUP_TYPE_CLASS |
                                                  USB_SETUP_RECIPIENT_INTERFACE ); //interface , Host to device , class
    msdInstnceInfo->setupPacket.bRequest =  USB_MSD_RESET ;

    msdInstnceInfo->setupPacket.wValue = 0x0000;

    msdInstnceInfo->setupPacket.wIndex = msdInstnceInfo->interfaceNumber;

    msdInstnceInfo->setupPacket.wLength = 0x00;


	/* Fill IRP */
    msdInstnceInfo->bulkOutIRP.data = NULL;
    msdInstnceInfo->bulkOutIRP.flags = 0x00;
    msdInstnceInfo->bulkOutIRP.size = 0x00;
    msdInstnceInfo->controlIRP.setup = &( msdInstnceInfo->setupPacket );

    /* keep track of setup request */
    msdInstnceInfo->setupRequest = USB_MSD_RESET ;

    /* submit Control IRP */
    _USB_HOST_IRPSubmit(msdInstnceInfo->controlPipeHandle,
                                &(msdInstnceInfo->controlIRP));

    /* Time Delay required */

    return USB_HOST_MSD_RESULT_OK;
}
/*************************************************************************/
/* Function:
   void _USB_HOST_MSD_Tasks(void)

  Summary:
        State machine of the MSD device

  Description:


  Parameters:
        None

  Returns:
        None
 */
void _USB_HOST_MSD_Tasks(uint8_t instanceNumber )
{
   
    USB_HOST_MSD_INSTANCE *msdInstnceInfo;
  
    msdInstnceInfo = &(gUSBHostMSDInstance[instanceNumber]);

    if (msdInstnceInfo == NULL )
        return;
    switch ( msdInstnceInfo->state )
    {
        case USB_HOST_MSD_ATTACHED:

            /* SCSI init */
            usbHostScsiInit.instanceNumber = msdInstnceInfo->instanceNumber ;
            usbHostScsiInit.luns = msdInstnceInfo->logicalUnitNumber;
            USB_HOST_SCSI_Initialize( instanceNumber , (SYS_MODULE_INIT  *) &usbHostScsiInit );
            msdInstnceInfo->appEventCallBack(msdInstnceInfo->instanceNumber,
                                    USB_HOST_MSD_EVENT_ATTACH,
                                    &( msdInstnceInfo->logicalUnitNumber )) ;
            msdInstnceInfo->state = USB_HOST_MSD_READY ;
            break;

        case USB_HOST_MSD_DETACHED :

              msdInstnceInfo->state = USB_HOST_MSD_WAIT ;
            break;

        case USB_HOST_MSD_WAIT :

            break;

        case USB_HOST_MSD_READY:
            USB_HOST_SCSI_Task( instanceNumber );
            break;

        case USB_HOST_MSD_ERROR_RECOVERY :
              _USB_HOST_MSD_reset( msdInstnceInfo );
               msdInstnceInfo->state = USB_HOST_MSD_RESET_DONE;

              break;

        case USB_HOST_MSD_RESET_DONE :
             if (msdInstnceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED)
             {
                  msdInstnceInfo->state = USB_HOST_MSD_READY;
             }

              break;
        case USB_HOST_CLEAR_BULKIN_ENDPOINT:

             _USB_HOST_MSD_ClearEndpointHalt (msdInstnceInfo->instanceNumber ,
                      msdInstnceInfo->bulkInEndpoint , _USB_HOST_MSD_ControlTransferComplete );
            msdInstnceInfo->state = USB_HOST_CLEAR_BULKIN_ENDPOINT_DONE ;
              break;

        case USB_HOST_CLEAR_BULKIN_ENDPOINT_DONE:

             if (msdInstnceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED  )
            {
            		msdInstnceInfo->bulkInIRP.flags = 0x00;
                msdInstnceInfo->bulkInIRP.data = msdInstnceInfo->msdCSW;
                msdInstnceInfo->bulkInIRP.size = 0x0D;
                msdInstnceInfo->bulkRequest = USB_HOST_MSD_CSW;
                _USB_HOST_IRPSubmit(msdInstnceInfo->bulkInPipeHandle,
                                &(msdInstnceInfo->bulkInIRP));
                 msdInstnceInfo->state = USB_HOST_MSD_READY ;
             }


             break;

        case USB_HOST_CLEAR_BULKOUT_ENDPOINT:

             _USB_HOST_MSD_ClearEndpointHalt (msdInstnceInfo->instanceNumber ,
                      msdInstnceInfo->bulkOutEndpoint ,
                     _USB_HOST_MSD_ControlTransferComplete );
              msdInstnceInfo->state =  USB_HOST_CLEAR_BULKOUT_ENDPOINT_DONE ;
              break;

        case USB_HOST_CLEAR_BULKOUT_ENDPOINT_DONE:
             if (msdInstnceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED)
            {
            		msdInstnceInfo->bulkInIRP.flags = 0x00;
                msdInstnceInfo->bulkInIRP.data = msdInstnceInfo->msdCSW;
                msdInstnceInfo->bulkInIRP.size = 0x0D;
                msdInstnceInfo->bulkRequest = USB_HOST_MSD_CSW;
                _USB_HOST_IRPSubmit(msdInstnceInfo->bulkInPipeHandle,
                                &(msdInstnceInfo->bulkInIRP));
                 msdInstnceInfo->state = USB_HOST_MSD_READY ;
             }

        case USB_HOST_MSD_CLEAR_CONTROL_ENDPOINT:

            _USB_HOST_MSD_ClearEndpointHalt (msdInstnceInfo->instanceNumber ,
                      (uint8_t )0 , _USB_HOST_MSD_ControlTransferComplete );

            msdInstnceInfo->state = USB_HOST_MSD_CLEAR_CONTROL_ENDPOINT_DONE;

            break;
        case USB_HOST_MSD_CLEAR_CONTROL_ENDPOINT_DONE :
            if (msdInstnceInfo->controlIRP.status ==
                                USB_HOST_IRP_STATUS_COMPLETED)
            {
                if ( msdInstnceInfo->setupRequest == USB_MSD_GET_MAX_LUN )
                {
                     msdInstnceInfo->state = USB_HOST_MSD_ATTACHED;
                     msdInstnceInfo->logicalUnitNumber = 0x01;
                }
                else

                msdInstnceInfo->state = USB_HOST_MSD_READY;
            }
            break;

        default :
            break;
    }
}

 
/*************************************************************************/
/* Function:
uint8_t _USB_HOST_MSD_validateCSW( USB_MSD_CSW csw, USB_HOST_MSD_COMMAND * msdCommand)

  Summary:
 validate the CSW status and signatures

  Description:


  Parameters:
        None

  Returns:
        None
 */
uint8_t _USB_HOST_MSD_validateCSW( USB_MSD_CSW *csw, volatile USB_HOST_MSD_COMMAND * msdCommand)
{
    if ( csw->dCSWSignature != USB_MSD_VALID_CSW_SIGNATURE )
        return USB_MSD_CSW_SIGNATURE_INVALID ;

    if (csw->dCSWTag !=  USB_MSD_VALID_CBW_TAG )
        return USB_MSD_CSW_TAG_INVALID ;
    if(csw->bCSWStatus != USB_MSD_CSW_STATUS_GOOD)
       return USB_MSD_TRANSFER_FAIL;

    return USB_MSD_CSW_STATUS_SUCCESS;
}
/*************************************************************************/
/* Function:
   uint8_t _USB_HOST_MSD_BulkTransferComplete(USB_HOST_IRP *irp)

  Summary:
        Call back function for Bulk IRP status

  Description:

  Parameters:
        USB_HOST_IRP *irp

  Returns:
        None
*/


void _USB_HOST_MSD_BulkTransferComplete( USB_HOST_IRP *irp)
{
    uint8_t msdInstance;
    USB_HOST_MSD_INSTANCE *msdInstnceInfo;
    uint8_t TransferStatus;

    msdInstnceInfo = ((USB_HOST_MSD_INSTANCE *) (irp->userData));
    msdInstance = msdInstnceInfo->instanceNumber;
    switch (msdInstnceInfo->bulkRequest )
    {
        case  USB_HOST_MSD_CBW :

        if( irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )
        {
              msdInstnceInfo->state = USB_HOST_CLEAR_BULKOUT_ENDPOINT;
              return;
        }

        if( irp->status ==   USB_HOST_IRP_STATUS_ABORTED  )
        {
             msdInstnceInfo->state = USB_HOST_MSD_ERROR_RECOVERY ;
             return ;
        }

        if( irp->status == USB_HOST_IRP_STATUS_COMPLETED )
        {
            if (msdInstnceInfo->msdCBW->dCBWDataTransferLength == 0)
            {
				msdInstnceInfo->bulkInIRP.flags = 0x00;
                msdInstnceInfo->bulkInIRP.data = msdInstnceInfo->msdCSW;
                msdInstnceInfo->bulkInIRP.size = 0x0D;
                msdInstnceInfo->bulkRequest = USB_HOST_MSD_CSW;
                _USB_HOST_IRPSubmit(msdInstnceInfo->bulkInPipeHandle,
                                &(msdInstnceInfo->bulkInIRP));
	   }
            else
            {
               if (msdInstnceInfo->msdCBW->bmCBWFlags.value == USB_MSD_CBW_FLAG_IN)
                {
                    msdInstnceInfo->bulkInIRP.flags = 0x00;
                    msdInstnceInfo->bulkInIRP.data =
                                msdInstnceInfo->msdCommand->data;
                    msdInstnceInfo->bulkInIRP.size =
                                msdInstnceInfo->msdCommand->length;
                    msdInstnceInfo->bulkRequest = USB_HOST_MSD_DATA_IN;
                    _USB_HOST_IRPSubmit(msdInstnceInfo->bulkInPipeHandle,
                                        &(msdInstnceInfo->bulkInIRP));

                }
		else
		{
                    msdInstnceInfo->bulkOutIRP.flags = 0x00;
                    msdInstnceInfo->bulkOutIRP.data =
                            msdInstnceInfo->msdCommand->data;
                    msdInstnceInfo->bulkOutIRP.size =
                            msdInstnceInfo->msdCommand->length;
                    msdInstnceInfo->bulkRequest =
                            USB_HOST_MSD_DATA_OUT;
                    _USB_HOST_IRPSubmit(msdInstnceInfo->bulkOutPipeHandle,
                                        &(msdInstnceInfo->bulkOutIRP));

                }
            }

           }

            break;
        case USB_HOST_MSD_DATA_IN :
             if( irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )
        {
              msdInstnceInfo->state = USB_HOST_CLEAR_BULKIN_ENDPOINT;
              return;
        }



              msdInstnceInfo->bulkInIRP.flags = 0x00;
            msdInstnceInfo->bulkInIRP.data = msdInstnceInfo->msdCSW;
            msdInstnceInfo->bulkInIRP.size = 0x0D;
            msdInstnceInfo->bulkRequest = USB_HOST_MSD_CSW;
            _USB_HOST_IRPSubmit(msdInstnceInfo->bulkInPipeHandle,
                                &(msdInstnceInfo->bulkInIRP));
             break;
        case USB_HOST_MSD_DATA_OUT:
             if( irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )
            {
              msdInstnceInfo->state = USB_HOST_CLEAR_BULKOUT_ENDPOINT;
              return;
            }

             if( irp->status ==   USB_HOST_IRP_STATUS_ABORTED  )
             {
                msdInstnceInfo->state = USB_HOST_MSD_ERROR_RECOVERY ;
                return ;
             }
	    msdInstnceInfo->bulkInIRP.flags = 0x00;
            msdInstnceInfo->bulkInIRP.data = msdInstnceInfo->msdCSW;
            msdInstnceInfo->bulkInIRP.size = 0x0D;
            msdInstnceInfo->bulkRequest = USB_HOST_MSD_CSW;
            _USB_HOST_IRPSubmit(msdInstnceInfo->bulkInPipeHandle,
                                &(msdInstnceInfo->bulkInIRP));

            break;



        case USB_HOST_MSD_CSW :

       if( irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )
        {
          msdInstnceInfo->state = USB_HOST_CLEAR_BULKIN_ENDPOINT ;
          return;
        }

         if( irp->status == USB_HOST_IRP_STATUS_ERROR_NAK_TIMEOUT ||
             irp->status ==   USB_HOST_IRP_STATUS_ABORTED  )
        {
             msdInstnceInfo->state = USB_HOST_MSD_ERROR_RECOVERY ;
             return ;
        }

        if( irp->status == USB_HOST_IRP_STATUS_COMPLETED )
       {
            TransferStatus =
            _USB_HOST_MSD_validateCSW(msdInstnceInfo->msdCSW,
                 msdInstnceInfo->msdCommand);
            if( TransferStatus == USB_MSD_CSW_STATUS_SUCCESS)
            {
                
               msdInstnceInfo->msdCommand->commandStatus = 0x00;
               _USB_HOST_SCSI_TransferCallback ( msdInstnceInfo->instanceNumber,
                                                msdInstnceInfo->msdCommand->commandStatus , 
                                                msdInstnceInfo->msdCommand->length  );
            }
            else
            {
              msdInstnceInfo->msdCommand->commandStatus =
                                        USB_MSD_TRANSFER_FAIL;
              _USB_HOST_SCSI_TransferCallback ( msdInstnceInfo->instanceNumber,
                                                msdInstnceInfo->msdCommand->commandStatus , 
                                                msdInstnceInfo->msdCommand->length  );
              
            }
        }
        break;

        default :
            break;
    }
}
  /*************************************************************************/




/*************************************************************************/
/* Function:
   _USB_HOST_MSD_ControlTransferComplete( USB_HOST_IRP *irp)

  Summary:
        Call back function for controltransfer status

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/

void _USB_HOST_MSD_ControlTransferComplete( USB_HOST_IRP *irp)
{
     USB_HOST_MSD_INSTANCE *msdInstnceInfo;
     
     msdInstnceInfo = ((USB_HOST_MSD_INSTANCE *) (irp->userData));

    switch ( msdInstnceInfo->setupRequest )
    {
        case USB_MSD_GET_MAX_LUN:
            if( irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )
            {
                msdInstnceInfo->logicalUnitNumber = 0x00;
                 msdInstnceInfo->state = USB_HOST_MSD_ATTACHED;
               // msdInstnceInfo->state = USB_HOST_MSD_CLEAR_CONTROL_ENDPOINT ;
                return;
            }

            else if( irp->status == USB_HOST_IRP_STATUS_ERROR_NAK_TIMEOUT ||
                irp->status ==   USB_HOST_IRP_STATUS_ABORTED  )

            {
                msdInstnceInfo->state = USB_HOST_MSD_ERROR_RECOVERY;
                return;
            }

             msdInstnceInfo->logicalUnitNumber = 0x00;
             msdInstnceInfo->state = USB_HOST_MSD_ATTACHED;
              
        
     
          break;

        case USB_MSD_RESET :

            if( irp->status == USB_HOST_IRP_STATUS_ERROR_NAK_TIMEOUT ||
                irp->status ==   USB_HOST_IRP_STATUS_ABORTED ||
                    irp->status ==  USB_HOST_IRP_STATUS_ERROR_STALL )

            {

                 msdInstnceInfo->state = USB_HOST_MSD_ERROR_RECOVERY;
                return;
            }

            else
            {
                msdInstnceInfo->state = USB_HOST_MSD_ATTACHED;
            }

            break;

        case USB_HOST_CLEAR_ENDPOINT:

            if (irp->status != USB_HOST_IRP_STATUS_COMPLETED  )
            {
                msdInstnceInfo->state = USB_HOST_MSD_ERROR_RECOVERY;
            }

            break;
        default :

            break;

    }
}


/*************************************************************************/
/* Function:
   USB_ERROR   _USB_HOST_MSD_ClearEndpointHalt (
                                        uint8_t  instanceNumber ,
                                        uint8_t endpointNumber ,
                                        void* callback )

  Summary:
   Send a setup packet to clear the stall condition

  Description:

  Parameters:
      USB_HOST_IRP                  *irp

  Returns:
   void
*/

USB_ERROR   _USB_HOST_MSD_ClearEndpointHalt (
                                        uint8_t  instanceNumber ,
                                        uint8_t endpointNumber ,
                                        void* callback )
{
    USB_ERROR status;
    USB_HOST_MSD_INSTANCE *msdInstnceInfo;
  
    msdInstnceInfo = &(gUSBHostMSDInstance[instanceNumber]);

    /* prevent the device from deenumerated */
    /* clear the halt */
    status = USB_ERROR_NONE;

   
    /* Cancel all transfers */

_USB_HOST_FillSetupPacket(
                   &(msdInstnceInfo->setupPacket),
                   ( USB_SETUP_DIRN_HOST_TO_DEVICE |
                     USB_SETUP_TYPE_STANDARD |
                     USB_SETUP_RECIPIENT_ENDPOINT ),
                     USB_REQUEST_CLEAR_FEATURE ,
                     USB_FEATURE_SELECTOR_ENDPOINT_HALT , endpointNumber  ,0 ) ;

   
    /* Fill irp */
    msdInstnceInfo->controlIRP.data = NULL;
    msdInstnceInfo->controlIRP.setup = & ( msdInstnceInfo->setupPacket );
    msdInstnceInfo->controlIRP.size = 0;
    msdInstnceInfo->controlIRP.callback = ( USB_HOST_IRP_CALLBACK ) callback;

    msdInstnceInfo->setupRequest = USB_HOST_CLEAR_ENDPOINT ;
    status = _USB_HOST_IRPSubmit( msdInstnceInfo->controlPipeHandle,
                                &msdInstnceInfo->controlIRP );

    return status;
 }
/*************************************************************************/
/* Function:
USB_ERROR USB_HOST_MSD_Transfer (uint8_t driveNumber, USB_HOST_MSD_COMMAND *msdCommand)

  Summary:
        Transfer the data to/from the MSD

  Description:


  Parameters:
       uint8_t        driveNumber
   USB_HOST_MSD_COMMAND      *msdCommand
  Returns:
       USB_ERROR                             status
*/



USB_ERROR USB_HOST_MSD_Transfer (uint8_t driveNumber,  volatile USB_HOST_MSD_COMMAND *msdCommand)
{
    USB_ERROR status;
    uint8_t         count;
    USB_HOST_MSD_INSTANCE *msdInstnceInfo;

    status  = USB_ERROR_NONE;
	/* Get drive info */
    msdInstnceInfo  = _USB_HOST_MSD_getDriveInfo  (  driveNumber );
	/* Fill CBW */
    msdInstnceInfo->msdCommand = msdCommand;
    msdInstnceInfo->msdCBW = &msdCBW;
    msdInstnceInfo->msdCSW = &msdCSW;
    msdInstnceInfo->msdCBW->dCBWSignature = USB_MSD_VALID_CBW_SIGNATURE ;
    msdInstnceInfo->msdCBW->dCBWTag = USB_MSD_VALID_CBW_TAG;
    msdInstnceInfo->msdCBW->dCBWDataTransferLength = msdCommand->length;
    if ( msdCommand ->direction == 0x01 )
    msdInstnceInfo->msdCBW->bmCBWFlags.value = USB_MSD_CBW_FLAG_IN ;
    else
         msdInstnceInfo->msdCBW->bmCBWFlags.bits.direction = USB_MSD_CBW_FLAG_OUT ;
    msdInstnceInfo->msdCBW->bCBWLUN = 0x00;
    msdInstnceInfo->msdCBW->bCBWCBLength = msdCommand->commandLength;
    for ( count = 0 ; count<16 ; count++)
        msdInstnceInfo->msdCBW->CBWCB[count] = 0x00;

    for( count = 0 ; count < msdCommand->commandLength ; count++)
        msdInstnceInfo->msdCBW->CBWCB[count] = * (uint8_t *)( msdCommand->command + count );
    /* Fill irp */
   // msdInstnceInfo->bulkoutIRP.status =
    msdInstnceInfo->bulkOutIRP.flags = 0x00;
    msdInstnceInfo->bulkOutIRP.data =  msdInstnceInfo->msdCBW ;
    msdInstnceInfo->bulkOutIRP.size =  0x1F;
    msdInstnceInfo->bulkRequest =  USB_HOST_MSD_CBW;
    _USB_HOST_IRPSubmit(msdInstnceInfo->bulkOutPipeHandle,
                                &(msdInstnceInfo->bulkOutIRP));
    return USB_ERROR_NONE;
}



/***************  End of  File ************************************/





