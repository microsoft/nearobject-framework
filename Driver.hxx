/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    User-mode Driver Framework 2

--*/

#include <windows.h>
#include <wdf.h>
#include <initguid.h>

#include "device.hxx"
#include "queue.hxx"
#include "trace.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD UwbSimulatorEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP UwbSimulatorEvtDriverContextCleanup;

EXTERN_C_END
