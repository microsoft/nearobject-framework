
#ifndef UWB_SESSION_SIMULATOR_HXX
#define UWB_SESSION_SIMULATOR_HXX

#include <memory>

#include <windows.h>

#include <wil/resource.h>

#include <uwb/UwbSessionEventCallbacks.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

namespace windows::devices::uwb::simulator
{
class UwbSessionSimulator :
    public windows::devices::uwb::UwbSession
{
public:
    // Inherit constructor for base class as nothing in for construction is different.
    using windows::devices::uwb::UwbSession::UwbSession;

    virtual ~UwbSessionSimulator() = default;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SESSION_SIMULATOR_HXX
