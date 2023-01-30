/**
 * @file Queue.cxx
 * @brief This file contains the queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2022
 */

#include <memory>

#include <windows.h>

#include "UwbSimulatorDdi.h"
#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorIoQueue.hxx"
#include "UwbSimulatorTracelogging.hxx"

UwbSimulatorIoQueue::UwbSimulatorIoQueue(WDFQUEUE wdfQueue) :
    m_wdfQueue(wdfQueue)
{}

VOID
UwbSimulatorIoQueue::OnWdfIoDeviceControl(WDFQUEUE queue, WDFREQUEST request, size_t outputBufferLength, size_t inputBufferLength, ULONG ioControlCode)
{
    auto instance = GetUwbSimulatorIoQueue(queue);
    instance->OnIoDeviceControl(request, outputBufferLength, inputBufferLength, ioControlCode);
}

VOID
OnWdfIoStop(WDFQUEUE queue, WDFREQUEST request, ULONG actionFlags)
{
    auto instance = GetUwbSimulatorIoQueue(queue);
    instance->OnIoStop(request, actionFlags);
}

NTSTATUS
UwbSimulatorIoQueue::Initialize()
{
    auto wdfDevice = WdfIoQueueGetDevice(m_wdfQueue);

    NTSTATUS status = WdfDeviceCreateDeviceInterface(wdfDevice, &GUID_DEVINTERFACE_UWB_SIMULATOR, nullptr);
    if (!NT_SUCCESS(status)) {
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "Queue WdfDeviceCreateDeviceInterface failed",
            TraceLoggingLevel(TRACE_LEVEL_ERROR),
            TraceLoggingNTStatus(status, "Status"));
        return status;
    }

    return STATUS_SUCCESS;
}

NTSTATUS
UwbSimulatorIoQueue::Uninitialize()
{
    return STATUS_SUCCESS;
}

void
UwbSimulatorIoQueue::OnIoDeviceControl(WDFREQUEST request, size_t outputBufferLength, size_t inputBufferLength, ULONG ioControlCode)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "Queue Request",
        TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
        TraceLoggingPointer(m_wdfQueue, "Queue"),
        TraceLoggingPointer(request, "Request"),
        TraceLoggingHexInt32(ioControlCode, "IoControlCode"),
        TraceLoggingUInt64(outputBufferLength, "OutputBufferLength"),
        TraceLoggingUInt64(inputBufferLength, "InputBufferLength"));

    NTSTATUS status = STATUS_INVALID_DEVICE_STATE;

    // Get the file object this request came in on, and dispatch the request to it.
    UwbSimulatorDeviceFile *uwbSimulatorDeviceFile = GetUwbSimulatorFile(WdfRequestGetFileObject(request));
    if (uwbSimulatorDeviceFile != nullptr) {
        status = uwbSimulatorDeviceFile->OnRequest(request, ioControlCode, inputBufferLength, outputBufferLength);
    }

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "Queue Request Complete",
        TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
        TraceLoggingPointer(m_wdfQueue, "Queue"),
        TraceLoggingPointer(request, "Request"),
        TraceLoggingHexInt32(ioControlCode, "IoControlCode"),
        TraceLoggingNTStatus(status));
}

void
UwbSimulatorIoQueue::OnIoStop(WDFREQUEST request, ULONG actionFlags)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "I/O Stop",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingPointer(m_wdfQueue, "Queue"),
        TraceLoggingPointer(request, "Request"),
        TraceLoggingHexULong(actionFlags, "ActionFlags"));

    // In most cases, the EvtIoStop callback function completes, cancels, or postpones
    // further processing of the I/O request.
    //
    // Typically, the driver uses the following rules:
    //
    // - If the driver owns the I/O request, it calls WdfRequestUnmarkCancelable
    //   (if the request is cancelable) and either calls WdfRequestStopAcknowledge
    //   with a Requeue value of TRUE, or it calls WdfRequestComplete with a
    //   completion status value of STATUS_SUCCESS or STATUS_CANCELLED.
    //
    //   Before it can call these methods safely, the driver must make sure that
    //   its implementation of EvtIoStop has exclusive access to the request.
    //
    //   In order to do that, the driver must synchronize access to the request
    //   to prevent other threads from manipulating the request concurrently.
    //   The synchronization method you choose will depend on your driver's design.
    //
    //   For example, if the request is held in a shared context, the EvtIoStop callback
    //   might acquire an internal driver lock, take the request from the shared context,
    //   and then release the lock. At this point, the EvtIoStop callback owns the request
    //   and can safely complete or requeue the request.
    //
    // - If the driver has forwarded the I/O request to an I/O target, it either calls
    //   WdfRequestCancelSentRequest to attempt to cancel the request, or it postpones
    //   further processing of the request and calls WdfRequestStopAcknowledge with
    //   a Requeue value of FALSE.
    //
    // A driver might choose to take no action in EvtIoStop for requests that are
    // guaranteed to complete in a small amount of time.
    //
    // In this case, the framework waits until the specified request is complete
    // before moving the device (or system) to a lower power state or removing the device.
    // Potentially, this inaction can prevent a system from entering its hibernation state
    // or another low system power state. In extreme cases, it can cause the system
    // to crash with bugcheck code 9F.
}
