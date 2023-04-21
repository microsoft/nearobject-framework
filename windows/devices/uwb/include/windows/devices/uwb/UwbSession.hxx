
#ifndef WINDOWS_UWB_SESSION_HXX
#define WINDOWS_UWB_SESSION_HXX

#include <cstdint>
#include <memory>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbRegisteredCallbacks.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <windows/devices/uwb/IUwbSessionDdi.hxx>

namespace windows::devices::uwb
{
class UwbDevice;

/**
 * @brief Windows concrete implementation of a UWB session.
 */
class UwbSession :
    public ::uwb::UwbSession
{
public:
    /**
     * @brief Construct a new UwbSession object without callbacks.
     *
     * @param sessionId The session identifier.
     * @param device Reference to the parent device.
     * @param uwbSessionConnector The connector used to communicate with the device.
     * @param deviceType The device type of the host in this session.
     */
    UwbSession(uint32_t sessionId, std::weak_ptr<::uwb::UwbDevice> device, std::shared_ptr<IUwbSessionDdiConnector> uwbSessionConnector, ::uwb::protocol::fira::DeviceType deviceType = ::uwb::protocol::fira::DeviceType::Controller);

    /**
     * @brief Construct a new UwbSession object.
     * This also registers the callbacks with the UwbConnector
     *
     * @param sessionId The session identifier.
     * @param device Reference to the parent device.
     * @param uwbSessionConnector The connector used to communicate with the device.
     * @param callbacks The event callback instance.
     * @param deviceType The device type of the host in this session.
     */
    UwbSession(uint32_t sessionId, std::weak_ptr<::uwb::UwbDevice> device, std::shared_ptr<IUwbSessionDdiConnector> uwbSessionConnector, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, ::uwb::protocol::fira::DeviceType deviceType = ::uwb::protocol::fira::DeviceType::Controller);

private:
    /**
     * @brief Configure the session for use.
     *
     * @param uwbSessionData The session data to configure the session with.
     */
    virtual void
    ConfigureImpl(const std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> configParams) override;

    /**
     * @brief Start ranging.
     */
    virtual void
    StartRangingImpl() override;

    /**
     * @brief Stop ranging.
     */
    virtual void
    StopRangingImpl() override;

    /**
     * @brief Attempt to add a controlee to this session.
     *
     * @param controleeMacAddress The mac address of the controlee. This is
     * expected to be in the mac address format configured for the session.
     * @return UwbStatus The status of the operation. UwbStatusGeneric::Ok is
     * returned if the controlee was successfully added.
     */
    virtual ::uwb::protocol::fira::UwbStatus
    TryAddControleeImpl(::uwb::UwbMacAddress controleeMacAddress) override;

    /**
     * @brief Get the Application Configuration Parameters object
     *
     * @param requestedTypes leave this as an empty vector to request all parameters
     * @return std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
     */
    virtual std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
    GetApplicationConfigurationParametersImpl(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameterType> requestedTypes) override;

    /**
     * @brief Set the Application Configuration Parameters object
     *
     * @param uwbApplicationConfigurationParameters
     */
    virtual void
    SetApplicationConfigurationParametersImpl(std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters) override;

    /**
     * @brief Get the current state for this session.
     *
     * @return ::uwb::protocol::fira::UwbSessionState
     */
    virtual ::uwb::protocol::fira::UwbSessionState
    GetSessionStateImpl() override;

    /**
     * @brief Destroy the session, making it unusable.
     */
    void
    DestroyImpl() override;

protected:
    /**
     * @brief Obtain a shared instance of the device driver connector.
     *
     * @return std::shared_ptr<IUwbSessionDdiConnector>
     */
    std::shared_ptr<IUwbSessionDdiConnector>
    GetUwbSessionConnector() noexcept;

private:
    std::shared_ptr<IUwbSessionDdiConnector> m_uwbSessionConnector;
    std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionEnded> m_onSessionEndedCallback;
    std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStarted> m_onRangingStartedCallback;
    std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnRangingStopped> m_onRangingStoppedCallback;
    std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnPeerPropertiesChanged> m_onPeerPropertiesChangedCallback;
    std::shared_ptr<::uwb::UwbRegisteredSessionEventCallbackTypes::OnSessionMembershipChanged> m_onSessionMembershipChangedCallback;
    ::uwb::UwbRegisteredSessionEventCallbackTokens m_registeredCallbacksTokens;
};

} // namespace windows::devices::uwb

#endif // WINDOWS_UWB_SESSION_HXX
