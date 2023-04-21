
#ifndef UWB_SESSION_HXX
#define UWB_SESSION_HXX

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <type_traits>
#include <unordered_set>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbPeer.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace uwb
{
class UwbDevice;

/**
 * @brief Represents a UWB session.
 */
class UwbSession
{
public:
    static constexpr uwb::protocol::fira::DeviceType DeviceTypeDefault = uwb::protocol::fira::DeviceType::Controller;

    /**
     * @brief Construct a new UwbSession object without callbacks.
     *
     * @param sessionId The session identifier.
     * @param device The parent device of this session.
     * @param deviceType The host device type in the session (controller, controlee).
     */
    UwbSession(uint32_t sessionId, std::weak_ptr<UwbDevice> device, uwb::protocol::fira::DeviceType deviceType = DeviceTypeDefault);

    /**
     * @brief Construct a new UwbSession object.
     *
     *
     * @param sessionId The session identifier.
     * @param device The parent device of this session.
     * @param callbacks The event callbacks to use.
     * @param deviceType The host device type in the session (controller, controlee).
     */
    UwbSession(uint32_t sessionId, std::weak_ptr<UwbDevice> device, std::weak_ptr<UwbSessionEventCallbacks> callbacks, uwb::protocol::fira::DeviceType deviceType = DeviceTypeDefault);

    /**
     * @brief Destroy the UwbSession object.
     */
    virtual ~UwbSession() = default;

    /**
     * @brief Get a weak reference to the event callbacks instance.
     *
     * @return std::weak_ptr<UwbSessionEventCallbacks>
     */
    std::weak_ptr<UwbSessionEventCallbacks>
    GetEventCallbacks() noexcept;

    /**
     * @brief Set the callbacks to be used for events.
     *
     * @param callbacks The event callbacks instance.
     */
    void
    SetEventCallbacks(std::weak_ptr<UwbSessionEventCallbacks> callbacks) noexcept;

    /**
     * @brief Get the Device Type associated with the host of this UwbSession
     *
     * @return uwb::protocol::fira::DeviceType
     */
    uwb::protocol::fira::DeviceType
    GetDeviceType() const noexcept;

    /**
     * @brief Get the unique session id.
     *
     * @return uint32_t
     */
    uint32_t
    GetId() const noexcept;

    /**
     * @brief Configure the session for use.
     *
     * This function tells the UWBS to initialize the session for ranging with
     * the particular sessionId and then configures it with configParams.
     *
     * @param configParams
     */
    void
    Configure(const std::vector<protocol::fira::UwbApplicationConfigurationParameter> configParams);

    /**
     * @brief Set the type of mac address to be used for session participants.
     *
     * @param uwbMacAddressType The type of mac address to use.
     */
    void
    SetMacAddressType(UwbMacAddressType uwbMacAddressType) noexcept;

    /**
     * @brief Attempt to add a controlee to this session.
     *
     * @param controleeMacAddress The mac address of the controlee. This is
     * expected to be in the mac address format configured for the session.
     * @return UwbStatus The status of the operation. UwbStatusGeneric::Ok is
     * returned if the controlee was successfully added.
     */
    uwb::protocol::fira::UwbStatus
    TryAddControlee(uwb::protocol::fira::UwbMacAddress controleeMacAddress);

    /**
     * @brief Start ranging.
     */
    void
    StartRanging();

    /**
     * @brief Stop ranging.
     */
    void
    StopRanging();

    /**
     * @brief Set the Session Status object. NOTE, this function is NOT thread safe
     *
     * @param status the new status
     */
    void
    SetSessionStatus(const uwb::protocol::fira::UwbSessionStatus& status);

    /**
     * @brief Get the Application Configuration Parameters object
     *
     * @param requestedTypes leave this as an empty vector to request all parameters
     * @return std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
     */
    std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
    GetApplicationConfigurationParameters(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> requestedTypes);

    /**
     * @brief Set the Application Configuration Parameters object
     *
     * @param uwbApplicationConfigurationParameters
     */
    void
    SetApplicationConfigurationParameters(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters);

    /**
     * @brief Get the current state for this session.
     *
     * @return ::uwb::protocol::fira::UwbSessionState
     */
    ::uwb::protocol::fira::UwbSessionState
    GetSessionState();

    /**
     * @brief Destroy the session, making it unusable.
     */
    void
    Destroy();

protected:
    /**
     * @brief Attempt to resolve the event callbacks from a weak to a shared
     * reference.
     *
     * @return std::shared_ptr<UwbSessionEventCallbacks>
     */
    std::shared_ptr<UwbSessionEventCallbacks>
    ResolveEventCallbacks() noexcept;

    /**
     * @brief Attempt to resolve the parent device object instance.
     *
     * This is protected to prevent derived classes from obtaining the weak_ptr,
     * which would never be useful to them.
     *
     * This allows derived types to obtain a properly typed smart pointer by
     * specifying the type as the template argument. If not, it defaults to the
     * base type.
     *
     * @tparam TDerived The derived type of the parent device.
     * @return std::shared_ptr<UwbDevice>
     */
    template <typename TDerived = UwbDevice>
    std::shared_ptr<UwbDevice>
    ResolveParentDevice() noexcept
    {
        if constexpr (std::is_same_v<TDerived, UwbDevice>) {
            return m_device.lock();
        } else {
            return std::dynamic_pointer_cast<TDerived>(m_device.lock());
        }
    }

private:
    /**
     * @brief Internal function to insert a peer address to this session
     *
     * @param peerAddress
     */
    void
    InsertPeerImpl(const uwb::UwbMacAddress& peerAddress);

    /**
     * @brief Configures the session for use.
     *
     * @param uwbSessionData The session data to configure the session with.
     */
    virtual void
    ConfigureImpl(const std::vector<protocol::fira::UwbApplicationConfigurationParameter> configParams) = 0;

    /**
     * @brief Start ranging.
     */
    virtual void
    StartRangingImpl() = 0;

    /**
     * @brief Stop ranging.
     */
    virtual void
    StopRangingImpl() = 0;

    /**
     * @brief Attempt to add a controlee to this session.
     *
     * @param controleeMacAddress The mac address of the controlee. This is
     * expected to be in the mac address format configured for the session.
     * @return UwbStatus The status of the operation. UwbStatusGeneric::Ok is
     * returned if the controlee was successfully added.
     */
    virtual uwb::protocol::fira::UwbStatus
    TryAddControleeImpl(UwbMacAddress controleeMacAddress) = 0;

    /**
     * @brief Get the Application Configuration Parameters object
     *
     * @param requestedTypes leave this as an empty vector to request all parameters
     * @return std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
     */
    virtual std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
    GetApplicationConfigurationParametersImpl(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> requestedTypes) = 0;

    /**
     * @brief Set the Application Configuration Parameters object
     *
     * @param uwbApplicationConfigurationParameters
     */
    virtual void
    SetApplicationConfigurationParametersImpl(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters) = 0;

    /**
     * @brief Get the current state for this session.
     *
     * @return ::uwb::protocol::fira::UwbSessionState
     */
    virtual ::uwb::protocol::fira::UwbSessionState
    GetSessionStateImpl() = 0;

    /**
     * @brief Destroy the session, making it unusable.
     */
    virtual void
    DestroyImpl() = 0;

protected:
    uwb::protocol::fira::DeviceType m_deviceType{ uwb::protocol::fira::DeviceType::Controller };
    uint32_t m_sessionId{ 0 };
    uwb::protocol::fira::UwbSessionStatus m_sessionStatus{};
    UwbMacAddressType m_uwbMacAddressType{ UwbMacAddressType::Extended };
    UwbMacAddress m_uwbMacAddressSelf;
    std::atomic<bool> m_rangingActive{ false };
    std::mutex m_peerGate;
    std::unordered_set<UwbMacAddress> m_peers{};
    std::shared_mutex m_callbacksGate;
    std::weak_ptr<UwbSessionEventCallbacks> m_callbacks;
    std::weak_ptr<UwbDevice> m_device;
};

} // namespace uwb

#endif // UWB_SESSION_HXX
