#ifndef   __USB100_H__
#define   __USB100_H__


#include <PSHPACK1.H>

#define MAXIMUM_USB_STRING_LENGTH 255

// values for the bits returned by the USB GET_STATUS command
#define USB_GETSTATUS_SELF_POWERED                0x01
#define USB_GETSTATUS_REMOTE_WAKEUP_ENABLED       0x02


#define USB_DEVICE_DESCRIPTOR_TYPE                0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE         0x02
#define USB_STRING_DESCRIPTOR_TYPE                0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE             0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE              0x05
#define USB_POWER_DESCRIPTOR_TYPE                 0x06
#define USB_OTHER_SPEED_CONF_DESCRIPTOR_TYPE      0x07
#define USB_HID_DESCRIPTOR_TYPE                   0x21
#define USB_CS_INTERRACE_DESCRIPTOR_TYPE          0x24




#define USB_DESCRIPTOR_MAKE_TYPE_AND_INDEX(d, i) ((USHORT)((USHORT)d<<8 | i))

//
// Values for bmAttributes field of an
// endpoint descriptor
//

#define USB_ENDPOINT_TYPE_MASK                    0x03

#define USB_ENDPOINT_TYPE_CONTROL                 0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS             0x01
#define USB_ENDPOINT_TYPE_BULK                    0x02
#define USB_ENDPOINT_TYPE_INTERRUPT               0x03


//
// definitions for bits in the bmAttributes field of a 
// configuration descriptor.
//
#define USB_CONFIG_POWERED_MASK                   0xc0

#define USB_CONFIG_BUS_POWERED                    0x80
#define USB_CONFIG_SELF_POWERED                   0x40
#define USB_CONFIG_REMOTE_WAKEUP                  0x20

//
// Endpoint direction bit, stored in address
//

#define USB_ENDPOINT_DIRECTION_MASK               0x80

// test direction bit in the bEndpointAddress field of
// an endpoint descriptor.
#define USB_ENDPOINT_DIRECTION_OUT(addr)          (!((addr) & USB_ENDPOINT_DIRECTION_MASK))
#define USB_ENDPOINT_DIRECTION_IN(addr)           ((addr) & USB_ENDPOINT_DIRECTION_MASK)

//
// USB defined request codes
// see chapter 9 of the USB 1.0 specifcation for
// more information.
//

// These are the correct values based on the USB 1.0
// specification

#define USB_REQUEST_GET_STATUS                    0x00
#define USB_REQUEST_CLEAR_FEATURE                 0x01

#define USB_REQUEST_SET_FEATURE                   0x03

#define USB_REQUEST_SET_ADDRESS                   0x05
#define USB_REQUEST_GET_DESCRIPTOR                0x06
#define USB_REQUEST_SET_DESCRIPTOR                0x07
#define USB_REQUEST_GET_CONFIGURATION             0x08
#define USB_REQUEST_SET_CONFIGURATION             0x09
#define USB_REQUEST_GET_INTERFACE                 0x0A
#define USB_REQUEST_SET_INTERFACE                 0x0B
#define USB_REQUEST_SYNC_FRAME                    0x0C


//
// defined USB device classes
//


#define USB_DEVICE_CLASS_RESERVED           0x00
#define USB_DEVICE_CLASS_AUDIO              0x01
#define USB_DEVICE_CLASS_COMMUNICATIONS     0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE    0x03
#define USB_DEVICE_CLASS_MONITOR            0x04
#define USB_DEVICE_CLASS_PHYSICAL_INTERFACE 0x05
#define USB_DEVICE_CLASS_POWER              0x06
#define USB_DEVICE_CLASS_PRINTER            0x07
#define USB_DEVICE_CLASS_STORAGE            0x08
#define USB_DEVICE_CLASS_HUB                0x09
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC    0xFF

//
// defined Audio device subclasses
//
#define AUDIO_SUBCLASS_UNDEFINED            0x00
#define AUDIO_SUBCLASS_AUDIOCONTROL         0x01
#define AUDIO_SUBCLASS_AUDIOSTREAMING       0x02
#define AUDIO_SUBCLASS_MIDISTREAMING        0x03

//
// defined Audio Interface Protocol Codes
//
#define AUDIO_PR_PROTOCOL_UNDEFINED         0x00

//
// defined MIDIStreaming(MS) Class-Specific Interface Descriptor Subtypes
//
#define MS_DESCRIPTOR_UNDEFINED             0x00
#define MS_HEADER                           0x01
#define MS_MIDI_IN_JACK                     0x02
#define MS_MIDI_OUT_JACK                    0x03
#define MS_ELEMENT                          0x04

//
// defined MS Class-Specific Endpoint Descriptor Subtypes
//
#define MS_DESCRIPTOR_UNDEFINED             0x00
#define MS_GENERAL                          0x01

//
// defined MS MIDI IN and OUT Jack types
//
#define MS_JACK_TYPE_UNDEFINED              0x00
#define MS_EMBEDDED                         0x01
#define MS_EXTERNAL                         0x02

//
// defined Endpoint Control Selectors
//
#define MS_EP_CONTROL_UNDEFINED             0x00
#define MS_ASSOCIATION_CONTROL              0x01


//
// USB defined Feature selectors
//
#define USB_FEATURE_ENDPOINT_STALL          0x0000
#define USB_FEATURE_REMOTE_WAKEUP           0x0001
#define USB_FEATURE_POWER_D0                0x0002
#define USB_FEATURE_POWER_D1                0x0003
#define USB_FEATURE_POWER_D2                0x0004
#define USB_FEATURE_POWER_D3                0x0005

typedef struct _USB_DEVICE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
} USB_DEVICE_DESCRIPTOR, *PUSB_DEVICE_DESCRIPTOR;

typedef struct _USB_ENDPOINT_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bEndpointAddress;
    UCHAR bmAttributes;
    USHORT wMaxPacketSize;
    UCHAR bInterval;
// ¥ BulkIFTest 1.1
	UCHAR bRefresh;			// Bus Class—p‚ÌŠg’£
	UCHAR bSynchAddress;
// £ BulkIFTest 1.1
} USB_ENDPOINT_DESCRIPTOR, *PUSB_ENDPOINT_DESCRIPTOR;

typedef struct _USB_CONFIGURATION_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT wTotalLength;
    UCHAR bNumInterfaces;
    UCHAR bConfigurationValue;
    UCHAR iConfiguration;
    UCHAR bmAttributes;
    UCHAR MaxPower;
} USB_CONFIGURATION_DESCRIPTOR, *PUSB_CONFIGURATION_DESCRIPTOR;

typedef struct _USB_INTERFACE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bInterfaceNumber;
    UCHAR bAlternateSetting;
    UCHAR bNumEndpoints;
    UCHAR bInterfaceClass;
    UCHAR bInterfaceSubClass;
    UCHAR bInterfaceProtocol;
    UCHAR iInterface;
} USB_INTERFACE_DESCRIPTOR, *PUSB_INTERFACE_DESCRIPTOR;

typedef struct _USB_STRING_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    WCHAR bString[1];
} USB_STRING_DESCRIPTOR, *PUSB_STRING_DESCRIPTOR;

// ¥ BulkIFTest 1.0
typedef struct _USB_OTHER_SPEED_CONF_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
	USHORT wTotalLength;
	UCHAR bNumInterfaces;
	UCHAR bConfigurationValue;
	UCHAR iConfiguration;
	UCHAR bmAttributes;
	UCHAR bMaxPower;
} USB_OTHER_SPEED_CONF_DESCRIPTOR, *PUSB_OTHER_SPEED_CONF_DESCRIPTOR;

typedef struct _USB_HID_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
	USHORT bcdHID;
	UCHAR bCountryCode;
	UCHAR bNumDescriptors;
	UCHAR bReportDescriptorType;
	USHORT wDescriptorLength;
} USB_HID_DESCRIPTOR, *PUSB_HID_DESCRIPTOR;

typedef struct _USB_CS_INTERFACE_DESCRIPTOR {
	UCHAR bLength;
	UCHAR bDescriptorType;
	UCHAR bDescriptorSubType;
	USHORT bcdADC;
	USHORT wTotalLength;
	UCHAR bInCollection;
	// ˆÈ‰ºAbaIntafaceNr(1) ` baInterfaceNr(n)
} USB_CS_INTERFACE_DESCRIPTOR, *PUSB_CS_INTERFACE_DESCRIPTOR;

typedef struct _USB_CS_INTERFACE_HEADER_DESCRIPTORS {
	UCHAR bLength;
	UCHAR bDescriptorType;
	UCHAR bDescriptorSubType;
	USHORT bcdADC;
	USHORT wTotalLength;
	UCHAR bInCollection;
	// ˆÈ‰ºAbaIntafaceNr(1) ` baInterfaceNr(n)
} USB_CS_INTERFACE_DESCRIPTORS, *PUSB_CS_INTERFACE_DESCRIPTORS;
// £ BulkIFTest 1.0

//
// USB power descriptor added to core specification
//

#define USB_SUPPORT_D0_COMMAND      0x01
#define USB_SUPPORT_D1_COMMAND      0x02
#define USB_SUPPORT_D2_COMMAND      0x04
#define USB_SUPPORT_D3_COMMAND      0x08

#define USB_SUPPORT_D1_WAKEUP       0x10
#define USB_SUPPORT_D2_WAKEUP       0x20


typedef struct _USB_POWER_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bCapabilitiesFlags;
    USHORT EventNotification;
    USHORT D1LatencyTime;
    USHORT D2LatencyTime;
    USHORT D3LatencyTime;
    UCHAR PowerUnit;
    USHORT D0PowerConsumption;
    USHORT D1PowerConsumption;
    USHORT D2PowerConsumption;
} USB_POWER_DESCRIPTOR, *PUSB_POWER_DESCRIPTOR;


typedef struct _USB_COMMON_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
} USB_COMMON_DESCRIPTOR, *PUSB_COMMON_DESCRIPTOR;


//
// Standard USB HUB definitions 
//
// See Chapter 11
//

typedef struct _USB_HUB_DESCRIPTOR {
    UCHAR        bDescriptorLength;      // Length of this descriptor
    UCHAR        bDescriptorType;        // Hub configuration type
    UCHAR        bNumberOfPorts;         // number of ports on this hub
    USHORT       wHubCharacteristics;    // Hub Charateristics
    UCHAR        bPowerOnToPowerGood;    // port power on till power good in 2ms
    UCHAR        bHubControlCurrent;     // max current in mA
    //
    // room for 255 ports power control and removable bitmask
    UCHAR        bRemoveAndPowerMask[64];       
} USB_HUB_DESCRIPTOR, *PUSB_HUB_DESCRIPTOR;


#include <POPPACK.H>


#endif   /* __USB100_H__ */
