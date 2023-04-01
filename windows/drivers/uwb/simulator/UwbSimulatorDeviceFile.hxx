
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
class UwbSimulatorDeviceFile :
    public std::enable_shared_from_this<UwbSimulatorDeviceFile>
{
protected:
    /**
     * @brief Construct a new UwbSimulatorDeviceFile object.
     *
     * @param wdfFile The WDF file object this context is associated with.
     * @param uwbSimulatorDevice The parent device of the open file handle.
     */
    explicit UwbSimulatorDeviceFile(WDFFILEOBJECT wdfFile, std::weak_ptr<UwbSimulatorDevice> uwbSimulatorDevice);

public:
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

    /**
     * @brief Get a pointer to the owning device instance.
     *
     * @return UwbSimulatorDevice*
     */
    std::weak_ptr<UwbSimulatorDevice>
    GetDevice() noexcept;

    /**
     * @brief Get a reference to the i/o event queue for this open file handle.
     *
     * @return std::shared_ptr<UwbSimulatorIoEventQueue>
     */
    std::shared_ptr<UwbSimulatorIoEventQueue>
    GetIoEventQueue();

public:
    static EVT_WDF_OBJECT_CONTEXT_DESTROY OnWdfDestroy;
    static EVT_WDF_REQUEST_CANCEL OnWdfRequestCancel;

private:
    /**
     * @brief Destroy member function which is called in response to the
     * EvtDestroyCallback WDF event (OnWdfDestroy) for the corresponding file
     * object.
     */
    void
    OnDestroy();

    /**
     * @brief Request cancelation member function which is called in response to
     * the EvtRequestCancel WDF event (OnWdfRequestCancel) for the corresponding
     * file object.
     *
     * @param request The request that is being canceled.
     */
    void
    OnRequestCancel(WDFREQUEST request);

private:
    WDFFILEOBJECT m_wdfFile;
    std::weak_ptr<UwbSimulatorDevice> m_uwbSimulatorDevice;
    std::vector<std::shared_ptr<windows::devices::uwb::simulator::IUwbSimulatorDdiHandler>> m_ddiHandlers{};
    std::shared_ptr<UwbSimulatorIoEventQueue> m_ioEventQueue;

    /**
     * @brief Default size for the data queue. This should be large enough to
     * contain the expected number of entries that could be generated in the
     * time it takes a client to process a single notification. The current
     * value (16) was selected to hopefully satisfy this, however, will be tuned
     * later once real-world empirical data is collected and analzyed.
     */
    static constexpr std::size_t MaximumQueueSizeDefault = 16;
};

struct UwbSimulatorDeviceFileWdfContext
{
    std::shared_ptr<UwbSimulatorDeviceFile> File;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorDeviceFileWdfContext, GetUwbSimulatorDeviceFileWdfContext);

#endif // UWB_SIMULATOR_DEVICE_FILE_OBJECT
