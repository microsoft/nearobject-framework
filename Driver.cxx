/**
 * @file Driver.cxx
 * @brief This file contains the driver entry points and callbacks.
 *
 * @copyright Copyright (c) 2022
 */

#include "driver.hxx"

#include "UwbSimulatorTracelogging.hxx"

/**
 * @brief DriverEntry initializes the driver and is the first routine called by
 * the system after the driver is loaded. DriverEntry specifies the other entry
 * points in the function driver, such as EvtDevice and DriverUnload.
 *
 * @param driverObject Represents the instance of the function driver that is
 * loaded into memory. DriverEntry must initialize members of DriverObject
 * before it returns to the caller. DriverObject is allocated by the system
 * before the driver is loaded, and it is released by the system after the
 * system unloads the function driver from memory.
 * @param registryPath Represents the driver specific path in the Registry. The
 * function driver can use the path to store driver related data between
 * reboots. The path does not store hardware instance specific data.
 * @return NTSTATUS STATUS_SUCCESS if successful, STATUS_UNSUCCESSFUL otherwise.
 */
NTSTATUS
DriverEntry(PDRIVER_OBJECT driverObject, PUNICODE_STRING registryPath)
{
    TraceLoggingRegister(UwbSimulatorTraceloggingProvider);
    TraceLoggingWrite(UwbSimulatorTraceloggingProvider, "DriverEntry");

    // Register a cleanup callback so that we can call TraceLoggingUnregister
    // when the framework driver object is deleted during driver unload.
    WDF_OBJECT_ATTRIBUTES attributes;
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = UwbSimulatorEvtDriverContextCleanup;

    WDF_DRIVER_CONFIG config;
    WDF_DRIVER_CONFIG_INIT(&config, UwbSimulatorEvtDeviceAdd);
    NTSTATUS status = WdfDriverCreate(driverObject, registryPath, &attributes, &config, WDF_NO_HANDLE);
    if (!NT_SUCCESS(status)) {
        TraceLoggingWrite(UwbSimulatorTraceloggingProvider, "WdfDriverCreate failed", TraceLoggingLevel(TRACE_LEVEL_ERROR), TraceLoggingNTStatus(status));
        TraceLoggingUnregister(UwbSimulatorTraceloggingProvider);
        return status;
    }

    return status;
}

/**
 * @brief EvtDeviceAdd is called by the framework in response to AddDevice call
 * from the PnP manager. We create and initialize a device object to represent a
 * new instance of the device.
 *
 * @param driver Handle to a framework driver object created in DriverEntry.
 * @param deviceInit Pointer to a framework-allocated WDFDEVICE_INIT structure.
 * @return NTSTATUS
 */
NTSTATUS
UwbSimulatorEvtDeviceAdd(WDFDRIVER /* driver */, WDFDEVICE_INIT *deviceInit)
{
    TraceLoggingWrite(UwbSimulatorTraceloggingProvider, "AddDevice", TraceLoggingLevel(TRACE_LEVEL_INFORMATION));

    NTSTATUS status = UwbSimulatorCreateDevice(deviceInit);
    return status;
}

/**
 * @brief Free all the resources allocated in DriverEntry.
 *
 * @param driverObject  Handle to a WDF Driver object.
 */
void
UwbSimulatorEvtDriverContextCleanup(WDFOBJECT /* driverObject */)
{
    TraceLoggingUnregister(UwbSimulatorTraceloggingProvider);
}
