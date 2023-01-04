
#ifndef WINDOWS_UWB_CX_ADAPTER_HXX
#define WINDOWS_UWB_CX_ADAPTER_HXX

#include <cstdint>

// NB: This must come before any other Windows include
#include <UwbCxLrpDeviceGlue.h>
#include <wil/resource.h>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>

namespace windows::devices::uwb
{
std::unique_ptr<uint8_t[]>
GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData);
} // namespace windows::devices::uwb

#endif // WINDOWS_UWB_CX_ADAPTER_HXX
