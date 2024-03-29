
#ifndef UWB_SIMULATOR_DEVICE_HXX
#define UWB_SIMULATOR_DEVICE_HXX

#include <windows.h>

#include <wdf.h>
#include <wdfrequest.h>

#include <chrono>
#include <memory>
#include <shared_mutex>
#include <tuple>
#include <unordered_map>

#include "IUwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorIoEventQueue.hxx"
#include "UwbSimulatorIoQueue.hxx"
#include "UwbSimulatorSession.hxx"

#include <uwb/protocols/fira/FiraDevice.hxx>

class UwbSimulatorDeviceFile;

/**
 * @brief
 */
class UwbSimulatorDevice :
    public std::enable_shared_from_this<UwbSimulatorDevice>
{
protected:
    /**
     * @brief Construct a new UwbSimulatorDevice object.
     *
     * @param wdfDevice
     */
    explicit UwbSimulatorDevice(WDFDEVICE wdfDevice);

public:
    /**
     * @brief
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Initialize();

    /**
     * @brief
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Uninitialize();

    /**
     * @brief Create a new uwb session.
     *
     * @param sessionId
     * @param sessionType
     * @return std::tuple<UwbStatus, std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>>
     */
    std::tuple<::uwb::protocol::fira::UwbStatus, std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>>
    SessionCreate(uint32_t sessionId, UwbSessionType sessionType);

    /**
     * @brief Destroy an existing session.
     *
     * @param sessionId
     * @return std::tuple<UwbStatus, std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>>
     */
    std::tuple<::uwb::protocol::fira::UwbStatus, std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>>
    SessionDestroy(uint32_t sessionId);

    /**
     * @brief Get a shared reference to an existing session.
     *
     * @param sessionId
     * @return std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>
     */
    std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>
    SessionGet(uint32_t sessionId);

    /**
     * @brief Get the number of sessions.
     *
     * @return std::size_t
     */
    std::size_t
    GetSessionCount();

    /**
     * @brief Perform device initialization.
     *
     * This simulates a real device by delaying for the specified amount of
     * time.
     *
     * @param initializeTime The amount of time it should take the device to
     * initialize. If not specified, a default value is used.
     */
    void
    DeviceInitialize(std::chrono::duration<double> initializeTime = SimulatedInitializationTimeDefault);

    /**
     * @brief Perform device uninitialization.
     */
    void
    DeviceUninitialize();

    /**
     * @brief Reset the device, cycling it from its current state to the
     * deinitialized state,  then re-initializing it.
     */
    void
    DeviceReset();

    /**
     * @brief Update the device state.
     *
     * This will generate a UwbNotification if the state changes.
     *
     * @param deviceState The new device state.
     */
    void
    DeviceUpdateState(::uwb::protocol::fira::UwbDeviceState deviceState);

    /**
     * @brief Push a simulated uwb notification.
     *
     * This distributes the event to all open-file handle i/o event queues.
     *
     * @param notificationData The notification data to distribute.
     */
    void
    PushUwbNotification(::uwb::protocol::fira::UwbNotificationData notificationData);

public:
    static EVT_WDF_DRIVER_DEVICE_ADD OnWdfDeviceAdd;
    static EVT_WDF_OBJECT_CONTEXT_CLEANUP OnWdfObjectContextCleanup;
    static EVT_WDF_DEVICE_D0_ENTRY OnWdfD0Entry;
    static EVT_WDF_DEVICE_D0_EXIT OnWdfD0Exit;
    static EVT_WDF_DEVICE_FILE_CREATE OnWdfFileCreate;
    static EVT_WDF_FILE_CLOSE OnWdfFileClose;

private:
    /**
     * @brief
     *
     * @param device
     * @param request
     * @param file
     */
    void
    OnFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file);

    /**
     * @brief
     *
     * @param file
     */
    void
    OnFileClose(WDFFILEOBJECT file);

    /**
     * @brief
     *
     * @param previousState
     * @return NTSTATUS
     */
    NTSTATUS
    OnD0Entry(WDF_POWER_DEVICE_STATE previousState);

    /**
     * @brief
     *
     * @param targetState
     * @return NTSTATUS
     */
    NTSTATUS
    OnD0Exit(WDF_POWER_DEVICE_STATE targetState);

private:
    static constexpr auto SimulatedInitializationTimeDefault = std::chrono::microseconds(10);

    WDFDEVICE m_wdfDevice;
    UwbSimulatorIoQueue *m_ioQueue{ nullptr };
    std::shared_ptr<windows::devices::uwb::simulator::IUwbSimulatorDdiHandler> m_ddiHandler;

    // Session state and associated lock that protects it.
    std::shared_mutex m_sessionsGate;
    std::unordered_map<uint32_t, std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>> m_sessions{};

    // Open file handles
    std::shared_mutex m_deviceFilesGate;
    std::vector<std::weak_ptr<UwbSimulatorDeviceFile>> m_deviceFiles;

    // Device state
    std::shared_mutex m_deviceStateGate;
    ::uwb::protocol::fira::UwbDeviceState m_deviceState{ ::uwb::protocol::fira::UwbDeviceState::Uninitialized };
};

struct UwbSimulatorDeviceWdfContext
{
    std::shared_ptr<UwbSimulatorDevice> Device;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorDeviceWdfContext, GetUwbSimulatorDeviceWdfContext)

#endif // UWB_SIMULATOR_DEVICE_HXX
