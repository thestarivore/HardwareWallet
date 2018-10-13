#ifndef NB_DEVICE_H
#define NB_DEVICE_H

#include <NBDevicesTypes.h>

#ifdef NB_CPP
extern "C"
{
#endif

/* Default finger detect type */
#define NB_DEVICE_DEFAULT_FINGER_DETECT_TYPE                    NBDeviceFingerDetectTypeEnhanced
/* Default finger detection threshold (above threshold, image is considered to have a fingerprint present) */
#define NB_DEVICE_DEFAULT_FINGER_DETECT_THRESHOLD               40
/* Infinite threshold is used in extended capture and means that the operation will either
   succeed or continue until canceled.                      */
#define NB_DEVICE_SCAN_TIMEOUT_INFINITE                         -1
/* Skip the finger not remove status when doing scan (in scenarios when sensor is awaken from sleep after finger is put on sensor area (in SPI)) */
#define NB_DEVICE_SCAN_SKIP_FINGER_NOT_REMOVED_STATUS_FLAG       0x08
/*
  Do not do GPIO initialization (and deinitialization) when
  connecting to device module through SPI                  
*/
#define NB_DEVICE_CONNECT_TO_SPI_SKIP_GPIO_INIT_FLAG             0x02

/*
 * Device manipulation
 */

 
/*
 * Connecting and destroying connection to the device
 */
 
/*
  Summary:
   Connect to a device using specified device descriptor
   (NBDeviceInfoA) obtained from either by calling
   NBDevicesGetDevicesA or through NBDevicesDeviceChangedProcA
   callback.
  Parameters:
   psDeviceInfo :  [in] device descriptor
   uiFlags :       [in] bitmask specifying flags, which modify
                   slightly behavior of the function
   phDevice :      [out] handle to device
  Remarks:
   Device handle is allocated on heap and returned after
   successful execution of the function.
   
   Each obtained device handle needs to be destroyed using
   NBDeviceDestroy function.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceConnectA(const NBDeviceInfoA * psDeviceInfo, NBUInt uiFlags, HNBDevice * phDevice);

/*
  Summary:
   Connect to a device using specified SPI name, awake and reset
   pin number.
  Parameters:
   szSpi :      [in] name of SPI device
   iAwakePin :  [in] awake pin number
   iResetPin :  [in] reset pin number
   uiFlags :    [in] bitmask specifying flags, which modify
                slightly behavior of the function
   phDevice :   [out] handle to device
  Remarks:
   Device handle is allocated on heap and returned after
   successful execution of the function.
   
   Each obtained device handle needs to be destroyed using
   NBDeviceDestroy function.
   
   If SPI is not supported on current platform,
   NB_ERROR_NOT_SUPPORTED error will be returned.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceConnectToSpiA(const NBAChar * szSpi, NBInt iAwakePin, NBInt iResetPin, NBUInt uiFlags, HNBDevice * phDevice);
/*
  Summary:
   Connect to a device using specified SPI name, awake, reset,
   and chip select pin number.
  Parameters:
   szSpi :           [in] name of SPI device
   iAwakePin :       [in] awake pin number
   iResetPin :       [in] reset pin number
   iChipSelectPin :  [in] chip select pin number (\-1 is allowed,
                     meaning not specified)
   uiFlags :         [in] bitmask specifying flags, which modify
                     slightly behavior of the function
   phDevice :        [out] handle to device
  Remarks:
   Device handle is allocated on heap and returned after
   successful execution of the function.
   
   Each obtained device handle needs to be destroyed using
   NBDeviceDestroy function.
   
   If SPI is not supported on current platform,
   NB_ERROR_NOT_SUPPORTED error will be returned.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceConnectToSpiExA(const NBAChar * szSpi, NBInt iAwakePin, NBInt iResetPin, NBInt iChipSelectPin, NBUInt uiFlags, HNBDevice * phDevice);

#ifdef NB_DEVICES_LITE
    /*
      Summary:
       Connect to a device using specified platform/board
       abstraction structure.
      Parameters:
       psDeviceIO :  [in] platform abstraction structure (BSP), will
                     be copied internally
       uiFlags :     [in] bitmask specifying flags, which modify
                     slightly behavior of the function
       phDevice :    [out] handle to device
      Remarks:
       Device handle is allocated on heap and is returned after
       successful execution of the function.
       
       Each obtained device handle needs to be destroyed using
       NBDeviceDestroy function.
       
       If SPI is not supported on current platform,
       NB_ERROR_NOT_SUPPORTED error will be returned.
      Returns:
       If the function succeeds, the return value is NB_OK. If the
       function fails, one of the error codes is returned (<link Error codes>).
    */
    NBResult NB_API NBDeviceConnectToSpiRaw(const NBDeviceIO * psDeviceIO, NBUInt uiFlags, HNBDevice * phDevice);
    /*
      Summary:
       Connect to a device using specified platform/board
       abstraction structure, created handle will be allocated in
       specified memory buffer.
      Parameters:
       psDeviceIO :    [in] platform abstraction structure (BSP),
                       will be copied internally
       pBuffer :       [in/out] memory buffer, where device handle
                       will be allocated in
       stBufferSize :  [in] size of the memory buffer
       uiFlags :       [in] bitmask specifying flags, which modify
                       slightly behavior of the function
       pstSize :       [out] minimal memory size required for buffer
       phDevice :      [out] handle to device
      Remarks:
       Device handle is allocated in specified memory buffer and
       returned after successful execution of the function.
       
       pBuffer must exist until device handle is not destroyed
       (using NBDeviceDestroy).
       
       Each obtained device handle needs to be destroyed using
       NBDeviceDestroy function.
       
       If SPI is not supported on current platform,
       NB_ERROR_NOT_SUPPORTED error will be returned.
      Returns:
       If the function succeeds, the return value is NB_OK. If the
       function fails, one of the error codes is returned (<link Error codes>).
    */
    NBResult NB_API NBDeviceConnectToSpiRawInMemory(const NBDeviceIO * psDeviceIO, NBByte * pBuffer, NBSizeType stBufferSize, NBUInt uiFlags, NBSizeType * pstSize, HNBDevice * phDevice);
#endif

/*
  Summary:
   Destroys device handle obtained using NBDeviceConnectA,
   NBDeviceConnectToSpiA, NBDeviceConnectToSpiExA,
   NBDeviceConnectToSpiRaw or NBDeviceConnectToSpiRawInMemory
   functions and frees all resources obtained by device handle.
  Parameters:
   hDevice :  [in] handle to device
  Remarks:
   After handle is destroyed, it is caller's responsibility to
   set the device handle to NULL. Calling NBDevice functions
   with a destroyed handle can give unpredictable results.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceDestroy(HNBDevice hDevice);

/*
 * Getting device state, resetting connection and parameters
 */
 
 /*
     Summary:
      Get current device state.
     Parameters:
      hDevice :        [in] handle to device
      peDeviceState :  [out] result, current device state
     Remarks:
      If returned device state is NBDeviceStateNotConnected -
      device is disconnected and removed from the system. In this
      case caller needs to destroy handle to device using
      NBDeviceDestroy.
      
      If returned device state is NBDeviceStateNotAwake - device is
      in sleep mode and can be awaken by using NBDeviceReset
      function.
     Returns:
      If the function succeeds, the return value is NB_OK. If the
      function fails, one of the error codes is returned (<link Error codes>).
 */
NBResult NB_API NBDeviceGetState(const HNBDevice hDevice, NBDeviceState * peDeviceState);

/*
  Summary:
   Soft resets connected device, awakes device if it is in
   NBDeviceStateNotAwake state (i.e. low power mode, see
   NBDeviceEnterStopMode).
  Parameters:
   hDevice :  [in] Handle to device
  Remarks:
   Current device state can be obtained using NBDeviceGetState
   method.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceReset(const HNBDevice hDevice);

/*
 *Getting device type, description, supported scan formats
 */
 
/*
    Summary:
     Get device type.
    Parameters:
     hDevice :       [in] handle to device
     peDeviceType :  [out] result, device type
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetType(const HNBDevice hDevice, NBDeviceType * peDeviceType);

/*
    Summary:
     Get device connection type.
    Parameters:
     hDevice :       [in] handle to device
     peDeviceType :  [out] result, device connection type
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetConnectionType(const HNBDevice hDevice, NBDeviceConnectionType * peConnectionType);

/*
    Summary:
     Get version of firmware present in the device.
    Parameters:
     hDevice :    [in] handle to device
     psVersion :  [out] result, firmware version
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetFirmwareVersion(const HNBDevice hDevice, NBVersion * psVersion);

/*
    Summary:
     Get device id.
    Parameters:
     hDevice :         [in] handle to device
     szValue :         [in/out] pre\-allocated buffer to store
                       device id
     uiValueLength :   [in] pre\-allocated buffer length
     puiValueLength :  [out] actual length of device id (not
                       including null\-terminator)
    Remarks:
     Before calling NBDeviceGetIdA to get device id, actual length
     of device id needs to be determined. To do that,
     NBDeviceGetIdA needs to be called with szValue set to NULL
     and uiValueLength set to 0. This way, puiValueLength will
     return actual length of device id (without the trailing
     null-termination symbol). Afterwards, caller allocates
     puiValueLength + 1 size of buffer and calls NBDeviceGetIdA
     with specifying pre-allocated buffer and its size,
     NBDevicesGetIdA will fill device id into specified buffer.
     
     NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
     specified buffer is not sufficient to store function result.
     
     NB_DEVICE_STRING_MAX_LENGTH can be used to pre-allocate
     buffer (on heap or stack), thus removing initial call to
     NBDeviceGetIdA.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetIdA(const HNBDevice hDevice, NBAChar * szValue, NBUInt uiValueLength, NBUInt * puiValueLength);

/*
    Summary:
     Get device manufacturer.
    Parameters:
     hDevice :         [in] handle to device
     szValue :         [in/out] pre\-allocated buffer to store
                       device manufacturer
     uiValueLength :   [in] pre\-allocated buffer length
     puiValueLength :  [out] actual length of device manufacturer
                       (not including null\-terminator)
    Remarks:
     Before calling NBDeviceGetManufacturerA to get device
     manufacturer, actual length of device manufacturer needs to
     be determined. To do that, NBDeviceGetManufacturerA needs to
     be called with szValue set to NULL and uiValueLength set to
     \0. This way, puiValueLength will return actual length of
     device manufacturer (without the trailing null-termination
     symbol). Afterwards, caller allocates puiValueLength + 1 size
     of buffer and calls NBDeviceGetManufacturerA with specifying
     pre-allocated buffer and its size, NBDeviceGetManufacturerA
     will fill device manufacturer into specified buffer.
     
     NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
     specified buffer is not sufficient to store function result.
     
     NB_DEVICE_STRING_MAX_LENGTH can be used to pre-allocate
     buffer (on heap or stack), thus removing initial call to
     NBDeviceGetManufacturerA.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetManufacturerA(const HNBDevice hDevice, NBAChar * szValue, NBUInt uiValueLength, NBUInt * puiValueLength);

/*
  Summary:
   Get device model.
  Parameters:
   hDevice :         [in] handle to device
   szValue :         [in/out] pre\-allocated buffer to store
                     device model
   uiValueLength :   [in] pre\-allocated buffer length
   puiValueLength :  [out] actual length of device model (not
                     including null\-terminator)
  Remarks:
   Before calling NBDeviceGetModelA to get device model, actual
   length of device model needs to be determined. To do that,
   NBDeviceGetModelA needs to be called with szValue set to NULL
   and uiValueLength set to 0. This way, puiValueLength will
   return actual length of device model (without the trailing
   null-termination symbol). Afterwards, caller allocates
   puiValueLength + 1 size of buffer and calls NBDeviceGetModelA
   with specifying pre-allocated buffer and its size,
   NBDeviceGetModelA will fill device model into specified
   buffer.
   
   NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
   specified buffer is not sufficient to store function result.
   
   NB_DEVICE_STRING_MAX_LENGTH can be used to pre-allocate
   buffer (on heap or stack), thus removing initial call to
   NBDeviceGetModelA.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetModelA(const HNBDevice hDevice, NBAChar * szValue, NBUInt uiValueLength, NBUInt * puiValueLength);

/*
  Summary:
   Get serial number for connected device.
  Parameters:
   hDevice :         [in] handle to device
   szValue :         [in/out] pre\-allocated buffer to store
                     device serial number
   uiValueLength :   [in] pre\-allocated buffer length
   puiValueLength :  [out] actual length of device serial number
                     (not including null\-terminator)
  Remarks:
   Before calling NBDeviceGetSerialNumberA to get device serial
   number, actual length of device serial number needs to be
   determined. To do that, NBDeviceGetSerialNumberA needs to be
   called with szValue set to NULL and uiValueLength set to 0.
   This way, puiValueLength will return actual length of device
   serial number (without the trailing null-termination symbol).
   Afterwards, caller allocates puiValueLength + 1 size of
   buffer and calls NBDeviceGetSerialNumberA with specifying
   pre-allocated buffer and its size, NBDeviceGetSerialNumberA
   will fill device serial number into specified buffer.
   
   NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
   specified buffer is not sufficient to store function result.
   
   NB_DEVICE_STRING_MAX_LENGTH can be used to pre-allocate
   buffer (on heap or stack), thus removing initial call to
   NBDeviceGetSerialNumberA.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetSerialNumberA(const HNBDevice hDevice, NBAChar * szValue, NBUInt uiValueLength, NBUInt * puiValueLength);

/*
  Summary:
   Get product name for connected device.
  Parameters:
   hDevice :         [in] handle to device
   szValue :         [in/out] pre\-allocated buffer to store
                     device product name
   uiValueLength :   [in] pre\-allocated buffer length
   puiValueLength :  [out] actual length of device product name
                     (not including null\-terminator)
  Remarks:
   Before calling NBDeviceGetProductA to get device product
   name, actual length of device product name needs to be
   determined. To do that, NBDeviceGetProductA needs to be
   called with szValue set to NULL and uiValueLength set to 0.
   This way, puiValueLength will return actual length of device
   product name (without the trailing null-termination symbol).
   Afterwards, caller allocates puiValueLength + 1 size of
   buffer and calls NBDeviceGetProductA with specifying
   pre-allocated buffer and its size, NBDeviceGetProductA will
   fill device product name into specified buffer.
   
   NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
   specified buffer is not sufficient to store function result.
   
   NB_DEVICE_STRING_MAX_LENGTH can be used to pre-allocate
   buffer (on heap or stack), thus removing initial call to
   NBDeviceGetProductA.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetProductA(const HNBDevice hDevice, NBAChar * szValue, NBUInt uiValueLength, NBUInt * puiValueLength);

/*
    Summary:
     Check if specified scan format is supported by device.
    Parameters:
     hDevice :        [in] handle to device
     eScanFormat :    [in] desired device scan format
     pbIsSupported :  [out] result, whether specified desired device
                      scan format is supported
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceIsScanFormatSupported(const HNBDevice hDevice, NBDeviceScanFormat eScanFormat, NBBool * pbIsSupported);

/*
    Summary:
     Get list of supported scan formats by specified device.
    Parameters:
     hDevice :              [in] handle to device
     areScanFormats :       [in/out] pre\-allocated array of
                            NBDeviceScanFormat
     uiScanFormatsLength :  [in] pre\-allocated array length
     puiCount :             [out] actual count of supported device
                            scan formats
    Remarks:
     Function requires caller to allocate areScanFormats.
     NBDeviceGetSupportedScanFormats only fills descriptor
     information into pre-allocated array.
     NBDeviceGetSupportedScanFormats accepts areScanFormats = NULL
     and uiScanFormatsLength = 0 in order for caller to find out
     how many scan formats are available and then pre-allocate array
     before passing for second time to
     NBDeviceGetSupportedScanFormats.
     
     NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
     specified buffer is not sufficient to store function result.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetSupportedScanFormats(const HNBDevice hDevice, NBDeviceScanFormat * areScanFormats, NBUInt uiScanFormatsLength, NBUInt * puiCount);
/*
    Summary:
     Retrieve extended information about scan format.
    Parameters:
     hDevice :           [in] handle to device
     eScanFormat :       [in] desired scan format
     psScanFormatInfo :  [out] result, extended information about
                         specified scan format
    Remarks:
     If specified desired scan format is not supported,
     NB_ERROR_NOT_SUPPORTED is returned (to check whether device
     format is supported, NBDeviceIsScanFormatSupported function
     can be used).
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetScanFormatInfo(const HNBDevice hDevice, NBDeviceScanFormat eScanFormat, NBDeviceScanFormatInfo * psScanFormatInfo);

/*
 * Getting/Setting device parameters
 */

/*
    Summary:
     Set value for specified parameter.
    Parameters:
     hDevice :      [in] handle to device
     uiParameter :  [in] id of parameter
     iValue :       [in] value for specified parameter
    Remarks:
     Parameter value is applied only for specified device (device handle).
     Currently no public parameters are supported.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceSetParameter(const HNBDevice hDevice, NBUInt uiParameter, NBInt iValue);
/*
    Summary:
     Get value of specified parameter.
    Parameters:
     hDevice :      [in] handle to device
     uiParameter :  [in] id of parameter
     piValue :      [out] result, value for specified parameter
    Remarks:
     Currently no public parameters are supported.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceGetParameter(const HNBDevice hDevice, NBUInt uiParameter, NBInt * piValue);

/*
 * Getting hardware fingerprint detect value, image acquisition
 */
 
/*
  Summary:
   Retrieve value representing whether there is finger present
   on the surface of the device.
  Parameters:
   hDevice :      [in] handle to device
   eDetectType :  [in] finger detection type
   piValue :      [out] result, value representing
                  score/probability that finger is present on
                  surface of the device (range\: 0\-255)
  Remarks:
   NB_DEVICE_DEFAULT_FINGER_DETECT_TYPE can be used to select
   default finger detect type to be used to detect whether
   finger is present on device surface.
   
   NB_DEVICE_DEFAULT_FINGER_DETECT_THRESHOLD can be used as
   threshold, defining that any score above specified threshold
   can be considered as meaning that there is finger present on
   device surface.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/ 
NBResult NB_API NBDeviceGetFingerDetectValue(const HNBDevice hDevice, NBDeviceFingerDetectType eDetectType, NBInt * piValue);
/*
  Summary:
   Scan snapshot from device scanning surface.
  Parameters:
   hDevice :       [in] handle to device
   eScanFormat :   [in] desired device scan format
   pBuffer :       [in/out] pre\-allocated buffer to store result
                   image
   stBufferSize :  [in] pre\-allocated buffer size
   uiFlags :       [in] bitmask specifying flags, which modify
                   slightly behavior of the function
   peStatus :      [out] result, additional information
                   specifying status of scan procedure
  Remarks:
   NB_ERROR_NOT_SUPPORTED error will be returned if specified
   desired scan format is not supported.
   
   pBuffer needs to be pre-allocated by function caller and its
   size should be equals or higher than (width x height) of
   desired scan format.
   
   peStatus returns status describing scanned snapshot, however
   it provides only additional feedback and pBuffer will still
   contain image captured even if peStatus status is not
   NBDeviceScanStatusOk.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceScan(const HNBDevice hDevice, NBDeviceScanFormat eScanFormat, NBByte * pBuffer, NBSizeType stBufferSize, NBUInt uiFlags, NBDeviceScanStatus * peStatus);
/*
  Summary:
   Extended scanning, blocks until best possible fingerprint
   image is captured or operation is timeout/canceled.
  Parameters:
   hDevice :       [in] handle to device
   eScanFormat :   [in] desired device scan format
   iTimeout :      [in] timeout in milliseconds
   pPreviewProc :  [in] callback to be called when new preview
                   image is captured (optional)
   pParam :        [in] pass\-through parameter to be passed to
                   pPreviewProc on new preview event (optional)
   pBuffer :       [in/out] pre\-allocated buffer to store result
                   image
   stBufferSize :  [in] pre\-allocated buffer size
   uiFlags :       [in] bitmask specifying flags, which modify
                   slightly behavior of the function
   peStatus :      [out] result, information specifying status of
                   scan procedure
  Remarks:
   Function blocks execution and does scanning until image of
   satisfactory parameters is acquired, or set timeout expires,
   or scanning is canceled.
   
   NB_DEVICE_SCAN_TIMEOUT_INFINITE can be used to indicate
   infinite scan timeout.
   
   NB_ERROR_NOT_SUPPORTED error will be returned if specified
   desired scan format is not supported.
   
   If uiTimeout is set to 0, NBDeviceScanEx returns after first
   image scan.
   
   pBuffer needs to be pre-allocated by function caller and its
   size should be equals or higher than (width x height) of
   desired scan format.
   
   peStatus returns status describing function execution.
   pBuffer will still contain last image captured (if it was
   captured) even if peStatus status is not
   NBDeviceScanStatusOk.
   
   pPreviewProc and pParam can be NULL if no preview is
   requested.
   
   Preview enables previewing the image and possibility to
   override specified status (e.g. accept images with lower
   quality, or requiring even higher image quality).
   
   NB_DEVICE_FLAG_SCAN_SKIP_FINGER_NOT_REMOVED_STATUS flag,
   passed as uiFlags parameter, disables the check of presence
   of the fingerprint during scan initialization (resulting in
   NBDeviceScanStatusNotRemoved status) and can often be used
   after the device is awaken from sleep on finger detect (after
   NBDeviceEnterStopMode with SPI NBDevice).
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceScanEx(const HNBDevice hDevice, NBDeviceScanFormat eScanFormat, NBInt iTimeout, NBDeviceScanPreviewProc pPreviewProc, void * pParam, NBByte * pBuffer, NBSizeType stBufferSize, NBUInt uiFlags, NBDeviceScanStatus * peStatus);

/*
  Summary:
   Check if there is scan in progress.
  Parameters:
   hDevice :  [in] handle to device
   pbValue :  [out] result, whether scan is in
              progress
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceIsScanRunning(const HNBDevice hDevice, NBBool * pbValue);
/*
    Summary:
     Cancel on-going extended scan operation.
    Parameters:
     hDevice :  [in] handle to device
    Remarks:
     If no scan operation is in progress,
     NBDeviceCancelScan function will succeed and won't return any
     error.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceCancelScan(const HNBDevice hDevice);

/*
  Summary:
   Method puts device/module in a low power state.
  Parameters:
   hDevice :                 [in] handle to device
   eStopMode :               [in] type of low power state mode to
                             enter
   uiFingerDetectionCount :  [in] finger detections to perform
                             before leaving the low power mode
                             (only to be used with
                             NBDeviceStopModeCountedFingerDetection)
  Remarks:
   Method halts device internal clocks putting the device to a
   low power state. Stop modes allow to change the behaviour of
   the device during low power state. If a finger is detected,
   device status pin is set and the module leaves sleep mode.
   Without a finger on the sensor, the application can do
   NBDeviceReset to leave the low power mode.
   
   Method is valid only for SPI devices, NB_ERROR_NOT_SUPPORTED
   error will be returned if the specified device is not SPI
   device.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceEnterStopMode(const HNBDevice hDevice, NBDeviceStopMode eStopMode, NBUInt uiFingerDetectionCount);

/*
  Summary:
   Method sets device/module LED.
  Parameters:
   hDevice :    [in] handle to device
   eLedState :  [in] type of LED pattern to set
  Remarks:
   Method is valid only for USB devices, NB_ERROR_NOT_SUPPORTED
   error will be returned if the specified device is not USB
   device or USB device does not support setting LED pattern.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDeviceSetLedState(const HNBDevice hDevice, NBDeviceLedState eLedState);

#ifdef NB_CPP
}
#endif

#endif // !NB_DEVICE_H
