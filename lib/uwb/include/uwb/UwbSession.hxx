
#ifndef UWB_SESSION_HXX
#define UWB_SESSION_HXX

#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>
#include <unordered_set>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace uwb
{
struct UwbSessionEventCallbacks;

/**
 * @brief The possible reasons for a session ending.
 */
enum class UwbSessionEndReason {
    /**
     * @brief The session owner stopped the session.
     *
     * This is the reason used when the session ends naturally.
     */
    Stopped,

    /**
     * @brief The session was locally canceled.
     */
    Canceled,

    /**
     * @brief The session timed out due to policy.
     */
    Timeout,

    /**
     * @brief The session ended for an unknown or unspecified reason.
     */
    Unspecified,
};

/**
 * @brief Represents a UWB session.
 */
class UwbSession
{
public:
    /**
     * @brief Construct a new UwbSession object.
     *
     * @param callbacks The callbacks to invoke for session events.
     */
    UwbSession(std::weak_ptr<UwbSessionEventCallbacks> callbacks);

    /**
     * @brief Destroy the UwbSession object.
     */
    virtual ~UwbSession() = default;

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
     * TODO: This probably needs to return something to indicate whether it was
     * successful or not.
     *
     * @param uwbSessionData The session configuration to use. This should have
     * been obtained as a result of out-of-band negotiation.
     */
    void
    Configure(const protocol::fira::UwbSessionData& uwbSessionData);

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
     * @brief
     */
    void
    StartRanging();

    /**
     * @brief
     */
    void
    StopRanging();

    /**
     * @brief Set the Session Status object
     *
     */
    void
    SetSessionStatus(const uwb::protocol::fira::UwbSessionStatus&);

    /**
     * @brief Get the Session Status object
     *
     * @return const UwbSessionStatus&
     */
    const uwb::protocol::fira::UwbSessionStatus&
    GetSessionStatus();

private:
    virtual void
    ConfigureImpl(const protocol::fira::UwbSessionData& uwbSessionData) = 0;

    virtual void
    StartRangingImpl() = 0;

    virtual void
    StopRangingImpl() = 0;

    virtual void
    AddPeerImpl(UwbMacAddress peerMacAddress) = 0;

protected:
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
