#ifndef NB_DEVICES_TYPES_H
#define NB_DEVICES_TYPES_H

#include <NBTypes.h>
#include <NBErrors.h>

#ifdef NB_CPP
extern "C"
{
#endif

#ifdef NB_EMBEDDED
    #define NB_DEVICES_LITE
#endif

/*
   Declaration of a NBDevice device handle
*/
typedef NBHandle HNBDevice;

/*
    Enumeration of device types
*/
typedef enum
{
    /* Unknown device type */
    NBDeviceTypeUnknown = 0,
    /* NB 1010 SPI device */
    NBDeviceTypeNB1010S = 100,
    /* NB 1020 SPI device */
    NBDeviceTypeNB1020S = 101,
    /* NB 2020 SPI device */
    NBDeviceTypeNB2020S = 102,
    /* NB 2021 SPI device */
    NBDeviceTypeNB2021S = 103,
    /* NB 2022 SPI device */
    NBDeviceTypeNB2022S = 104,
    /* NB 2034 SPI device */
    NBDeviceTypeNB2034S = 105,
    /* NB 1010 USB device */
    NBDeviceTypeNB1010U = 200,
    /* NB 1020 USB device */
    NBDeviceTypeNB1020U = 201,
    /* NB 2020 USB device */
    NBDeviceTypeNB2020U = 202,
    /* NB 2024 USB device */
    NBDeviceTypeNB2024U = 203,
    /* NB 3023 USB device */
    NBDeviceTypeNB3023U = 204,
    /* NB 2023 USB device */
    NBDeviceTypeNB2023U = 205,
} NBDeviceType;

/*
    Enumeration of device connection types
*/
typedef enum
{
    /* Unknown device connection type */
    NBDeviceConnectionTypeUnknown = 0,
    /* USB device connection type */
    NBDeviceConnectionTypeUsb = 1,
    /* SPI device connection type */
    NBDeviceConnectionTypeSpi = 2,
} NBDeviceConnectionType;

/*
    Enumeration of device states
*/
typedef enum
{
    /* Device is not connected (disconnected, NBDeviceDestroy is requested) */
    NBDeviceStateNotConnected = 0,
    /* Device is not awake (NBDeviceReset to wake device) */
    NBDeviceStateNotAwake = 1,
    /* Device is awake */
    NBDeviceStateAwake = 2
} NBDeviceState;

/*
    Enumeration of supported fingerprint detection types
*/
typedef enum
{
    /* Baseline */
    NBDeviceFingerDetectTypeBaseline = 0,
    /* Enhanced finger on */
    NBDeviceFingerDetectTypeEnhanced = 1,
    /* Quick (capacitive touch score) */
    NBDeviceFingerDetectTypeQuick = 2,
    /* Last wake (enhanced score following wake up) */
    NBDeviceFingerDetectTypeLastWake = 3
} NBDeviceFingerDetectType;

/*
    Enumeration of supported scan formats
*/
typedef enum
{
    /* 12 mm x 17 mm format, width 180 pixels, height 256 pixels, 385 dpi */
    NBDeviceScanFormat12x17 = 0,
    /* 12 mm x 16 mm format, width 180 pixels, height 244 pixels, 385 dpi */
    NBDeviceScanFormat12x16 = 1,
    /* partial format, width 90 pixels, height 128 pixels, 385 dpi */
    NBDeviceScanFormatPartial = 2,
    /* 12 mm x 12 mm format, width 180 pixels, height 180 pixels, 385 dpi */
    NBDeviceScanFormat12x12 = 3,
    /* 12 mm x 17 mm format, width 234 pixels, height 332 pixels, upscaled to 500 dpi */
    NBDeviceScanFormat12x17_500 = 4,
    /* 12 mm x 16 mm format, width 234 pixels, height 317 pixels, upscaled to 500 dpi */
    NBDeviceScanFormat12x16_500 = 5,
    /* partial format, width 117 pixels, height 166 pixels, upscaled to 500 dpi */
    NBDeviceScanFormatPartial_500 = 6,
    /* 12 mm x 12 mm format, width 234 pixels, height 234 pixels, upscaled to 500 dpi */
    NBDeviceScanFormat12x12_500 = 7,
    /* quarter partial format, width 22 pixels, height 128 pixels, 385 dpi */
    NBDeviceScanFormatQuarterPartial = 8,
    /* quarter partial format, width 29 pixels, height 166 pixels, upscaled to 500 dpi */
    NBDeviceScanFormatQuarterPartial_500 = 9
} NBDeviceScanFormat;

/*
  Enumeration of scan format types
*/
typedef enum
{
    /*
      Native (to device) scan format type
    */
    NBDeviceScanFormatTypeNative = 0,
    /*
      Upscaled (from native) scan format type
    */
    NBDeviceScanFormatTypeUpscaled = 1,
    /*
      Optical equivalent scan format type
    */
    NBDeviceScanFormatTypeOpticalEquivalent = 2
} NBDeviceScanFormatType;

/*
  Structure holding extended information about scan format -
  scan format, scan format type, width, height, horizontal and
  vertical resolution                                         
*/
typedef struct
{
    /* Scan format */
    NBDeviceScanFormat eScanFormat;
    /* Scan format type */
    NBDeviceScanFormatType eScanFormatType;
    /* Width (pixels) */
    NBUInt uiWidth;
    /* Height (pixels) */
    NBUInt uiHeight;
    /* Horizontal resolution (dpi) */
    NBUInt uiHorizontalResolution;
    /* Vertical resolution (dpi) */
    NBUInt uiVerticalResolution;
} NBDeviceScanFormatInfo;

/*
    Enumeration of scan result statuses
*/
typedef enum
{
    /* Status is not set (None) */
    NBDeviceScanStatusNone = 0,
    /* Capture is successful */
    NBDeviceScanStatusOk = 1,
    /* Capture is canceled */
    NBDeviceScanStatusCanceled = 2,
    /* Timeout occurred */
    NBDeviceScanStatusTimeout = 3,
    /* No finger detected */
    NBDeviceScanStatusNoFinger = 4,
    /* Finger is present before scan start */
    NBDeviceScanStatusNotRemoved = 5,
    /* Bad fingerprint quality */
    NBDeviceScanStatusBadQuality = 6,
    /* Bad fingerprint size */
    NBDeviceScanStatusBadSize = 7
} NBDeviceScanStatus;

/* Maximum length for various strings used for
   device descriptions (e.g. device manufacturer or model)      */
#define NB_DEVICE_STRING_MAX_LENGTH 256

/*
    Device descriptor holding information about a device - device
    index, device ID, manufacturer, model, serial number, device
    type, firmware version                                       
*/           
typedef struct
{
    /* Internal device index */
    NBUInt uiDeviceIndex;
    /* Device id */
    NBAChar szId[NB_DEVICE_STRING_MAX_LENGTH];
    /* Device manufacturer */
    NBAChar szManufacturer[NB_DEVICE_STRING_MAX_LENGTH];
    /* Device model */
    NBAChar szModel[NB_DEVICE_STRING_MAX_LENGTH];
    /* Device serial number */
    NBAChar szSerialNumber[NB_DEVICE_STRING_MAX_LENGTH];
    /* Device type */
    NBDeviceType eType;
    /* Device firmware version */
    NBVersion sFirmwareVersion;
} NBDeviceInfoA;

/*
  Enumeration of stop modes that device could enter
*/
typedef enum
{
    /* Continuous finger detection */
    NBDeviceStopModeContinuousFingerDetection = 0,
    /* Hard stop (go directly to lowest power state) */
    NBDeviceStopModeHardStop = 1,
    /* Perform 1 .. n finger detections */
    NBDeviceStopModeCountedFingerDetection = 2
} NBDeviceStopMode;

/*
  Enumeration of LED states
*/
typedef enum
{
    /* LED off */
    NBDeviceLedStateOff = 0,
    /* LED flashes 0.3s ON, 0.3s OFF, and repeats */
    NBDeviceLedStateFingerOnRequested = 1,
    /* LED ON solid */
    NBDeviceLedStateInScan = 4,
    /* LED flashes Long ON, Short OFF, and repeats */
    NBDeviceLedStateCustom = 6
} NBDeviceLedState;

/* Additional information for scan preview */
typedef struct
{
    /* Finger detection value */
    NBInt iFingerDetectValue;
} NBDeviceScanPreviewDetails;

typedef NBResult(NB_CALLBACK NBDeviceScanPreviewProc)(const HNBDevice hDevice, NBDeviceScanStatus * peStatus, const NBDeviceScanFormatInfo * psScanFormatInfo, const NBByte * pBuffer, NBSizeType stBufferSize, const NBDeviceScanPreviewDetails * psDetails, void * pParam);

#ifdef NB_DEVICES_LITE

/*
  Enumeration of allowed PIN values
*/
typedef enum
{
    /*
      UNKNOWN pin value
    */
    NBDevicePinValueUnknown = 0,
    /*
      LOW pin value
    */
    NBDevicePinValueLow = 1,
    /*
      HIGH pin value
    */
    NBDevicePinValueHigh = 2
} NBDevicePinValue;

typedef NBResult(NB_CALLBACK NBDeviceDestroyContextProc)(void * pContext);
typedef NBResult(NB_CALLBACK NBDeviceDelayMicrosecondsProc)(void * pContext, NBUInt32 ui32Value); // delay (sleep) in microseconds
typedef NBResult(NB_CALLBACK NBDeviceGetTimestampProc)(void * pContext, NBUInt32 * pu32Value); // return timestamp in milliseconds
typedef NBResult(NB_CALLBACK NBDeviceResetSetValueProc)(void * pContext, NBDevicePinValue eValue);
typedef NBResult(NB_CALLBACK NBDeviceAwakeGetValueProc)(void * pContext, NBDevicePinValue * peValue);
typedef NBResult(NB_CALLBACK NBDeviceSendReceiveDataProc)(void * pContext, NBUInt8 * pui8TxBuffer, NBUInt16 ui16TxLength, NBUInt8 * pui8RxBuffer, NBUInt16 ui16RxLength);

/*
  Structure holding communication abstraction primitives (BSP)
  that should be implemented by board                         
*/
typedef struct
{
    /*
      Specified whether HIGH awake pin value should be treated as
      connected device is awake (consult device specification for
      correct value)                                             
    */
    NBBool bIsAwakeHigh;

    /*
      User context that will be passed to all abstraction primitive
      functions (can be NULL)                                      
    */
    void * pContext;
    /*
      Function that should destroy(free) specified context (required if
      pContext is not NULL, otherwise optional)               
    */
    NBDeviceDestroyContextProc pDestroyContext;

    /*
      Function to delay (sleep) in milliseconds (required)
    */
    NBDeviceDelayMicrosecondsProc pDelayMicroseconds;
    /*
      Function to get current timestamp in milliseconds (required)
    */
    NBDeviceGetTimestampProc pGetTimestamp;
    /*
      Function to set reset pin value (required)
    */
    NBDeviceResetSetValueProc pResetSetValue;
    /*
      Function to get current awake pin value (required)
    */
    NBDeviceAwakeGetValueProc pAwakeGetValue;
    /*
      Function to send and receive data (required)
    */
    NBDeviceSendReceiveDataProc pSendReceiveData;
} NBDeviceIO;

#endif

#ifdef NB_CPP
}
#endif

#endif // !NB_DEVICES_TYPES_H
