
#ifndef UWB_SIMULATOR_DEVICE_HXX
#define UWB_SIMULATOR_DEVICE_HXX

#include <windows.h>
#include <wdf.h>
#include <wdfrequest.h>

class UwbSimulatorDevice
{
public:
	static void OnFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file);
	static void OnFileClose(WDFFILEOBJECT file);
};

#endif // UWB_SIMULATOR_DEVICE_HXX
