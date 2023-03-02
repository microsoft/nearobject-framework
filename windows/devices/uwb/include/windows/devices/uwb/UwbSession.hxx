
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
     * @brief TODO
     */
    void
    ConfigureImpl(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData) override;

    /**
     * @brief TODO
     */
    void
    StartRangingImpl() override;

    /**
     * @brief TODO
     */
    void
    StopRangingImpl() override;

    /**
     * @brief TODO
     */
    void
    AddPeerImpl(::uwb::UwbMacAddress peerMacAddress) override;

private:
    wil::shared_hfile m_handleDriver;
};

} // namespace windows::devices::uwb

#endif // WINDOWS_UWB_SESSION_HXX
