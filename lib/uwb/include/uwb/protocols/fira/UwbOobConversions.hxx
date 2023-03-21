
#ifndef UWB_OOB_CONVERSIONS_HXX
#define UWB_OOB_CONVERSIONS_HXX

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Converts the config params given by OOB to config params that UCI needs
 *
 * @return std::vector<UwbApplicationConfigurationParameter>
 */
std::vector<UwbApplicationConfigurationParameter>
GetUciConfigParams(const UwbConfiguration& uwbConfiguration);

} // namespace uwb::protocol::fira

#endif // UWB_OOB_CONVERSIONS_HXX
