
#ifndef UWB_CX_ADAPTER_DDI_LRP_HXX
#define UWB_CX_ADAPTER_DDI_LRP_HXX

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbApplicationConfiguration.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>

/**
 * @brief Define LRP DDI conversion functionality as free functions in a
 * namespace.
 *
 * The namespace name is intended to be used explicitly in the
 * function call, which can help identify the type of conversion being done.
 * This allows usage such as:
 *
 *  UwbStatus uwbStatus = GetStatus();
 *  UWB_STATUS ddiStatus = uwb::ddi::lrp::From(uwbStatus);
 */
namespace windows::devices::uwb::ddi::lrp
{
/**
 * @brief Converts UwbStatus to UWB_STATUS.
 *
 * @param uwbStatus
 * @return UWB_STATUS
 */
UWB_STATUS
From(const ::uwb::protocol::fira::UwbStatus &uwbStatus);

/**
 * @brief Converts UwbApplicationConfigurationParameterType to UWB_APP_CONFIG_PARAM_TYPE.
 *
 * @param uwbApplicationConfigurationParameterType
 * @return UWB_APP_CONFIG_PARAM_TYPE
 */
UWB_APP_CONFIG_PARAM_TYPE
From(const ::uwb::protocol::fira::UwbApplicationConfigurationParameterType &uwbApplicationConfigurationParameterType);

/**
 * @brief Converts UwbSessionState to UWB_SESSION_STATE.
 *
 * @param uwbSessionState
 * @return UWB_SESSION_STATE
 */
UWB_SESSION_STATE
From(const ::uwb::protocol::fira::UwbSessionState uwbSessionState);

/**
 * @brief Converts UwbDeviceInfoInformation to UWB_DEVICE_INFO.
 *
 * @param uwbDeviceInfo
 * @return UWB_DEVICE_INFO
 */
UWB_DEVICE_INFO
From(const ::uwb::protocol::fira::UwbDeviceInfoInformation &uwbDeviceInfo);

/**
 * @brief Converts UwbCapability to UWB_DEVICE_CAPABILITIES.
 *
 * @param uwbDeviceCapabilities
 * @return UWB_DEVICE_CAPABILITIES
 */
UWB_DEVICE_CAPABILITIES
From(const ::uwb::protocol::fira::UwbCapability &uwbDeviceCapabilities);

/**
 * @brief Converts UwbDeviceConfigurationParameterType to UWB_DEVICE_CONFIG_PARAM_TYPE.
 *
 * @param uwbDeviceConfigurationParameterType
 * @return UWB_DEVICE_CONFIG_PARAM_TYPE
 */
UWB_DEVICE_CONFIG_PARAM_TYPE
From(const ::uwb::protocol::fira::UwbDeviceConfigurationParameterType &uwbDeviceConfigurationParameterType);

} // namespace windows::devices::uwb::ddi::lrp

#endif // UWB_CX_ADAPTER_DDI_LRP_HXX
