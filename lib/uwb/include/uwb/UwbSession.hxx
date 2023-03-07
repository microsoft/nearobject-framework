
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
     */
    UwbSession();

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
     * @brief Configure the session for use. Also moves the callbacks into ownership here
     *
     * TODO: This probably needs to return something to indicate whether it was
     * successful or not.
     *
     * @param uwbSessionData The session configuration to use. This should have
     * been obtained as a result of out-of-band negotiation.
     * @param callbacks The callbacks to invoke for session events.
     */
    void
    Configure(const protocol::fira::UwbSessionData& uwbSessionData, std::weak_ptr<UwbSessionEventCallbacks> callbacks);

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
    ConfigureImpl(const protocol::fira::UwbSessionData& uwbSessionData) = 0;

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
