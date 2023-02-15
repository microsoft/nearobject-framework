
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
 * @brief Converts UwbDeviceState to UWB_DEVICE_STATE.
 *
 * @param uwbDeviceState
 * @return UWB_DEVICE_STATE
 */
UWB_DEVICE_STATE
From(const ::uwb::protocol::fira::UwbDeviceState &uwbDeviceState);

/**
 * @brief Converts UwbMulticastAction to UWB_MULTICAST_ACTION.
 *
 * @param uwbMulticastAction
 * @return UWB_MULTICAST_ACTION
 */
UWB_MULTICAST_ACTION
From(const ::uwb::protocol::fira::UwbMulticastAction &uwbMulticastAction);

/**
 * @brief Converts UwbStatusMulticast to UWB_MULTICAST_STATUS.
 *
 * @param uwbStatusMulticast
 * @return UWB_MULTICAST_STATUS
 */
UWB_MULTICAST_STATUS
From(const ::uwb::protocol::fira::UwbStatusMulticast &uwbStatusMulticast);

/**
 * @brief Converts UwbStatusMulticast to UWB_MULTICAST_LIST_STATUS.
 *
 * @param uwbStatusMulticast
 * @return UWB_MULTICAST_LIST_STATUS
 */
UWB_MULTICAST_LIST_STATUS
From(const ::uwb::protocol::fira::UwbMulticastListStatus &uwbStatusMulticastList);

/**
 * @brief Converts UwbSessionUpdateMulticastListEntry to UWB_MULTICAST_CONTROLEE_LIST_ENTRY.
 *
 * @param uwbSessionUpdateMulticastListEntry
 * @return UWB_MULTICAST_CONTROLEE_LIST_ENTRY
 */
UWB_MULTICAST_CONTROLEE_LIST_ENTRY
From(const ::uwb::protocol::fira::UwbSessionUpdateMulticastListEntry &uwbSessionUpdateMulticastListEntry);

/**
 * @brief Converts UwbSessionUpdateMulicastList to UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST.
 *
 * @param uwbSessionUpdateMulicastList
 * @return UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST
 */
UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST
From(const ::uwb::protocol::fira::UwbSessionUpdateMulicastList &uwbSessionUpdateMulicastList);

/**
 * @brief Converts UwbSessionUpdateMulicastListStatus to UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF.
 *
 * @param uwbSessionUpdateMulicastListStatus
 * @return UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF
 */
UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF
From(const ::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus &uwbSessionUpdateMulicastListStatus);

/**
 * @brief Converts UwbRangingMeasurementType to UWB_RANGING_MEASUREMENT_TYPE.
 *
 * @param uwbRangingType
 * @return UWB_RANGING_MEASUREMENT_TYPE
 */
UWB_RANGING_MEASUREMENT_TYPE
From(const ::uwb::protocol::fira::UwbRangingMeasurementType &uwbRangingType);

/**
 * @brief Converts UwbSessionReasonCode to UWB_SESSION_REASON_CODE.
 *
 * @param uwbSessionReasonCode
 * @return UWB_SESSION_REASON_CODE
 */
UWB_SESSION_REASON_CODE
From(const ::uwb::protocol::fira::UwbSessionReasonCode &uwbSessionReasonCode);

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
 * @brief Converts UwbSessionStatus to UWB_SESSION_STATUS.
 *
 * @param uwbSessionStatus
 * @return UWB_SESSION_STATUS
 */
UWB_SESSION_STATUS
From(const ::uwb::protocol::fira::UwbSessionStatus &uwbSessionStatus);

/**
 * @brief Converts UwbDeviceInformation to UWB_DEVICE_INFO.
 *
 * @param uwbDeviceInfo
 * @return UWB_DEVICE_INFO
 */
UWB_DEVICE_INFO
From(const ::uwb::protocol::fira::UwbDeviceInformation &uwbDeviceInfo);

/**
 * @brief Converts UwbCapability to UWB_DEVICE_CAPABILITIES.
 *
 * @param uwbDeviceCapabilities
 * @return UWB_DEVICE_CAPABILITIES
 */
UWB_DEVICE_CAPABILITIES
From(const ::uwb::protocol::fira::UwbCapability &uwbDeviceCapabilities);

/**
 * @brief Converts UwbStatusDevice to UWB_DEVICE_STATUS.
 *
 * @param uwbStatusDevice
 * @return UWB_DEVICE_STATUS
 */
UWB_DEVICE_STATUS
From(const ::uwb::protocol::fira::UwbStatusDevice &uwbStatusDevice);

/**
 * @brief Converts UwbDeviceConfigurationParameterType to UWB_DEVICE_CONFIG_PARAM_TYPE.
 *
 * @param uwbDeviceConfigurationParameterType
 * @return UWB_DEVICE_CONFIG_PARAM_TYPE
 */
UWB_DEVICE_CONFIG_PARAM_TYPE
From(const ::uwb::protocol::fira::UwbDeviceConfigurationParameterType &uwbDeviceConfigurationParameterType);

/**
 * @brief Converts UwbRangingData to UWB_RANGING_DATA.
 *
 * @param uwbRangingData
 * @return UWB_RANGING_DATA
 */
UWB_RANGING_DATA
From(const ::uwb::protocol::fira::UwbRangingData &uwbRangingData);

/**
 * @brief Converts UwbNotificationData to UWB_NOTIFICATION_DATA.
 *
 * @param uwbNotificationData
 * @return UWB_NOTIFICATION_DATA
 */
UWB_NOTIFICATION_DATA
From(const ::uwb::protocol::fira::UwbNotificationData &uwbNotificationData);

} // namespace windows::devices::uwb::ddi::lrp

#endif // UWB_CX_ADAPTER_DDI_LRP_HXX
