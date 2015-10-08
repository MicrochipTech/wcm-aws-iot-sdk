/*******************************************************************************
  USB Audio class definitions

  Company:
    Microchip Technology Inc.

  File Name:
    usb_audio_v2_0.h

  Summary:
    USB Audio class definitions

  Description:
    This file describes the Audio v2.0 class specific definitions.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to  you  the  right  to  use,  modify,  copy  and  distribute
Software only when embedded on a Microchip  microcontroller  or  digital  signal
controller  that  is  integrated  into  your  product  or  third  party  product
(pursuant to the  sublicense  terms  in  the  accompanying  license  agreement).

You should refer  to  the  license  agreement  accompanying  this  Software  for
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
// DOM-IGNORE-END

#ifndef _USB_AUDIO_v2_0_H_
#define _USB_AUDIO_v2_0_H_

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Audio Interface Class codes

  Summary:
    Identifies the Class Codes for Audio interface. 

  Description:
    This constant identifies the value of the Audio Interface class code. 

  Remarks:
    None.
*/

#define  USB_AUDIO_CLASS_CODE 0x01

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Audio Interface subclass codes

  Summary:
    Identifies the subclass codes for Audio interface. 

  Description:
    This enumeration identifies the possible subclass codes for 
    audio interface. 

  Remarks:
    The "ISC" in the enumeration member names is an acronym for Interface 
    Subclass Code.
*/


typedef enum
{
    USB_AUDIO_SUBCLASS_UNDEFINED    = 0x00,
    USB_AUDIO_AUDIOCONTROL          = 0x01,
    USB_AUDIO_AUDIOSTREAMING        = 0x02,
    USB_AUDIO_MIDISTREAMING         = 0x03

} USB_AUDIO_SUBCLASS_CODE; 

// *****************************************************************************
/* Audio Interface Protocol codes

  Summary:
    Identifies the protocol codes for Audio interface. 

  Description:
    This enumeration identifies the possible protocol codes for 
    audio interface

  Remarks:
    As per USB Device class defintion for Audio Device release v1.0, the
    protocol code should always be 0.
*/

typedef enum
{
    USB_AUDIO_PR_PROTOCOL_UNDEFINED    = 0x0

} USB_AUDIO_PROTOCOL_CODE;

// *****************************************************************************
/* Audio Class Specific Descriptor Types

  Summary:
    Identifies the Audio class specific descriptor types for Audio. 

  Description:
    This enumeration identifies the Audio Class specific descriptor types. 

  Remarks:
    The "CS" in the enumeration member names is an acronym for Class Specific.
*/

typedef enum
{
    USB_AUDIO_CS_UNDEFINED       = 0x20,
    USB_AUDIO_CS_DEVICE          = 0x21,
    USB_AUDIO_CS_CONFIGURATION   = 0x22,
    USB_AUDIO_CS_STRING          = 0x23,
    USB_AUDIO_CS_INTERFACE       = 0x24,
    USB_AUDIO_CS_ENDPOINT        = 0x25

} USB_AUDIO_CS_DESCRIPTOR_TYPE;

// *****************************************************************************
/* Audio Class Specfic AC Interface Descriptor Subtypes

  Summary:
    Identifies the Audio Class Specific AC Interface Descriptor Subtypes.

  Description:
    This enumeration identifies the possible Audio Class Specific AC Interface
    Descriptor Subtypes.

  Remarks:
    The "CS" in the enumeration member names is an acronym for Class Specific.
    The "AC" in the enumeration member names is an acronym for Audio Control.
*/

typedef enum
{
    USB_AUDIO_AC_DESCRIPTOR_UNDEFINED    = 0x00,
    USB_AUDIO_HEADER                     = 0x01,
    USB_AUDIO_INPUT_TERMINAL             = 0x02,
    USB_AUDIO_OUTPUT_TERMINAL            = 0x03,
    USB_AUDIO_MIXER_UNIT                 = 0x04,
    USB_AUDIO_SELECTOR_UNIT              = 0x05,
    USB_AUDIO_FEATURE_UNIT               = 0x06,
    USB_AUDIO_PROCESSING_UNIT            = 0x07,
    USB_AUDIO_EXTENSION_UNIT             = 0x08,

} USB_AUDIO_CS_AC_INTERFACE_DESCRIPTOR_SUBTYPE;

// From USB 2.0, additional USB descriptor types
#define USB_DESCRIPTOR_DEBUG            0x0A    // bDescriptorType for a Debug Descriptor
#define USB_DESCRIPTOR_IA               0x0B    // bDescriptorType for an Interface Associate Descriptor

/* A.1 Audio Function Class Code */
#define AUDIO_FUNCTION              AUDIO

/* A.2 Audio Function Subclass Codes */
#define FUNCTION_SUBCLASS_UNDEFINED 0x00

/* A.3 Audio Function Protocol Codes */
#define FUNCTION_PROTOCOL_UNDEFINED 0x00
#define AF_VERSION_02_00            IP_VERSION_02_00

/* A.4 Audio Interface Class Code */
#define AUDIO                       0x01

/* A.5 Audio Interface Subclass Codes */
#define AUDIOCONTROL                0x01
#define AUDIOSTREAMING              0x02
#define MIDISTREAMING               0x03

/* A.6 Audo Interface Protocol Codes */
#define IP_VERSION_02_00            0x20

/* A.7 Audio Function Category Codes */
#define FUNCTION_SUBCLASS_UNDEFINED 0x00
#define DESKTOP_SPEAKER             0x01
#define HOME_THEATER                0x02
#define MICROPHONE                  0x03
#define HEADSET                     0x04
#define TELEPHONE                   0x05
#define CONVERTER                   0x06
#define VOICE_SOUND_RECORDER        0x07
#define IO_BOX                      0x08
#define MUSICAL_INTRUMENT           0x09
#define PRO_AUDIO                   0x0A
#define AUDIO_VIDEO                 0x0B
#define CONTROL_PANEL               0x0C
#define OTHER                       0xFF

/* A.8 Audio Class-Specific Descriptor Types */
#define CS_UNDEFINED                0x20
#define CS_DEVICE                   0x21
#define CS_CONFIGURATION            0x22
#define CS_STRING                   0x23
#define CS_INTERFACE                0x24
#define CS_ENDPOINT                 0x25

/* A.9 Audio Class-Specific AC Interface Descriptor Subtypes */
#define AC_DESCRIPTOR_UNDEFINED     0x00
#define HEADER                      0x01
#define INPUT_TERMINAL              0x02
#define OUTPUT_TERMINAL             0x03
#define MIXER_UNIT                  0x04
#define SELECTOR_UNIT               0x05
#define FEATURE_UNIT                0x06
#define EFFECT_UNIT                 0x07
#define PROCESSING_UNIT             0x08
#define EXTENSION_UNIT              0x09
#define CLOCK_SOURCE                0x0A
#define CLOCK_SELECTOR              0x0B
#define CLOCK_MULTIPLIER            0x0C
#define SAMPLE_RATE_CONVERTER       0x0D

/* A.10 Audio Class Specific AS Interface Descriptor Subtypes */
#define AS_DESCRIPTOR_UNDEFINED     0x00
#define AS_GENERAL                  0x01
#define FORMAT_TYPE                 0x02
#define ENCODER                     0x03
#define DECODER                     0x04

/* A.11 Effect Unit Effect Types */
#define EFFECT_UNDEFINED            0x00
#define PARAM_EQ_SECTION_EFFECT     0x01
#define REVERBERATION_EFFECT        0x02
#define MOD_DELAY_EFFECT            0x03
#define DYN_RANGE_COMP_EFFECT       0x04

/* A.12 Processing Unit Process Types */
#define PROCESS_UNDEFINED           0x00
#define UP_DOWNMIX_PROCESS          0x01
#define DOLBY_PROLOGIC_PROCESS      0x02
#define STEREO_EXTENDER_PROCESS     0x03

/* A.13 Audio Class-Specific Endpoint Descriptor Subtypes */
#define DESCRIPTOR_UNDEFINED        0x00
#define EP_GENERAL                  0x01

/* A.14 Audio Class-Specific Request Codes */
#define REQUEST_CODE_UNDEFINED      0x00
#define CUR                         0x01
#define RANGE                       0x02
#define MEM                         0x03

/* A.15 Encoder Type Codes */
#define ENCODER_UNDEFINED           0x00
#define OTHER_ENCODER               0x01
#define MPEG_ENCODER                0x02
#define AC_3_ENCODER                0x03
#define WMA_ENCODER                 0x04
#define DTS_ENCODER                 0x05

/* A.17 Control Selector Codes */
/* A.17.1 Clock Source Control Selectors */
#define CS_CONTROL_UNDEFINED        0x00
#define CS_SAM_FREQ_CONTROL         0x01
#define CS_CLOCK_VALID_CONTROL      0x02

/* A.17.2 Clock Selector Control Selectors */
#define CX_CONTROL_UNDEFINED        0x00
#define CX_CLOCK_SELECTOR_CONTROL   0x01

/* A.17.7 Feature Unit Control Selectors */
typedef enum
{
    USB_AUDIO_FCS_FU_CONTROL_UNDEFINED        = 0x00,
    USB_AUDIO_FCS_MUTE_CONTROL                = 0x01,
    USB_AUDIO_FCS_VOLUME_CONTROL              = 0x02,
    USB_AUDIO_FCS_BASS_CONTROL                = 0x03,
    USB_AUDIO_FCS_MID_CONTROL                 = 0x04,
    USB_AUDIO_FCS_TREBLE_CONTROL              = 0x05,
    USB_AUDIO_FCS_GRAPHIC_EQUALIZER_CONTROL   = 0x06,
    USB_AUDIO_FCS_AUTOMATIC_GAIN_CONTROL      = 0x07,
    USB_AUDIO_FCS_DELAY_CONTROL               = 0x08,
    USB_AUDIO_FCS_BASS_BOOST_CONTROL          = 0x09,
    USB_AUDIO_FCS_LOUDNESS_CONTROL            = 0x0A

}USB_AUDIO_FEATURE_UNIT_CONTROL_SELECTORS;

#define FU_CONTROL_UNDEFINED        0x00
#define FU_MUTE_CONTROL             0x01
#define FU_VOLUME_CONTROL           0x02

/* A.17.11 Audio Streaming Interface Control Selectors */
#define AS_CONTROL_UNDEFINED        0x00
#define AS_ACT_ALT_SETTING_CONTROL  0x01
#define AS_VAL_ALT_SETTINGS_CONTROL 0x02
#define AS_AUDIO_DATA_FORMAT_CONTROL 0x03

/* Audio Class-Specific Descriptor Types */
#define CS_INTERFACE                0x24
#define CS_ENDPOINT                 0x25


/* Audio Class-Specific AS Interface Descriptor Subtypes */
#define FORMAT_TYPE                 0x02


/***********************************************************************/
/* Universal Serial Bus Device Class Definition for Audio Data Formats */

/* A.1 Format Type Codes */
#define FORMAT_TYPE_UNDEFINED       0x00
#define FORMAT_TYPE_I               0x01
#define FORMAT_TYPE_II              0x02
#define FORMAT_TYPE_III             0x03
#define FORMAT_TYPE_IV              0x04
#define EXT_FORMAT_TYPE_I           0x81
#define EXT_FORMAT_TYPE_II          0x82
#define EXT_FORMAT_TYPE_III         0x83

/* A.3 Side Band Protocol Codes */
#define PROTOCOL_UNDEFINED          0x00
#define PRESS_TIMESTAMP_PROTOCOL    0x01


/***********************************************************************/
/* Univeral Serial Bus  Device Class Definition for Terminal Types */

/* 2.1 USB Terminal Types */
/* Terminal Types that describe Terminals that handle signals carried over USB */
#define USB_TERMTYPE_UNDEFINED               0x0100
#define USB_TERMTYPE_USB_STREAMING           0x0101
#define USB_TERMTYPE_VENDOR_SPECIFIC         0x01FF

/* 2.2 Input Terminal Types */
/* Terminal Types that describe Terminals that are designed to record sounds */
#define INPUT_UDEFINED              0x0200
#define MICROPHONE_                 0x0201
#define DESKTOP_MICROPHONE          0x0202
#define PERSONAL_MICROPHONE         0x0203
#define OMNIDIRECTIONAL_MICROPHONE  0x0204
#define MICROPHONE_ARRAY            0x0205
#define PROCESSING_MICROPHONE_ARRAY 0x0206

/* 2.3 Output Terminal Types */
/* These Terminal Types describe Terminals that produce audible signals that are intended to
 * be heard by the user of the audio function */
#define SPEAKER                     0x0301
#define HEADPHONES                  0x0302
#define HEAD_MOUNTED_DISPLAY        0x0303
#define DESKTOPSPEAKER              0x0304
#define ROOM_SPEAKER                0x0305
#define COMMUNICATION_SPEAKER       0x0306
#define LOW_FREQ_EFFECTS_SPEAKER    0x0307


#define FORMAT_TYPE_I               0x01

/* A.2 AudioData Format Bit Allocation in the bmFormats field */
/* A.2.1 Audio Data Format Type I Bit Allocations */
#define PCM                         0x00000001
#define PCM8                        0x00000002
#define IEEE_FLOAT                  0x00000004
/* 0x08 to 0x4000000 Reserved */
#define TYPE_1_RAW_DATA             0x80 /* TODO */

//=================================================================
// Structures
// Clock Source
typedef struct
{
    unsigned direction : 1;
    unsigned type: 2;
    unsigned Recipient: 5;
    uint8_t bRequest;
    uint8_t channelNumber;
    uint8_t controlSelector;
    uint8_t interfaceId;
    uint8_t ClockSourceId;
    uint16_t wLength;
} USB_AUDIO_CLOCKSOURCE_CONTROL_REQUEST;

typedef struct
{
    unsigned short  numSampleRate;          // Number of supported sampling rates
    unsigned short   min[2];
    unsigned short   max[2];
    unsigned short   res[2];
} USB_AUDIO_CLOCKSOURCE_RANGE;

// Clock Select
typedef struct
{

    unsigned direction : 1;
    unsigned type: 2;
    unsigned Recipient: 5;
    uint8_t bRequest;
    uint8_t channelNumber;
    uint8_t controlSelector;
    uint8_t interfaceId;
    uint8_t ClockSelectId;
    uint16_t wLength;
} USB_AUDIO_CLOCKSELECT_CONTROL_REQUEST;

// *****************************************************************************
/* USB Audio Status Interrupt Endpoint Status Word Format.

  Summary:
    Specifies the format of USB Audio Status Interrupt Endpoint Status Word
    Format.

  Description:
    This type indentifies the format of the USB Audio Status Interrupt Endpoint
    Status Word. This is defined in Table 3-1 of the USB Device Class Defintion 
    for Audio Devices Release 1.0 document.

  Remarks:
    Always needs to be packed.
*/

typedef union __attribute__((packed))
{
    uint16_t value;

    struct
    {
        unsigned bStatusType:8;
        unsigned bOriginator:8; 
    };
    struct
    {
        unsigned originator:4;
        unsigned :2;
        unsigned memoryContentsChanged:1;
        unsigned interruptPending:1;
        unsigned bOriginator:8;
    };
} USB_AUDIO_INTERRUPT_STATUS_WORD;

// *****************************************************************************
/* USB Audio Class Specific Audio Control Interface Header Descriptor

  Summary:
    Identifies the USB Audio Class Specific Audio Control Interface Header
    Descriptor.

  Description:
    This type identifies the USB Audio Class Specific Audio Control Interface
    Header Descriptor. This structure is as per Table 4-2 of the USB Device
    Class Defintion for Audio Device v1.0 document.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__ ((packed))
{
	/* Size of this descriptor */
    uint8_t bLength;
	
	/* Interface descriptor type */
    uint8_t bDescriptorType;
	
	/* Interface Descriptor Subtype */
    uint8_t bDescriptorSubtype;
	
    /* Audio Device Class Specification Release Number in BCD format*/	
    uint16_t bcdADC;
	
    /* Total number of bytes returned for the class-specific AudioControl
       interface descriptor. Includes the combined length of this descriptor
       header and all Unit and Terminal descriptors */
    uint16_t wTotalLength;

    /* The number of AudioStreaming and MIDIStreaming interfaces in the Audio
       Interface Collection to which this AudioControl interface belongs. This
       is equal to USB_DEVICE_AUDIO_STREAMING_INTERFACES_NUMBER.*/
    uint8_t bInCollection;

    /* Note: The subsequent members of this structure should contain the
     * interface numbers of Audio and MIDI streaming interfaces in the
     * collection. The total number of interface entries should match the value
     * of bInCollection member of this structure. The type of each entry should
     * be USB_DEVICE_AUDIO_STREAMING_INTERFACE_NUMBER. */

} USB_AUDIO_CS_AC_INTERFACE_HEADER_DESCRIPTOR;

// *****************************************************************************
/* Audio Input Terminal Descriptor Type

  Summary:
    Identifies the Audio Input Terminal Descriptor Type.

  Description:
    This type identifies the Audio Input Terminal Descriptor.  This structure is
    as per Table 4-3 of the USB Device Class Defintion for Audio Device v1.0
    document.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__ ((packed))
{
    /* Size of this descriptor */
    uint8_t bLength;

    /* Interface descriptor type */
    uint8_t bDescriptorType;

    /* Interface Descriptor Subtype */
    uint8_t bDescriptorSubtype;

    /* Unique Terminal Identifier Constant */
    uint8_t bTerminalID;

    /* Terminal Type */
    uint16_t wTerminalType;

    /* ID of the associated Output Terminal */
    uint8_t bAssocTerminal;

    /* Number of channels in the terminal output */
    uint8_t bNrChannels;

    /* Spatial location of the logical channels */
    uint16_t wChannelConfig;

    /* First Logical Channel String descriptor index */
    uint8_t iChannelNames;

    /* Input Terminal String Descriptor Index */
    uint8_t iTerminal;

} USB_AUDIO_INPUT_TERMINAL_DESCRIPTOR;

// *****************************************************************************
/* Audio Output Terminal Descriptor Type

  Summary:
    Identifies the Audio Output Terminal Descriptor Type.

  Description:
    This type identifies the Audio Output Terminal Descriptor.  This structure
    is as per Table 4-4 of the USB Device Class Defintion for Audio Device v1.0
    document.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__ ((packed))
{
    /* Size of this descriptor */
    uint8_t bLength;

    /* Interface descriptor type */
    uint8_t bDescriptorType;

    /* Interface Descriptor Subtype */
    uint8_t bDescriptorSubtype;

    /* Unique Terminal Identifier Constant */
    uint8_t bTerminalID;

    /* Terminal Type */
    uint16_t wTerminalType;

    /* ID of the associated Input Terminal */
    uint8_t bAssocTerminal;

    /* Source Unit or Terminal ID */
    uint8_t bSourceID;

    /* Output Terminal String Descriptor Index */
    uint8_t iTerminal;

} USB_AUDIO_OUTPUT_TERMINAL_DESCRIPTOR;

// *****************************************************************************
/* USB Audio Class Specific Audio Streaming Interface Descriptor

  Summary:
    Identifies the USB Audio Class Specific Audio Streaming Interface Descriptor
    Type.

  Description:
    This type identifies the USB Audio Class Specific Audio Streaming Interface
    Descriptor.  This structure is as per Table 4-19 of the USB Device Class
    Defintion for Audio Device v1.0 document.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Size of the descriptor in bytes */
    uint8_t bLength;

    /* CS_INTERFACE descriptor type */
    uint8_t bDescriptorType;

    /* AS_GENERAL desriptor subtype */
    uint8_t bDescriptorSubtype;

    /* Terminal ID of the terminal to which the endpoint of this interface is
     * connected */
    uint8_t bTerminalLink;

    /* Delay introduced by the data path */
    uint8_t bDelay;

    /* Audio data format of this interface */
    uint16_t wFormatTag;

} USB_AUDIO_CS_AS_INTERFACE_DESCRIPTOR;

// *****************************************************************************
/* USB Audio Class Specific Audio Streaming Isochronous Audio Data Endpoint 
   Descriptor

  Summary:
    Identifies the USB Audio Class Specific Audio Streaming Isochronous Audio
    Data Endpoint Descriptor

  Description:
    This type identifies the USB Audio Class Specific Audio Streaming
    Isochronous Audio Data Endpoint Descriptor Type. This structure is as per
    Table 4-21 of the USB Device Class Defintion for Audio Device v1.0 document.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Size of this descriptor */
    uint8_t bLength;

    /* CS_ENDPOINT descriptor type */
    uint8_t bDescriptorType;

    /* EP_GENERAL descriptor subtype */
    uint8_t bDescriptorSubtype;

    /* Bit map indicating the mentioned control is supported by this endpoint */
    uint8_t bmAttributes;

    /* Indicates the units used for the wLockDelay field */
    uint8_t bLockDelayUnits;

    /* Indicates the time it takes this endpoint to reliably lock its internal
     * clock recovery circuitry */
    uint16_t wLockDelay;

} USB_AUDIO_CS_AS_ISOCHRONOUS_AUDIO_DATA_EP_DESCRIPTOR;

// *****************************************************************************
/* USB Audio Control Interface Set and Get Request.

  Summary:
    Identifies the type of the USB Audio Control Interface Set and Get Request.

  Description:
    This type identifies the type of the USB Audio Control Interface Set and Get
    Request.  The application can type cast the received audio class specific
    setup packet to this type in order to service Control attribute Set and Get
    requests. This structure is as per Table 5-1 and 5-2 of the USB Device Class
    Defintion for Audio Device v1.0 document. Refer to section 5.2.1.1 and
    section 5.2.1.2 of this document for detailed description and request
    specific interpretation of the fields.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Request type SET or GET */
    uint8_t bmRequestType;

    /* Identifies the attribute to be accessed */
    uint8_t bRequest;

    /* The wValue field */
    uint16_t wValue;

    union
    {
        /* Identifies the interface number */
        uint8_t interfaceNumber;

        /* Identifies the endpoint */
        uint8_t endpoint;
    };

    /* Identifies the entity ID */
    uint8_t entityID;

    /* Length of the parameter block */
    uint16_t wLength;

} USB_AUDIO_CONTROL_INTERFACE_REQUEST;

// *****************************************************************************
/* USB Audio Terminal Control Set and Get Request.

  Summary:
    Identifies the type of the USB Audio Terminal Control Set and Get Request.

  Description:
    This type identifies the type of the USB Audio Terminal Control Set and Get
    Request.  The application can type cast the received audio class specific
    setup packet to this type in order to service terminal control Set and Get
    requests. This structure is as per Table 5-3 and 5-4 of the USB Device Class
    Defintion for Audio Device v1.0 document. Refer to section 5.2.2.1.1 and
    section 5.2.2.1.2 of this document for detailed description and request
    specific interpretation of the fields.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Request type SET or GET */
    uint8_t bmRequestType;

    /* Identifies the attribute to be accessed */
    uint8_t bRequest;

    /* This field is usually 0 */
    unsigned :8;

    /* Identifies the control to be accessed */          
    uint8_t controlSelector;

    /* Identifies the interface number */
    uint8_t interfaceNumber;
    
    /* Identifies the terminal ID */
    uint8_t terminalId;
    
    /* Length of the parameter block */
    uint16_t wLength;
    
} USB_AUDIO_TERMINAL_CONTROL_REQUEST;

// *****************************************************************************
/* USB Audio Mixer Unit Control Set and Get Request.

  Summary:
    Identifies the type of the USB Audio Mixer Unit Control Set and Get Request.

  Description:
    This type identifies the type of the USB Audio Mixer Unit Control Set and
    Get Request.  The application can type cast the received audio class
    specific setup packet to this type in order to service mixer unit control
    Set and Get requests. This structure is as per Table 5-6 and 5-7 of the USB
    Device Class Defintion for Audio Device v1.0 document. Refer to section
    5.2.2.2.1 and section 5.2.2.2.2 of this document for detailed description
    and request specific interpretation of the fields.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Request type SET or GET */
    uint8_t bmRequestType;

    /* Identifies the attribute to be accessed */
    uint8_t bRequest;

    /* Output Channel Number */
    uint8_t OCN;

    /* Input Channel Number */
    uint8_t ICN;

    /* Identifies the interface number */
    uint8_t interfaceNumber;
    
    /* Identifies the Mixer Unit ID */
    uint8_t mixerUnitID;
    
    /* Length of the parameter block */
    uint16_t wLength;

} USB_AUDIO_MIXER_UNIT_CONTROL_REQUEST;

// *****************************************************************************
/* USB Audio Selector Unit Control Set and Get Request.

  Summary:
    Identifies the type of the USB Audio Selector Unit Control Set and Get Request.

  Description:
    This type identifies the type of the USB Audio Selector Unit Control Set and
    Get Request.  The application can type cast the received audio class
    specific setup packet to this type in order to service selector unit control
    Set and Get requests. This structure is as per Table 5-11 and 5-12 of the USB
    Device Class Defintion for Audio Device v1.0 document. Refer to section
    5.2.2.3.1 and section 5.2.2.3.2 of this document for detailed description
    and request specific interpretation of the fields.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Request type SET or GET */
    uint8_t bmRequestType;

    /* Identifies the attribute to be accessed */
    uint8_t bRequest;

    /* This value is always 0 */
    unsigned :16;
    
    /* Identifies the interface number */
    uint8_t interfaceNumber;

    /* Identifies the Selector Unit ID */
    uint8_t selectorUnitId;
    
    /* Length of the parameter block */
    uint16_t wLength;

} USB_AUDIO_SELECTOR_UNIT_CONTROL_REQUEST;

// *****************************************************************************
/* USB Audio Feature Unit Control Set and Get Request.

  Summary:
    Identifies the type of the USB Audio Feature Unit Control Set and Get Request.

  Description:
    This type identifies the type of the USB Audio Feature Unit Control Set and
    Get Request.  The application can type cast the received audio class
    specific setup packet to this type in order to service the feature unit
    control Set and Get requests. This structure is as per Table 5-14 and 5-15
    of the USB Device Class Defintion for Audio Device v1.0 document. Refer to
    section 5.2.2.4.1 and section 5.2.2.4.2 of this document for detailed
    description and request specific interpretation of the fields.

  Remarks:
    Always needs to be packed.
*/

typedef struct __attribute__((packed))
{
    /* Request type SET or GET */
    uint8_t bmRequestType;
    
    /* Identifies the attribute to be accessed */
    uint8_t bRequest;

    /* Identifies the channel number */
    uint8_t channelNumber;

    /* Identifies the control selector */
    uint8_t controlSelector;

    /* Identifies the interface number */
    uint8_t interfaceNumber;
    
    /* Identifies the feature unit ID */
    uint8_t featureUnitId;

    /* Length of the parameter block */
    uint16_t wLength;
} USB_AUDIO_FEATURE_CONTROL_REQUEST;

// *****************************************************************************
/* USB Audio Processing Unit Control Set and Get Request.

  Summary:
    Identifies the type of the USB Audio Processing Unit Control Set and Get
    Request.

  Description:
    This type identifies the type of the USB Audio Processing Unit Control Set
    and Get Request. The application can type cast the received audio class
    specific setup packet to this type in order to service the processing unit
    control Set and Get requests. This structure is as per Table 5-36 and 5-37
    of the USB Device Class Definition for Audio Device v1.0 document. Refer to
    section 5.2.2.5.1 and section 5.2.2.5.2 of this document for detailed
    description and request specific interpretation of the fields.

  Remarks:
    Always needs to be packed.
*/

typedef struct
{
    /* Request type Set or Get */
    uint8_t bmRequestType;

    /* Identifies the attribute to be accessed */
    uint8_t bRequest;

    /* Not used */
    unsigned :8;
    
    /* Identifies the Control Selector */          
    uint8_t controlSelector;

    /* Identifies the interface number */
    uint8_t interfaceNumber;

    /* Identifies the processing unit ID */
    uint8_t processingUnitID;
    
    /* Length of the parameter block */
    uint16_t wLength;

} USB_AUDIO_PROCESSING_UNIT_CONTROL_REQUEST;

// *****************************************************************************
/* USB Audio Mixer Unit Descriptor Header.

  Summary:
   Structure describing USB Audio Mixer unit descriptor Header.

  Description:
    This type identifies the type of the USB Audio mixer unit descriptor.  This
    structure is as per Table 5-45 USB Device Class Definition for Audio Device
    v1.0 document. This structure represents first 5 Bytes of the USB Audio mixer
    unit descriptor.

    The complete structure of the USB_AUDIO_MIXER_UNIT_DESCRIPTOR should be as
    such.

    struct __attribute__((packed))
    {
        USB_AUDIO_MIXER_UNIT_DESCRIPTOR_HEADER header;
        USB_AUDIO_MIXER_UNIT_DESCRIPTOR_SOURCE_ID mixerInputSourceID[NUMBER_OF_MIXER_INPUT_PINS];
        USB_AUDIO_MIXER_UNIT_DESCRIPTOR_CHANNEL_INFO channelInfo;
        USB_AUDIO_MIXER_UNIT_DESCRIPTOR_BMACONTROLS mixingControls[NUMBER_OF_PROGRAMMABLE_MIXING_CONTROLS];
        USB_AUDIO_MIXER_UNIT_DESCRIPTOR_FOOTER stringdesciptorIndex;
    
    } USB_AUDIO_MIXER_UNIT_DESCRIPTOR;

  Remarks:
    This structure is a part of the USB_AUDIO_MIXER_UNIT_DESCRIPTOR. The
    USB_AUDIO_MIXER_UNIT_DESCRIPTOR cannot be defined as it an open ended data
    structure.

*/

typedef struct
{
    /* Size of this descriptor, in bytes */
    uint8_t bLength;

    /* CS_INTERFACE descriptor type - constant */
    uint8_t bDescriptorType;

    /* MIXER_UNIT descriptor subtype - constant*/
    uint8_t bDescriptorSubtype;

    /* Constant uniquely identifying the Unit within the audio function.*/
    uint8_t bUnitID;

    /* Number of Input Pins of this Unit */
    uint8_t bNrInPins;

    /* This header is followed by USB_AUDIO_MIXER_UNIT_DESCRIPTOR_SOURCE_ID
       type of entries identifying the ID of the entities connected to input pin
       of the mixer. So the first entry is the ID of the entity that is
       connected to the first mixer input pin and so on.*/ 
    
} USB_AUDIO_MIXER_UNIT_DESCRIPTOR_HEADER;

// *****************************************************************************
/* USB Audio Mixer Unit Descriptor Source ID.

  Summary:
   Structure describing USB Audio Mixer unit descriptor Source ID.

  Description:
    This type identifies the type of the USB Audio mixer unit descriptor.  This
    structure is as per Table 5-45 USB Device Class Definition for Audio Device
    v1.0 document.

  Remarks:
    This structure is a part of the USB_AUDIO_MIXER_UNIT_DESCRIPTOR. The
    USB_AUDIO_MIXER_UNIT_DESCRIPTOR cannot be defined as it an open ended data
    structure.
*/

typedef struct
{
    
    /* ID of the Unit or Terminal to which the first Input Pin of this Mixer
       Unit is connected */
    uint8_t baSourceID;

} USB_AUDIO_MIXER_UNIT_DESCRIPTOR_SOURCE_ID;

// *****************************************************************************
/* USB Audio Mixer Unit Descriptor Channel Info.

  Summary:
   Structure describing USB Audio Mixer unit descriptor Channel Info.

  Description:
    This type identifies the type of the USB Audio mixer unit descriptor.  This
    structure is as per Table 5-45 USB Device Class Definition for Audio Device
    v1.0 document.

  Remarks:
    This structure is a part of the USB_AUDIO_MIXER_UNIT_DESCRIPTOR. The
    USB_AUDIO_MIXER_UNIT_DESCRIPTOR cannot be defined as it an open ended data
    structure.
*/

typedef struct
{
    /* Number of logical output channels in the Mixer’s output audio channel
       cluster */
    uint8_t bNrChannels;

    /* Describes the spatial location of the logical channels. */
    uint16_t wChannelConfig;

    /* Index of a string descriptor, describing the name of the first logical
       channel */
    uint8_t iChannelNames;

} USB_AUDIO_MIXER_UNIT_DESCRIPTOR_CHANNEL_INFO;

// *****************************************************************************
/* USB Audio Mixer Unit Descriptor bmaControls.

  Summary:
   Structure describing USB Audio Mixer unit descriptor bmaControls.

  Description:
    This type identifies the type of the USB Audio mixer unit descriptor.  This
    structure is as per Table 5-45 USB Device Class Definition for Audio Device
    v1.0 document.

  Remarks:
    This structure is a part of the USB_AUDIO_MIXER_UNIT_DESCRIPTOR. The
    USB_AUDIO_MIXER_UNIT_DESCRIPTOR cannot be defined as it an open ended data
    structure.
*/

typedef struct
{
    /* Bit map indicating which mixing Controls are programmable. */
    uint8_t bmControls;

}USB_AUDIO_MIXER_UNIT_DESCRIPTOR_BMACONTROLS;

// *****************************************************************************
/* USB Audio Mixer Unit Descriptor Footer.

  Summary:
   Structure describing USB Audio Mixer unit descriptor Footer.

  Description:
    This type identifies the type of the USB Audio mixer unit descriptor.  This
    structure is as per Table 5-45 USB Device Class Definition for Audio Device
    v1.0 document.

  Remarks:
    This structure is a part of the USB_AUDIO_MIXER_UNIT_DESCRIPTOR. The
    USB_AUDIO_MIXER_UNIT_DESCRIPTOR cannot be defined as it an open ended data
    structure.
*/

typedef struct
{
    /* Index of a string descriptor, describing the Mixer Unit. */
    uint8_t iMixer;

}USB_AUDIO_MIXER_UNIT_DESCRIPTOR_FOOTER;
// End file _USBAUDIO20
#endif
