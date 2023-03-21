
#ifndef WINDOWS_UWB_CX_ADAPTER_HXX
#define WINDOWS_UWB_CX_ADAPTER_HXX

#include <cstdint>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

#include <uwb/UwbSession.hxx>

namespace windows::devices::uwb
{
/**
 * @brief Generate neutral type UwbSetAppConfigParameters from UwbSessionData TODO this function needs to be updated to reflect the new differentiation between Uci and Oob app config params
 *
 * @param uwbSessionData
 * @return UwbSetAppConfigurationParameters
 */
UwbSetAppConfigurationParameters
GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData);
} // namespace windows::devices::uwb

#endif // WINDOWS_UWB_CX_ADAPTER_HXX
