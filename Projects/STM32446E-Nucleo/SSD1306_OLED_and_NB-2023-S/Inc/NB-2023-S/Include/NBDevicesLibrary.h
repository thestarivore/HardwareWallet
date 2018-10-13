#ifndef NB_DEVICES_LIBRARY_H
#define NB_DEVICES_LIBRARY_H

#include <NBTypes.h>
#include <NBErrors.h>
#include <NBDevices.h>
#include <NBDevicesTypes.h>
#include <NBDevice.h>

#ifdef NB_CPP
extern "C"
{
#endif

/*
    Summary:
     Function to version of current loaded library.
    Parameters:
     psVersion :  [out] result, library version
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBDevicesLibraryGetVersion(NBVersion * psVersion);

#ifdef NB_CPP
}
#endif

#endif // !NB_DEVICES_LIBRARY_H
