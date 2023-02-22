
#ifndef UWB_CX_ADAPTER_DDI_LRP_HXX
#define UWB_CX_ADAPTER_DDI_LRP_HXX

#include <notstd/flextype_wrapper.hxx>

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

using UwbSessionUpdateMulicastListWrapper = notstd::flextype_wrapper<UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, std::remove_extent<decltype(UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST::controleeList)>>;

/**
 * @brief Converts UwbSessionUpdateMulicastList to UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST.
 *
 * @param uwbSessionUpdateMulicastList
 * @return UwbSessionUpdateMulicastListWrapper
 */
UwbSessionUpdateMulicastListWrapper
From(const ::uwb::protocol::fira::UwbSessionUpdateMulicastList &uwbSessionUpdateMulicastList);

using UwbSessionUpdateMulicastListStatusWrapper = notstd::flextype_wrapper<UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF, std::remove_extent<decltype(UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF::statusList)>>;

/**
 * @brief Converts UwbSessionUpdateMulicastListStatus to UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF.
 *
 * @param uwbSessionUpdateMulicastListStatus
 * @return UwbSessionUpdateMulicastListStatusWrapper
 */
UwbSessionUpdateMulicastListStatusWrapper
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

using UwbDeviceInformationWrapper = notstd::flextype_wrapper<UWB_DEVICE_INFO, std::remove_extent<decltype(UWB_DEVICE_INFO::vendorSpecificInfo)>>;

/**
 * @brief Converts UwbDeviceInformation to UWB_DEVICE_INFO.
 *
 * @param uwbDeviceInfo
 * @return UwbDeviceInformationWrapper
 */
UwbDeviceInformationWrapper
From(const ::uwb::protocol::fira::UwbDeviceInformation &uwbDeviceInfo);

using UwbDeviceCapabilitiesWrapper = notstd::flextype_wrapper<UWB_DEVICE_CAPABILITIES, std::remove_extent<decltype(UWB_DEVICE_CAPABILITIES::capabilityParams)>>;

/**
 * @brief Converts UwbCapability to UWB_DEVICE_CAPABILITIES.
 *
 * @param uwbDeviceCapabilities
 * @return UwbDeviceCapabilitiesWrapper
 */
UwbDeviceCapabilitiesWrapper
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

using UwbRangingDataWrapper = notstd::flextype_wrapper<UWB_RANGING_DATA, std::remove_extent<decltype(UWB_RANGING_DATA::rangingMeasurements)>>;

/**
 * @brief Converts UwbRangingData to UWB_RANGING_DATA.
 *
 * @param uwbRangingData
 * @return UwbRangingDataWrapper
 */
UwbRangingDataWrapper
From(const ::uwb::protocol::fira::UwbRangingData &uwbRangingData);

/**
 * @brief Converts UwbNotificationData to UWB_NOTIFICATION_DATA.
 *
 * @param uwbNotificationData
 * @return UWB_NOTIFICATION_DATA
 */
UWB_NOTIFICATION_DATA
From(const ::uwb::protocol::fira::UwbNotificationData &uwbNotificationData);

/**
 * @brief Converts UWB_DEVICE_CAPABILITIES to UwbCapability.
 *
 * @param deviceCapabilities
 * @return ::uwb::protocol::fira::UwbCapability
 */
::uwb::protocol::fira::UwbCapability
To(const UWB_DEVICE_CAPABILITIES &deviceCapabilities);

/**
 * @brief Converts UWB_DEVICE_STATUS to UwbStatusDevice.
 *
 * @param device_status
 * @return ::uwb::protocol::fira::UwbStatusDevice
 */
::uwb::protocol::fira::UwbStatusDevice
To(const UWB_DEVICE_STATUS &deviceStatus);

/**
 * @brief Converts UWB_DEVICE_CONFIG_PARAM_TYPE to UwbDeviceConfigurationParameterType.
 *
 * @param deviceConfigurationParameterType
 * @return ::uwb::protocol::fira::UwbDeviceConfigurationParameterType
 */
::uwb::protocol::fira::UwbDeviceConfigurationParameterType
To(const UWB_DEVICE_CONFIG_PARAM_TYPE &deviceConfigurationParameterType);

/**
 * @brief Converts UWB_STATUS to UwbStatus.
 *
 * @param status
 * @return ::uwb::protocol::fira::UwbStatus
 */
::uwb::protocol::fira::UwbStatus
To(const UWB_STATUS &status);

/**
 * @brief Converts UWB_DEVICE_STATE to UwbDeviceState.
 *
 * @param deviceState
 * @return ::uwb::protocol::fira::UwbDeviceState
 */
::uwb::protocol::fira::UwbDeviceState
To(const UWB_DEVICE_STATE &deviceState);

/**
 * @brief Converts UWB_MULTICAST_ACTION to UwbMulticastAction.
 *
 * @param multicastAction
 * @return ::uwb::protocol::fira::UwbMulticastAction
 */
::uwb::protocol::fira::UwbMulticastAction
To(const UWB_MULTICAST_ACTION &multicastAction);

/**
 * @brief Converts UWB_MULTICAST_STATUS to UwbStatusMulticast.
 *
 * @param statusMulticast
 * @return ::uwb::protocol::fira::UwbStatusMulticast
 */
::uwb::protocol::fira::UwbStatusMulticast
To(const UWB_MULTICAST_STATUS &statusMulticast);

/**
 * @brief Converts UWB_MULTICAST_LIST_STATUS to UwbMulticastListStatus.
 *
 * @param multicastListStatus
 * @return ::uwb::protocol::fira::UwbMulticastListStatus
 */
::uwb::protocol::fira::UwbMulticastListStatus
To(const UWB_MULTICAST_LIST_STATUS &multicastListStatus);

/**
 * @brief Converts UWB_MULTICAST_CONTROLEE_LIST_ENTRY to UwbSessionUpdateMulticastListEntry.
 *
 * @param sessionUpdateMulticastListEntry
 * @return ::uwb::protocol::fira::UwbSessionUpdateMulticastListEntry
 */
::uwb::protocol::fira::UwbSessionUpdateMulticastListEntry
To(const UWB_MULTICAST_CONTROLEE_LIST_ENTRY &sessionUpdateMulticastListEntry);

/**
 * @brief Converts UWB_SESSION_STATE to UwbSessionState.
 *
 * @param sessionState
 * @return ::uwb::protocol::fira::UwbSessionState
 */
::uwb::protocol::fira::UwbSessionState
To(const UWB_SESSION_STATE &sessionState);

/**
 * @brief Converts UWB_SESSION_STATUS to UwbSessionStatus.
 *
 * @return ::uwb::protocol::fira::UwbSessionStatus
 */
::uwb::protocol::fira::UwbSessionStatus
To(const UWB_SESSION_STATUS &sessionStatus);

/**
 * @brief Converts UWB_RANGING_MEASUREMENT_TYPE to UwbRangingMeasurementType.
 *
 * @param rangingMeasurementType
 * @return ::uwb::protocol::fira::UwbRangingMeasurementType
 */
::uwb::protocol::fira::UwbRangingMeasurementType
To(const UWB_RANGING_MEASUREMENT_TYPE &rangingMeasurementType);

/**
 * @brief Converts UWB_SESSION_REASON_CODE to UwbSessionReasonCode.
 *
 * @param sessionReasonCode
 * @return ::uwb::protocol::fira::UwbSessionReasonCode
 */
::uwb::protocol::fira::UwbSessionReasonCode
To(const UWB_SESSION_REASON_CODE &sessionReasonCode);

/**
 * @brief Converts UWB_APP_CONFIG_PARAM_TYPE to UwbApplicationConfigurationParameterType.
 *
 * @param appConfigParameterType
 * @return ::uwb::protocol::fira::UwbApplicationConfigurationParameterType
 */
::uwb::protocol::fira::UwbApplicationConfigurationParameterType
To(const UWB_APP_CONFIG_PARAM_TYPE &appConfigParameterType);

/**
 * @brief Converts UWB_NOTIFICATION_DATA to UwbNotificationData.
 *
 * @param notificationData
 * @return ::uwb::protocol::fira::UwbNotificationData
 */
::uwb::protocol::fira::UwbNotificationData
To(const UWB_NOTIFICATION_DATA &notificationData);

} // namespace windows::devices::uwb::ddi::lrp

#endif // UWB_CX_ADAPTER_DDI_LRP_HXX
