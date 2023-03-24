
#ifndef UWB_DEVICE_HXX
#define UWB_DEVICE_HXX

#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include <uwb/UwbDeviceEventCallbacks.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

namespace uwb
{
class UwbSession;

/**
 * @brief Interface for interacting with a UWB device.
 */
class UwbDevice
{
public:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     *
     * @param callbacks
     * @return std::shared_ptr<UwbSession>
     */
    std::shared_ptr<UwbSession>
    CreateSession(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks);

    /**
     * @brief Get the FiRa capabilities of the device.
     *
     * @return uwb::protocol::fira::UwbCapability
     */
    uwb::protocol::fira::UwbCapability
    GetCapabilities();

    /**
     * @brief Get the FiRa device information of the device.
     *
     * @return ::uwb::protocol::fira::UwbDeviceInformation
     */
    ::uwb::protocol::fira::UwbDeviceInformation
    GetDeviceInformation();

    /**
     * @brief Reset the device to an initial clean state.
     */
    void
    Reset();

    /**
     * @brief Initializes a new UWB device
     *
     * @return bool
     */
    bool
    Initialize();

    /**
     * @brief Determine if this device is the same as another.
     *
     * @param other
     * @return true
     * @return false
     */
    virtual bool
    IsEqual(const UwbDevice& other) const noexcept = 0;

    /**
     * @brief Destroy the UwbDevice object.
     */
    virtual ~UwbDevice() = default;

private:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     *
     * @param sessionId
     * @param callbacks
     * @return std::shared_ptr<UwbSession>
     */
    virtual std::shared_ptr<UwbSession>
    CreateSessionImpl(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks) = 0;

    /**
     * @brief Get the FiRa capabilities of the device.
     *
     * @return uwb::protocol::fira::UwbCapability
     */
    virtual uwb::protocol::fira::UwbCapability
    GetCapabilitiesImpl() = 0;

    /**
     * @brief Get the FiRa device information of the device.
     *
     * @return ::uwb::protocol::fira::UwbDeviceInformation
     */
    virtual ::uwb::protocol::fira::UwbDeviceInformation
    GetDeviceInformationImpl() = 0;

    /**
     * @brief Reset the device to an initial clean state.
     */
    virtual void
    ResetImpl() = 0;

    /**
     * @brief Initializes a new UWB device.
     *
     * @return bool
     */
    virtual bool
    InitializeImpl();

protected:
    /**
     * @brief Get a reference to the specified session.
     *
     * @param sessionId
     * @return std::shared_ptr<UwbSession>
     */
    std::shared_ptr<UwbSession>
    GetSession(uint32_t sessionId);

    /**
     * @brief Invoked when a generic error occurs. TODO this callback needs to be invoked by a UwbDeviceConnector for the linux portion too
     *
     * @param status The generic error that occurred.
     */
    void
    OnStatusChanged(::uwb::protocol::fira::UwbStatus status);

    /**
     * @brief Invoked when the device status changes.
     *
     * @param statusDevice
     */
    void
    OnDeviceStatusChanged(::uwb::protocol::fira::UwbStatusDevice statusDevice);

    /**
     * @brief Invoked when the status of a session changes.
     *
     * @param statusSession The new status of the session.
     */
    void
    OnSessionStatusChanged(::uwb::protocol::fira::UwbSessionStatus statusSession);

private:
    ::uwb::protocol::fira::UwbStatusDevice m_status{ .State = ::uwb::protocol::fira::UwbDeviceState::Uninitialized };
    ::uwb::protocol::fira::UwbStatus m_lastError{ ::uwb::protocol::fira::UwbStatusGeneric::Ok };
    std::shared_mutex m_sessionsGate;
    std::unordered_map<uint32_t, std::weak_ptr<uwb::UwbSession>> m_sessions{};
};

bool
operator==(const UwbDevice&, const UwbDevice&) noexcept;

bool
operator!=(const UwbDevice&, const UwbDevice&) noexcept;

} // namespace uwb

#endif // UWB_DEVICE_HXX
