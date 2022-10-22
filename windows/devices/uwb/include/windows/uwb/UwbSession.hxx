
#ifndef WINDOWS_UWB_SESSION_HXX
#define WINDOWS_UWB_SESSION_HXX

#include <cstdint>

#include <windows.h>
#include <wil/resource.h>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbSession.hxx>

namespace windows
{
namespace devices
{

/**
 * @brief Windows concrete implementation of a UWB session. 
 */
class UwbSession :
    public uwb::UwbSession
{
public:
    /**
     * @brief Construct a new UwbSession object.
     * 
     * @param sessionId The unique session identifier.
     * @param handleDriver File handle for a UWB-CX driver instance.
     */
    UwbSession(uint32_t sessionId, wil::unique_hfile handleDriver);

private:
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
    AddPeerImpl(uwb::UwbMacAddress peerMacAddress) override;

private:
    wil::unique_hfile m_handleDriver;
};

} // namespace devices
} // namespace windows

#endif // WINDOWS_UWB_SESSION_HXX
