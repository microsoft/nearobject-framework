
#ifndef UWB_SIMULATOR_DEVICE_FILE_NOTIFICATION_OBJECT
#define UWB_SIMULATOR_DEVICE_FILE_NOTIFICATION_OBJECT

#include <memory>

#include <windows.h>

#include <wdf.h>

#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorIoEventQueue.hxx"

/**
 * @brief Device driver open file abstraction for notification handling.
 */
class UwbSimulatorDeviceFileNotification :
    public UwbSimulatorDeviceFile
{
protected:
    /**
     * @brief Construct a new UwbSimulatorDeviceFile object.
     *
     * @param wdfFile The WDF file object this context is associated with.
     * @param uwbSimulatorDevice The parent device of the open file handle.
     */
    using UwbSimulatorDeviceFile::UwbSimulatorDeviceFile;

public:
    /**
     * @brief Initializes the file object for use.
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Initialize() override;

    /**
     * @brief Get a reference to the i/o event queue for this open file handle.
     *
     * @return std::shared_ptr<UwbSimulatorIoEventQueue>
     */
    std::shared_ptr<UwbSimulatorIoEventQueue>
    GetIoEventQueue() override;

private:
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

#endif // UWB_SIMULATOR_DEVICE_FILE_NOTIFICATION_OBJECT
