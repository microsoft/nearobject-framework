/**
 * @file Driver.cxx
 * @brief This file contains the driver entry points and callbacks.
 *
 * @copyright Copyright (c) 2022
 */

#include "driver.hxx"
#include "driver.tmh"

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
DriverEntry(PDRIVER_OBJECT driverObject, UNICODE_STRING *registryPath)
{
    //
    // Initialize WPP Tracing
    //
#if UMDF_VERSION_MAJOR == 2 && UMDF_VERSION_MINOR == 0
    WPP_INIT_TRACING(UWB_SIMULATOR_TRACING_ID);
#else
    WPP_INIT_TRACING(driverObject, registryPath);
#endif

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");

    //
    // Register a cleanup callback so that we can call WPP_CLEANUP when
    // the framework driver object is deleted during driver unload.
    //
    WDF_OBJECT_ATTRIBUTES attributes;
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.EvtCleanupCallback = UwbSimulatorEvtDriverContextCleanup;

    WDF_DRIVER_CONFIG config;
    WDF_DRIVER_CONFIG_INIT(&config, UwbSimulatorEvtDeviceAdd);
    NTSTATUS status = WdfDriverCreate(driverObject, registryPath, &attributes, &config, WDF_NO_HANDLE);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_DRIVER, "WdfDriverCreate failed %!STATUS!", status);
#if UMDF_VERSION_MAJOR == 2 && UMDF_VERSION_MINOR == 0
        WPP_CLEANUP();
#else
        WPP_CLEANUP(driverObject);
#endif
        return status;
    }

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");

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
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");

    NTSTATUS status = UwbSimulatorCreateDevice(deviceInit);

    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Exit");

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
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Entry");

    //
    // Stop WPP Tracing
    //
#if UMDF_VERSION_MAJOR == 2 && UMDF_VERSION_MINOR == 0
    WPP_CLEANUP();
#else
    WPP_CLEANUP(WdfDriverWdmGetDriverObject((WDFDRIVER)driverObject));
#endif
}
