
#include "UwbSimulatorDeviceFileNotification.hxx"
#include "UwbSimulatorTracelogging.hxx"

std::shared_ptr<UwbSimulatorIoEventQueue>
UwbSimulatorDeviceFileNotification::GetIoEventQueue()
{
    return m_ioEventQueue;
}

NTSTATUS
UwbSimulatorDeviceFileNotification::Initialize()
{
    std::shared_ptr<UwbSimulatorIoEventQueue> ioEventQueue;

    // Create a manual dispatch queue for event handling.
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
    queueConfig.PowerManaged = WdfFalse;

    WDFQUEUE wdfQueue;
    WDFDEVICE wdfDevice = WdfFileObjectGetDevice(GetWdfFile());
    NTSTATUS status = WdfIoQueueCreate(wdfDevice, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &wdfQueue);
    if (!NT_SUCCESS(status)) {
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "WdfIoQueueCreate failed",
            TraceLoggingLevel(TRACE_LEVEL_FATAL),
            TraceLoggingNTStatus(status));
        return status;
    }

    ioEventQueue = std::make_shared<UwbSimulatorIoEventQueue>(wdfQueue, MaximumQueueSizeDefault);
    if (ioEventQueue == nullptr) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = ioEventQueue->Initialize();
    if (!NT_SUCCESS(status)) {
        return status;
    }

    m_ioEventQueue = std::move(ioEventQueue);

    return STATUS_SUCCESS;
}
