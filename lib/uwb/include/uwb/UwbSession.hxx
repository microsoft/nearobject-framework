
#ifndef UWB_SESSION_HXX
#define UWB_SESSION_HXX

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <ranges>
#include <unordered_set>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbPeer.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace uwb
{
/**
 * @brief Represents a UWB session.
 */
class UwbSession
{
public:
    /**
     * @brief Construct a new UwbSession object.
     *
     * @param sessionId
     * @param callbacks
     * @param deviceType
     */
    UwbSession(uint32_t sessionId, std::weak_ptr<UwbSessionEventCallbacks> callbacks, uwb::protocol::fira::DeviceType deviceType = uwb::protocol::fira::DeviceType::Controller);

    /**
     * @brief Destroy the UwbSession object.
     */
    virtual ~UwbSession() = default;

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
     * @brief Add a peer to this session.
     *
     * @param peerMacAddress The mac address of the peer. This is expected to be
     * in the mac address format configured for the session.
     */
    void
    AddPeer(UwbMacAddress peerMacAddress);

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
     * @brief Get the application configuration parameters for this session.
     * 
     * @return std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> 
     */
    std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
    GetApplicationConfigurationParameters();

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
     * @brief Add a new peer to the session.
     *
     * @param peerMacAddress
     */
    virtual void
    AddPeerImpl(UwbMacAddress peerMacAddress) = 0;

    /**
     * @brief Get the application configuration parameters for this session.
     * 
     * @return std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter> 
     */
    virtual std::vector<::uwb::protocol::fira::UwbApplicationConfigurationParameter>
    GetApplicationConfigurationParametersImpl() = 0;

protected:
    uwb::protocol::fira::DeviceType m_deviceType{ uwb::protocol::fira::DeviceType::Controller };
    uint32_t m_sessionId{ 0 };
    uwb::protocol::fira::UwbSessionStatus m_sessionStatus{};
    UwbMacAddressType m_uwbMacAddressType{ UwbMacAddressType::Extended };
    UwbMacAddress m_uwbMacAddressSelf;
    std::atomic<bool> m_rangingActive{ false };
    std::mutex m_peerGate;
    std::unordered_set<UwbMacAddress> m_peers{};
    std::weak_ptr<UwbSessionEventCallbacks> m_callbacks;
};

} // namespace uwb

#endif // UWB_SESSION_HXX
