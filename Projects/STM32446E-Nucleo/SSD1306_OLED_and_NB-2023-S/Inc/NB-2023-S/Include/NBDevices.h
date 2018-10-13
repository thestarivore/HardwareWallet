#ifndef NB_DEVICES_H
#define NB_DEVICES_H

#include <NBDevicesTypes.h>
#include <NBDevice.h>

#ifdef NB_CPP
extern "C"
{
#endif

/*
    Maximum number of devices.
*/
#define NB_DEVICE_MAX_COUNT               256

typedef NBResult(NB_CALLBACK NBDevicesDeviceChangedProcA)(const NBDeviceInfoA * psDeviceInfo, void * pParam);

/*
  Summary:
   NBDevices initialization function.
  Parameters:
   pDeviceAddedProc :    [in] callback to be called when new
                         device is added (NULL allowed)
   pDeviceRemovedProc :  [in] callback to be called when device
                         is removed (NULL allowed)
   pParam :              [in] pass\-through parameter to be
                         passed to pDeviceAddedProc and
                         pDeviceRemovedProc callbacks
   uiFlags :             [in] bitmask specifying flags, which
                         modify slightly behavior of the function
  Remarks:
   NBDevicesInitialize is intended to be called once during
   application initialization and is used to initialize internal
   variables, structures and threads allowing communication with
   devices. If NBDevices initialization was done, termination
   should be done too in order to clean allocated resources and
   exit gracefully by using NBDevicesTerminate function.
   
   Specifying device add/remove callbacks is optional and can be
   replaced with NULL if no callbacks are wanted.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDevicesInitializeA(NBDevicesDeviceChangedProcA pDeviceAddedProc, NBDevicesDeviceChangedProcA pDeviceRemovedProc, void * pParam, NBUInt uiFlags);

/*
  Summary:
   Function to check whether NBDevicesInitializeA was called and
   NBDevices was initialized.
  Parameters:
   pbValue :  [out] result, whether NBDevices is initialized
              (NBTrue in case it is initialized).
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDevicesIsInitialized(NBBool * pbValue);

/*
    Summary:
     Function to terminate NBDevices.
    Remarks:
     Cleans resources allocated for
     \internal variables and threads. Will not return error if the
     NBDevices is not initialized.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDevicesTerminate(void);

/*
  Summary:
   Function is used to retrieve the list of current connected
   devices. Function retrieves the list of device descriptors,
   that can be used to call NBDeviceConnectA to connect to the
   device descriptor specified device and obtain device handle.
  Parameters:
   arsDevices :       [in/out] pre\-allocated array to store
                      device descriptors to
   uiDevicesLength :  [in] pre\-allocated array length
   puiCount :         [out] \- actual count of connected devices
  Remarks:
   Function requires caller to allocate arsDevices.
   NBDevicesGetDevicesA only fills descriptor information into
   pre-allocated array. NBDevicesGetDevicesA accepts (arsDevices
   = NULL AND uiDevicesLength = 0 (in order for caller to find
   out how many devices are connected and then pre-allocate
   array before passing for second time to
   NBDevicesGetDevicesA)) OR (arsDevices != NULL AND
   uiDevicesLength \>= 0). In order to simplify mentioned
   procedure, NB_DEVICE_MAX_COUNT macro can be used to
   pre-allocate descriptor array on stack.
   
   uiDevicesLength can be lower than puiCount.
  Returns:
   If the function succeeds, the return value is NB_OK. If the
   function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDevicesGetDevicesA(NBDeviceInfoA * arsDevices, NBUInt uiDevicesLength, NBUInt * puiCount);

#ifdef NB_CPP
}
#endif

#endif // !NB_DEVICES_H
