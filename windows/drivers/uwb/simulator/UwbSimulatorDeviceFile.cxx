
#include <algorithm>
#include <iterator>

#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorIoEventQueue.hxx"
#include "UwbSimulatorTracelogging.hxx"

using windows::devices::uwb::simulator::IUwbSimulatorDdiHandler;

UwbSimulatorDeviceFile::UwbSimulatorDeviceFile(WDFFILEOBJECT wdfFile, UwbSimulatorDevice *uwbSimulatorDevice) :
    m_wdfFile(wdfFile),
    m_uwbSimulatorDevice(uwbSimulatorDevice)
{}

WDFFILEOBJECT
UwbSimulatorDeviceFile::GetWdfFile() const noexcept
{
    return m_wdfFile;
}

UwbSimulatorIoEventQueue *
UwbSimulatorDeviceFile::GetIoEventQueue() noexcept
{
    return m_ioEventQueue;
}

UwbSimulatorDevice *
UwbSimulatorDeviceFile::GetDevice() noexcept
{
    return m_uwbSimulatorDevice;
}

NTSTATUS
UwbSimulatorDeviceFile::Initialize()
{
    // Create a manual dispatch queue for event handling.
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);
    queueConfig.PowerManaged = WdfFalse;

    // Set the parent to this file such that the queue's lifetime is tied to it.
    WDF_OBJECT_ATTRIBUTES queueAttributes;
    queueAttributes.ParentObject = m_wdfFile;
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&queueAttributes, UwbSimulatorIoEventQueue);

    WDFQUEUE wdfQueue;
    WDFDEVICE wdfDevice = WdfFileObjectGetDevice(m_wdfFile);
    NTSTATUS status = WdfIoQueueCreate(wdfDevice, &queueConfig, &queueAttributes, &wdfQueue);
    if (!NT_SUCCESS(status)) {
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "WdfIoQueueCreate failed",
            TraceLoggingLevel(TRACE_LEVEL_FATAL),
            TraceLoggingNTStatus(status));
        return status;
    }

    // Construct a new UwbSimulatorIoEventQueue instance using the WDF pre-allocated buffer.
    auto ioEventQueueBuffer = GetUwbSimulatorIoEventQueue(wdfQueue);
    auto ioEventQueue = new (ioEventQueueBuffer) UwbSimulatorIoEventQueue(wdfQueue);
    if (ioEventQueue == nullptr) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = ioEventQueue->Initialize();
    if (!NT_SUCCESS(status)) {
        ioEventQueue->~UwbSimulatorIoEventQueue();
        return status;
    }

    m_ioEventQueue = ioEventQueue;

    return status;
}

void
UwbSimulatorDeviceFile::RegisterHandler(std::unique_ptr<IUwbSimulatorDdiHandler> handler)
{
    m_ddiHandlers.push_back(std::move(handler));
}

/* static */
VOID
UwbSimulatorDeviceFile::OnWdfDestroy(WDFOBJECT wdfFile)
{
    auto instance = GetUwbSimulatorFile(wdfFile);
    if (instance->m_wdfFile != wdfFile) {
        return;
    }

    // Explicitly invoke the destructor since the object was created with placement new.
    instance->OnDestroy();
    instance->~UwbSimulatorDeviceFile();
}

/* static */
VOID
UwbSimulatorDeviceFile::OnWdfRequestCancel(WDFREQUEST request)
{
    auto wdfFile = WdfRequestGetFileObject(request);
    auto instance = GetUwbSimulatorFile(wdfFile);
    if (instance->m_wdfFile != wdfFile) {
        return;
    }

    instance->OnRequestCancel(request);
}

void
UwbSimulatorDeviceFile::OnDestroy()
{
    if (m_ioEventQueue != nullptr) {
        m_ioEventQueue->Uninitialize();
        m_ioEventQueue->~UwbSimulatorIoEventQueue();
        m_ioEventQueue = nullptr;
    }
}

void
UwbSimulatorDeviceFile::OnRequestCancel(WDFREQUEST /* request */)
{
}

NTSTATUS
UwbSimulatorDeviceFile::OnRequest(WDFREQUEST request, ULONG ioControlCode, size_t inputBufferLength, size_t outputBufferLength)
{
    NTSTATUS status;

    // Find a handler for this i/o control code.
    auto ddiHandlerIt = std::find_if(std::begin(m_ddiHandlers), std::end(m_ddiHandlers), [&](const auto &ddiHandler) {
        return (ddiHandler->HandlesIoControlCode(ioControlCode));
    });
    if (ddiHandlerIt == std::cend(m_ddiHandlers)) {
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "RequestNotImplemented",
            TraceLoggingLevel(TRACE_LEVEL_ERROR),
            TraceLoggingHexUInt32(ioControlCode, "IoControlCode"));
        status = STATUS_NOT_IMPLEMENTED;
        WdfRequestComplete(request, status);
        return status;
    }

    // Use the handler to validate the request.
    auto &ddiHandler = *ddiHandlerIt;
    status = ddiHandler->ValidateRequest(request, ioControlCode, inputBufferLength, outputBufferLength);
    switch (status) {
    case STATUS_SUCCESS:
        break;
    case STATUS_BUFFER_OVERFLOW:
        WdfRequestCompleteWithInformation(request, status, outputBufferLength);
        return status;
    default:
        WdfRequestComplete(request, status);
        return status;
    }

    // Obtain the input and output buffers.
    void *inputBufferPtr = nullptr;
    if (inputBufferLength != 0) {
        status = WdfRequestRetrieveInputBuffer(request, 0, &inputBufferPtr, nullptr);
        if (status != STATUS_SUCCESS) {
            WdfRequestComplete(request, status);
            return status;
        }
    }
    void *outputBufferPtr = nullptr;
    if (outputBufferLength != 0) {
        status = WdfRequestRetrieveOutputBuffer(request, 0, &outputBufferPtr, nullptr);
        if (status != STATUS_SUCCESS) {
            WdfRequestComplete(request, status);
            return status;
        }
    }

    // Dispatch the request to the DDI handler.
    std::span<uint8_t> inputBuffer{ reinterpret_cast<uint8_t *>(inputBufferPtr), inputBufferLength };
    std::span<uint8_t> outputBuffer{ reinterpret_cast<uint8_t *>(outputBufferPtr), outputBufferLength };
    status = ddiHandler->HandleRequest(request, ioControlCode, inputBuffer, outputBuffer);

    return status;
}
