/**
 * @file Driver.hxx
 * @brief This file contains the driver definitions.
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <windows.h>

#include <initguid.h>
#include <wdf.h>

#include "device.hxx"
#include "queue.hxx"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD UwbSimulatorEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP UwbSimulatorEvtDriverContextCleanup;

EXTERN_C_END
