
#ifndef UWB_SIMULATOR_DEVICE_FILE_OBJECT
#define UWB_SIMULATOR_DEVICE_FILE_OBJECT

#include <windows.h>
#include <wdf.h>

class UwbSimulatorDeviceFileObject
{
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorDeviceFileObject, GetUwbSimulatorFileObject);

#endif // UWB_SIMULATOR_DEVICE_FILE_OBJECT

