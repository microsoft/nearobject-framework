
#ifndef WINDOWS_UWB_SESSION_HXX
#define WINDOWS_UWB_SESSION_HXX

#include <cstdint>

// NB: This must come before any other Windows include
#include <windows.h>

#include <wil/resource.h>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>

namespace windows::devices::uwb
{
/**
 * @brief Windows concrete implementation of a UWB session.
 */
class UwbSession :
    public ::uwb::UwbSession
{
public:
    /**
     * @brief Construct a new UwbSession object.
     *
     * @param callbacks The event callback instance.
     * @param handleDriver File handle for a UWB-CX driver instance.
     */
    UwbSession(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, wil::shared_hfile handleDriver);

private:
    /**
     * @brief Configure the session for use.
     *
     * @param uwbSessionData The session data to configure the session with.
     */
    virtual void
    ConfigureImpl(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData) override;

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
     * @brief Add a new peer to the session.
     */
    virtual void
    AddPeerImpl(::uwb::UwbMacAddress peerMacAddress) override;

protected:
    /**
     * @brief Obtain a shared instance of the primary driver handle.
     *
     * @return wil::shared_hfile
     */
    wil::shared_hfile
    HandleDriver() noexcept;

private:
    wil::shared_hfile m_handleDriver;
};

} // namespace windows::devices::uwb

#endif // WINDOWS_UWB_SESSION_HXX
