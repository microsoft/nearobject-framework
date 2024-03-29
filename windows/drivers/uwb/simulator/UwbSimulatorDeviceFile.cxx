
#include <algorithm>
#include <iterator>

#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorIoEventQueue.hxx"
#include "UwbSimulatorTracelogging.hxx"

using windows::devices::uwb::simulator::IUwbSimulatorDdiHandler;

UwbSimulatorDeviceFile::UwbSimulatorDeviceFile(WDFFILEOBJECT wdfFile, std::weak_ptr<UwbSimulatorDevice> uwbSimulatorDevice) :
    m_wdfFile(wdfFile),
    m_uwbSimulatorDevice(uwbSimulatorDevice)
{}

WDFFILEOBJECT
UwbSimulatorDeviceFile::GetWdfFile() const noexcept
{
    return m_wdfFile;
}

std::weak_ptr<UwbSimulatorDevice>
UwbSimulatorDeviceFile::GetDevice() noexcept
{
    return m_uwbSimulatorDevice;
}

std::shared_ptr<UwbSimulatorIoEventQueue>
UwbSimulatorDeviceFile::GetIoEventQueue()
{
    // Default file handles have no i/o event queue.
    return nullptr;
}

NTSTATUS
UwbSimulatorDeviceFile::Initialize()
{
    return STATUS_SUCCESS;
}

void
UwbSimulatorDeviceFile::RegisterHandler(std::shared_ptr<IUwbSimulatorDdiHandler> handler)
{
    m_ddiHandlers.push_back(std::move(handler));
}

/* static */
VOID
UwbSimulatorDeviceFile::OnWdfDestroy(WDFOBJECT wdfFile)
{
    auto instance = GetUwbSimulatorDeviceFileWdfContext(wdfFile)->File;
    if (instance->m_wdfFile != wdfFile) {
        return;
    }

    instance->OnDestroy();
}

/* static */
VOID
UwbSimulatorDeviceFile::OnWdfRequestCancel(WDFREQUEST request)
{
    auto wdfFile = WdfRequestGetFileObject(request);
    auto instance = GetUwbSimulatorDeviceFileWdfContext(wdfFile)->File;
    if (instance->m_wdfFile != wdfFile) {
        return;
    }

    instance->OnRequestCancel(request);
}

void
UwbSimulatorDeviceFile::OnDestroy()
{
    DbgPrint("%p destroyed\n", m_wdfFile);
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
