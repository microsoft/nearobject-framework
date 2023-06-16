
#ifndef UWB_OOB_CONVERSIONS_HXX
#define UWB_OOB_CONVERSIONS_HXX

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Converts the config params given by OOB to config params that UCI needs
 *
 * @param uwbConfiguration The UWB configuration data used to generate the UCI configuration parameter list
 * @param deviceType The type of device (Controller/Controlee)
 * @return std::vector<UwbApplicationConfigurationParameter>
 */
std::vector<UwbApplicationConfigurationParameter>
GetUciConfigParams(const UwbConfiguration& uwbConfiguration, DeviceType deviceType);

/**
 * @brief Converts the given UCI application configuration parameters to the OOB UwbSessionData equivalent
 *
 * @param applicationConfigurationParameters The UCI application configuration parameters used to construct UwbSessionData
 * @return UwbSessionData
 */
UwbSessionData
GetUwbSessionData(std::vector<UwbApplicationConfigurationParameter> applicationConfigurationParameters);

} // namespace uwb::protocol::fira

#endif // UWB_OOB_CONVERSIONS_HXX
