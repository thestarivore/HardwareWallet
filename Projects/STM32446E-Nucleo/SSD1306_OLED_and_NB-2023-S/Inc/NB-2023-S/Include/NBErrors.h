#ifndef NB_ERRORS_H
#define NB_ERRORS_H

#include <NBTypes.h>

#ifdef NB_CPP
extern "C"
{
#endif

typedef NBInt NBResult;

/*
    Operation completed successfully
*/
#define NB_OK                                       (0)
/*
    Operation failed
*/
#define NB_ERROR_FAILED                             (-100)
/*
    Argument is invalid
*/
#define NB_ERROR_ARGUMENT                           (-200)
/*
    Argument is NULL
*/
#define NB_ERROR_ARGUMENT_NULL                      (-201)
/*
    Argument is out of range
*/
#define NB_ERROR_ARGUMENT_OUT_OF_RANGE              (-202)
/*
    Invalid enumeration value
*/
#define NB_ERROR_INVALID_ENUM_ARGUMENT              (-203)
/*
    Allocated buffer is insufficient
*/
#define NB_ERROR_INSUFFICIENT_BUFFER                (-204)
/*
Index was out of range
*/
#define NB_ERROR_INDEX_OUT_OF_RANGE                 (-205)
/*
    Argument format is invalid
*/
#define NB_ERROR_FORMAT                             (-300)
/*
    Memory error occurred
*/
#define NB_ERROR_MEMORY                             (-400)
/*
    Out of memory
*/
#define NB_ERROR_OUT_OF_MEMORY                      (-401)
/*
    Memory corruption occurred
*/
#define NB_ERROR_MEMORY_CORRUPTION                  (-402)
/*
    Arithmetic error occurred
*/
#define NB_ERROR_ARITHMETIC                         (-500)
/*
    Division by zero
*/
#define NB_ERROR_ARITHMETIC_DIVIDE_BY_ZERO          (-501)
/*
    Matrix not square
*/
#define NB_ERROR_ARITHMETIC_MATRIX_NOT_SQUARE       (-502)
/*
    Overflow occurred
*/
#define NB_ERROR_OVERFLOW                           (-503)
/*
    Operation error occurred
*/
#define NB_ERROR_OPERATION                          (-600)
/*
    Operation is not supported
*/
#define NB_ERROR_NOT_SUPPORTED                      (-601)
/*
    Operation is not implemented
*/
#define NB_ERROR_NOT_IMPLEMENTED                    (-602)
/*
    Invalid operation
*/
#define NB_ERROR_INVALID_OPERATION                  (-603)
/*
    Operation canceled
*/
#define NB_ERROR_OPERATION_CANCELED                 (-604)
/*
    Operation timeout
*/
#define NB_ERROR_TIMEOUT                            (-605)
/*
    I/O error occurred
*/
#define NB_ERROR_IO                                 (-700)
/*
    Device is busy
*/
#define NB_ERROR_IO_DEVICE_BUSY                     (-701)
/*
    Device is not active
*/
#define NB_ERROR_IO_DEVICE_NOT_ACTIVE               (-702)
/*
    Device sensor failed
*/
#define NB_ERROR_IO_DEVICE_SENSOR_FAILED            (-703)
/*
    Device is not calibrated
*/
#define NB_ERROR_IO_DEVICE_NOT_CALIBRATED           (-704)
/*
    Device authentication failed
*/
#define NB_ERROR_IO_DEVICE_AUTHENTICATION_FAILED    (-705)
/*
    No devices available
*/
#define NB_ERROR_IO_NO_DEVICES                      (-706)
/*
    Command failed
*/
#define NB_ERROR_IO_COMMAND_FAILED                  (-707)
/*
    Response failed
*/
#define NB_ERROR_IO_COMMAND_RESPONSE_FAILED         (-708)
/*
    Communication failed
*/
#define NB_ERROR_IO_COMMUNICATION_FAILED            (-709)
/*
    Data field is invalid
*/
#define NB_ERROR_IO_DATA_FIELD_INVALID              (-710)
/*
    Data length is invalid
*/
#define NB_ERROR_IO_DATA_LENGTH_INVALID             (-711)
/*
    Parameter field is invalid
*/
#define NB_ERROR_IO_PARAMETER_FIELD_INVALID         (-712)
/*
    DCA error occurred
*/
#define NB_ERROR_IO_DCA                             (-713)
/*
    MCU error occurred
*/
#define NB_ERROR_IO_MCU                             (-714)
/*
    Operation conditions are invalid
*/
#define NB_ERROR_IO_OPERATION_CONDITIONS_INVALID    (-715)
/*
    Unknown command
*/
#define NB_ERROR_IO_UNKNOWN_COMMAND                 (-716)
/*
    FLASH error occurred
*/
#define NB_ERROR_IO_FLASH                           (-717)
/*
    Socket error occurred
*/
#define NB_ERROR_IO_SOCKET                          (-718)
/*
    External error occurred
*/
#define NB_ERROR_EXTERNAL                           (-800)
/*
    Win32 error occurred
*/
#define NB_ERROR_EXTERNAL_WIN32                     (-801)
/*
    Sys error occurred
*/
#define NB_ERROR_EXTERNAL_SYS                       (-802)
/*
    C++ error occurred
*/
#define NB_ERROR_EXTERNAL_CPP                       (-803)
/*
    .NET error occurred
*/
#define NB_ERROR_EXTERNAL_DOTNET                    (-804)
/*
    JVM error occurred
*/
#define NB_ERROR_EXTERNAL_JVM                       (-805)
/*
    COM error occurred
*/
#define NB_ERROR_EXTERNAL_COM                       (-806)

/*
    Allows to quickly check whether the invoked operation failed
*/
#define NBFailed(x) ((x) != NB_OK)

/*
    Allows to quickly check whether the invoked operation succeeded
*/
#define NBSucceeded(x) (!NBFailed(x))

/*
    Summary:
     Retrieve a string containing an error message corresponding
     to error code passed in parameter.
    Parameters:
     iCode :      [in] error code for which to get error
                       \description
     szValue :         [in/out] pre\-allocated buffer to store
                       error description
     uiValueLength :   [in] pre\-allocated buffer size
     puiValueLength :  [out] actual length of error description
                       (without trailing null\-terminator)
    Remarks:
     Before calling NBErrorsGetMessageA to get actual error
     message, actual size of error message needs to be found out.
     To do that, NBErrorsGetMessageA needs to be called with
     szValue set to NULL and uiValueLength set to 0. This way,
     puiValueLength will return actual length of the specified
     error description (without the trailing null-termination
     symbol). Afterwards, caller allocates puiValueLength + 1 size
     of buffer and calls NBErrorsGetMessageA with specifying
     pre-allocated buffer and its size, NBErrorsGetMessageA will
     fill error description into specified buffer.
     
     NB_ERROR_INSUFFICIENT_BUFFER error will be returned if the
     specified buffer is not sufficient to store function result.
    Returns:
     If the function succeeds, the return value is NB_OK. If the
     function fails, one of the error codes is returned (<link Error codes>).
*/
NBResult NB_API NBErrorsGetMessageA(NBResult iCode, NBAChar * szValue, NBUInt uiValueLength, NBUInt * puiValueLength);

void NB_API NBErrorsClear(NBResult iCode);
NBResult NB_API NBErrorsSetLastA(NBResult iCode, const NBAChar * szMessage, NBInt iExternalErrorCode, const NBAChar * szExternalCallStack, NBUInt uiFlags);

#ifdef NB_CPP
}
#endif

#endif // !NB_ERRORS_H
