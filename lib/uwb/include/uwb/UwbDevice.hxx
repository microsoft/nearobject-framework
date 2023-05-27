
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
protected:
    /**
     * @brief Only allow derived classes to instantiate an instance.
     */
    UwbDevice() = default;

public:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     *
     * @param callbacks
     * @param deviceType
     * @return std::shared_ptr<UwbSession>
     */
    std::shared_ptr<UwbSession>
    CreateSession(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks, uwb::protocol::fira::DeviceType deviceType);

    /**
     * @brief Obtains a shared reference to a pre-existing session.
     *
     * @param sessionId The session identifier.
     * @return std::shared_ptr<UwbSession>
     */
    std::shared_ptr<UwbSession>
    GetSession(uint32_t sessionId);

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
     * @brief Get the number of sessions associated with the device.
     *
     * @return uint32_t
     */
    uint32_t
    GetSessionCount();

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
     * @param deviceType
     * @return std::shared_ptr<UwbSession>
     */
    virtual std::shared_ptr<UwbSession>
    CreateSessionImpl(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks, uwb::protocol::fira::DeviceType deviceType = uwb::protocol::fira::DeviceType::Controller) = 0;

    /**
     * @brief Attempt to resolve a session from the underlying UWB device.
     *
     * This is used when a session may be pre-existing within the driver but
     * this instance is not yet aware of.
     *
     * @param sessionId The identifier of the session to resolve.
     * @return std::shared_ptr<UwbSession>
     */
    virtual std::shared_ptr<UwbSession>
    ResolveSessionImpl(uint32_t sessionId) = 0;

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
     * @brief Get the number of sessions associated with the device.
     *
     * @return uint32_t
     */
    virtual uint32_t
    GetSessionCountImpl() = 0;

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
     * @brief Find the specified session in the session cache.
     *
     * @param sessionId The identifier of the session to find.
     * @return std::shared_ptr<UwbSession>
     */
    std::shared_ptr<UwbSession>
    FindSession(uint32_t sessionId);

    /**
     * @brief Internal function which finds the specified session but does not
     * enforce any mutual exclusion. The caller must hold the m_sessionsGate
     * mutex either in shared or exclusive mode in order to safely call this
     * function.
     *
     * @param sessionId The identifier of the session to find.
     * @return std::shared_ptr<UwbSession>
     */
    std::shared_ptr<UwbSession>
    FindSessionLocked(uint32_t sessionId);

    /**
     * @brief Invoked when a generic error occurs. TODO this callback needs to be invoked by a UwbConnector for the linux portion too
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
