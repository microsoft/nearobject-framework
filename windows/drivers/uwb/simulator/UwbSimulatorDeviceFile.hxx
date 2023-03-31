
#ifndef UWB_SIMULATOR_DEVICE_FILE_OBJECT
#define UWB_SIMULATOR_DEVICE_FILE_OBJECT

#include <memory>
#include <vector>

#include <windows.h>

#include <wdf.h>

#include "IUwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorIoEventQueue.hxx"

class UwbSimulatorDevice;

/**
 * @brief Device driver open file abstraction.
 */
class UwbSimulatorDeviceFile
{
public:
    explicit UwbSimulatorDeviceFile(WDFFILEOBJECT wdfFile, UwbSimulatorDevice *uwbSimulatorDevice);

    /**
     * @brief Initializes the file object for use.
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Initialize();

    /**
     * @brief Register a DDI handler with this file instance.
     *
     * @param handler The handler to register.
     */
    void
    RegisterHandler(std::shared_ptr<windows::devices::uwb::simulator::IUwbSimulatorDdiHandler> handler);

    /**
     * @brief Device i/o control handler function.
     *
     * This is invoked when the driver receives an i/o control request on the
     * file handle associated with this instance.
     *
     * @param request The WDF driver request.
     * @param ioControlCode The i/o control code for the request.
     * @param inputBufferLength The input buffer length.
     * @param outputBufferLength The output buffer length.
     * @return NTSTATUS The status of the request operation.
     */
    NTSTATUS
    OnRequest(WDFREQUEST request, ULONG ioControlCode, size_t inputBufferLength, size_t outputBufferLength);

    /**
     * @brief Get the associated wdf file handle.
     *
     * @return WDFFILEOBJECT
     */
    WDFFILEOBJECT
    GetWdfFile() const noexcept;

    UwbSimulatorDevice *
    GetDevice() noexcept;

    std::shared_ptr<UwbSimulatorIoEventQueue>
    GetIoEventQueue();

public:
    static EVT_WDF_OBJECT_CONTEXT_DESTROY OnWdfDestroy;
    static EVT_WDF_REQUEST_CANCEL OnWdfRequestCancel;

private:
    /**
     * @brief
     */
    void
    OnDestroy();

    /**
     * @brief
     *
     * @param request
     */
    void
    OnRequestCancel(WDFREQUEST request);

private:
    WDFFILEOBJECT m_wdfFile;
    UwbSimulatorDevice *m_uwbSimulatorDevice{ nullptr };
    std::vector<std::shared_ptr<windows::devices::uwb::simulator::IUwbSimulatorDdiHandler>> m_ddiHandlers{};
    std::shared_ptr<UwbSimulatorIoEventQueue> m_ioEventQueue;

    static constexpr std::size_t MaximumQueueSizeDefault = 16;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorDeviceFile, GetUwbSimulatorFile);

#endif // UWB_SIMULATOR_DEVICE_FILE_OBJECT
