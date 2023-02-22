
#include <algorithm>
#include <bitset>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <variant>
#include <vector>

#include <notstd/utility.hxx>
#include <plog/Log.h>
#include <wil/common.h>

#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

using namespace ::uwb::protocol::fira;
using namespace windows::devices::uwb::ddi::lrp;

UWB_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbStatus &uwbStatus)
{
    static const std::unordered_map<UwbStatusGeneric, UWB_STATUS> StatusMapGeneric{
        { UwbStatusGeneric::Ok, UWB_STATUS_OK },
        { UwbStatusGeneric::Rejected, UWB_STATUS_REJECTED },
        { UwbStatusGeneric::Failed, UWB_STATUS_FAILED },
        { UwbStatusGeneric::SyntaxError, UWB_STATUS_SYNTAX_ERROR },
        { UwbStatusGeneric::InvalidParameter, UWB_STATUS_INVALID_PARAM },
        { UwbStatusGeneric::InvalidRange, UWB_STATUS_INVALID_RANGE },
        { UwbStatusGeneric::InvalidMessageSize, UWB_STATUS_INVALID_MESSAGE_SIZE },
        { UwbStatusGeneric::UnknownGid, UWB_STATUS_UNKNOWN_GID },
        { UwbStatusGeneric::UnknownOid, UWB_STATUS_UNKNOWN_OID },
        { UwbStatusGeneric::ReadOnly, UWB_STATUS_READ_ONLY },
        { UwbStatusGeneric::CommandRetry, UWB_STATUS_COMMAND_RETRY },
    };
    static const std::unordered_map<UwbStatusSession, UWB_STATUS> StatusMapSession{
        { UwbStatusSession::NotExist, UWB_STATUS_ERROR_SESSION_NOT_EXIST },
        { UwbStatusSession::Duplicate, UWB_STATUS_ERROR_SESSION_DUPLICATE },
        { UwbStatusSession::Active, UWB_STATUS_ERROR_SESSION_ACTIVE },
        { UwbStatusSession::MaxSessionsExceeded, UWB_STATUS_ERROR_MAX_SESSIONS_EXCEEDED },
        { UwbStatusSession::NotConfigured, UWB_STATUS_ERROR_SESSION_NOT_CONFIGURED },
        { UwbStatusSession::ActiveSessionsOngoing, UWB_STATUS_ERROR_ACTIVE_SESSIONS_ONGOING },
        { UwbStatusSession::MulticastListFull, UWB_STATUS_ERROR_MULTICAST_LIST_FULL },
        { UwbStatusSession::AddressNotFound, UWB_STATUS_ERROR_ADDRESS_NOT_FOUND },
        { UwbStatusSession::AddressAlreadyPresent, UWB_STATUS_ERROR_ADDRESS_ALREADY_PRESENT },
    };
    static const std::unordered_map<UwbStatusRanging, UWB_STATUS> StatusMapRanging{
        { UwbStatusRanging::TxFailed, UWB_STATUS_RANGING_TX_FAILED },
        { UwbStatusRanging::RxTimeout, UWB_STATUS_RANGING_RX_TIMEOUT },
        { UwbStatusRanging::RxPhyDecodingFailed, UWB_STATUS_RANGING_RX_PHY_DEC_FAILED },
        { UwbStatusRanging::RxPhyToaFailed, UWB_STATUS_RANGING_RX_PHY_TOA_FAILED },
        { UwbStatusRanging::RxPhyStsFailed, UWB_STATUS_RANGING_RX_PHY_STS_FAILED },
        { UwbStatusRanging::MacDecodingFailed, UWB_STATUS_RANGING_RX_MAC_DEC_FAILED },
        { UwbStatusRanging::RxMacIeDecodingFailed, UWB_STATUS_RANGING_RX_MAC_IE_DEC_FAILED },
        { UwbStatusRanging::RxMacIeMissing, UWB_STATUS_RANGING_RX_MAC_IE_MISSING },
    };

    UWB_STATUS status = UWB_STATUS_FAILED;

    std::visit([&status](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, UwbStatusGeneric>) {
            status = StatusMapGeneric.at(arg);
        } else if constexpr (std::is_same_v<T, UwbStatusSession>) {
            status = StatusMapSession.at(arg);
        } else if constexpr (std::is_same_v<T, UwbStatusRanging>) {
            status = StatusMapRanging.at(arg);
        } else {
            throw std::runtime_error("unknown UwbStatus variant value encountered");
        }
    },
        uwbStatus);

    return status;
}

UWB_DEVICE_STATE
windows::devices::uwb::ddi::lrp::From(const UwbDeviceState &uwbDeviceState)
{
    static const std::unordered_map<UwbDeviceState, UWB_DEVICE_STATE> DeviceStateMap{
        { UwbDeviceState::Ready, UWB_DEVICE_STATE_READY },
        { UwbDeviceState::Active, UWB_DEVICE_STATE_ACTIVE },
        { UwbDeviceState::Error, UWB_DEVICE_STATE_ERROR },
    };

    return DeviceStateMap.at(uwbDeviceState);
}

UWB_MULTICAST_ACTION
windows::devices::uwb::ddi::lrp::From(const UwbMulticastAction &uwbMulticastAction)
{
    static const std::unordered_map<UwbMulticastAction, UWB_MULTICAST_ACTION> ActionMap{
        { UwbMulticastAction::AddShortAddress, UWB_MULTICAST_ACTION_ADD_SHORT_ADDRESS },
        { UwbMulticastAction::DeleteShortAddress, UWB_MULTICAST_ACTION_DELETE_SHORT_ADDRESS },
    };

    return ActionMap.at(uwbMulticastAction);
}

UWB_MULTICAST_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbStatusMulticast &uwbStatusMulticast)
{
    static const std::unordered_map<UwbStatusMulticast, UWB_MULTICAST_STATUS> StatusMap{
        { UwbStatusMulticast::OkUpdate, UWB_MULTICAST_STATUS_OK_MULTICAST_LIST_UPDATE },
        { UwbStatusMulticast::ErrorListFull, UWB_MULTICAST_STATUS_ERROR_MULTICAST_LIST_FULL },
        { UwbStatusMulticast::ErrorKeyFetchFail, UWB_MULTICAST_STATUS_ERROR_KEY_FETCH_FAIL },
        { UwbStatusMulticast::ErrorSubSessionIdNotFound, UWB_MULTICAST_STATUS_ERROR_SUB_SESSION_ID_NOT_FOUND },
    };

    return StatusMap.at(uwbStatusMulticast);
}

UWB_MULTICAST_LIST_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbMulticastListStatus &uwbStatusMulticastList)
{
    if (uwbStatusMulticastList.ControleeMacAddress.GetType() != ::uwb::UwbMacAddressType::Short) {
        throw std::runtime_error("UWB_MULTICAST_LIST_STATUS requires a short mac address");
    }

    UWB_MULTICAST_LIST_STATUS statusMulticastList{};
    statusMulticastList.size = sizeof statusMulticastList;
    statusMulticastList.controleeMacAddress = uwbStatusMulticastList.ControleeMacAddress.GetValueShort().value();
    statusMulticastList.subSessionId = uwbStatusMulticastList.SubSessionId;
    statusMulticastList.status = From(uwbStatusMulticastList.Status);

    return statusMulticastList;
}

UWB_MULTICAST_CONTROLEE_LIST_ENTRY
windows::devices::uwb::ddi::lrp::From(const UwbSessionUpdateMulticastListEntry &uwbSessionUpdateMulticastListEntry)
{
    if (uwbSessionUpdateMulticastListEntry.ControleeMacAddress.GetType() != ::uwb::UwbMacAddressType::Short) {
        throw std::runtime_error("UWB_MULTICAST_CONTROLEE_LIST_ENTRY requires a short mac address");
    }

    UWB_MULTICAST_CONTROLEE_LIST_ENTRY multicastListEntry{};
    multicastListEntry.size = sizeof multicastListEntry;
    multicastListEntry.subSessionId = uwbSessionUpdateMulticastListEntry.SubSessionId;
    multicastListEntry.shortAddress = uwbSessionUpdateMulticastListEntry.ControleeMacAddress.GetValueShort().value();

    return multicastListEntry;
}

UwbSessionUpdateMulicastListWrapper
windows::devices::uwb::ddi::lrp::From(const UwbSessionUpdateMulicastList &uwbSessionUpdateMulicastList)
{
    auto sessionUpdateControllerMulticastListWrapper = UwbSessionUpdateMulicastListWrapper::from_num_elements(std::size(uwbSessionUpdateMulicastList.Controlees));
    UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST &sessionUpdateControllerMulticastList = sessionUpdateControllerMulticastListWrapper;
    sessionUpdateControllerMulticastList.size = sessionUpdateControllerMulticastListWrapper.size();
    sessionUpdateControllerMulticastList.sessionId = uwbSessionUpdateMulicastList.SessionId;
    sessionUpdateControllerMulticastList.action = From(uwbSessionUpdateMulicastList.Action);
    sessionUpdateControllerMulticastList.numberOfControlees = std::size(uwbSessionUpdateMulicastList.Controlees);

    for (auto i = 0; i < std::size(uwbSessionUpdateMulicastList.Controlees); i++) {
        auto &controlee = sessionUpdateControllerMulticastList.controleeList[i];
        controlee = From(uwbSessionUpdateMulicastList.Controlees[i]);
    }

    return sessionUpdateControllerMulticastListWrapper;
}

UwbSessionUpdateMulicastListStatusWrapper
windows::devices::uwb::ddi::lrp::From(const UwbSessionUpdateMulicastListStatus &uwbSessionUpdateMulicastListStatus)
{
    auto multicastListStatusWrapper = UwbSessionUpdateMulicastListStatusWrapper::from_num_elements(std::size(uwbSessionUpdateMulicastListStatus.Status));
    UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF &multicastListStatus = multicastListStatusWrapper;
    multicastListStatus.size = multicastListStatusWrapper.size();
    multicastListStatus.sessionId = uwbSessionUpdateMulicastListStatus.SessionId;
    multicastListStatus.numberOfControlees = std::size(uwbSessionUpdateMulicastListStatus.Status);
    multicastListStatus.remainingMulticastListSize = 0;

    for (auto i = 0; i < std::size(uwbSessionUpdateMulicastListStatus.Status); i++) {
        auto &status = multicastListStatus.statusList[i];
        status = From(uwbSessionUpdateMulicastListStatus.Status[i]);
    }

    return multicastListStatusWrapper;
}

UWB_RANGING_MEASUREMENT_TYPE
windows::devices::uwb::ddi::lrp::From(const UwbRangingMeasurementType &uwbRangingType)
{
    static const std::unordered_map<UwbRangingMeasurementType, UWB_RANGING_MEASUREMENT_TYPE> RangingTypeMap{
        { UwbRangingMeasurementType::TwoWay, UWB_RANGING_MEASUREMENT_TYPE_TWO_WAY },
    };

    return RangingTypeMap.at(uwbRangingType);
}

UWB_SESSION_REASON_CODE
windows::devices::uwb::ddi::lrp::From(const UwbSessionReasonCode &uwbSessionReasonCode)
{
    static const std::unordered_map<UwbSessionReasonCode, UWB_SESSION_REASON_CODE> SessionReasonCodeMap{
        { UwbSessionReasonCode::StateChangeWithSessionManagementCommands, UWB_SESSION_REASON_CODE_STATE_CHANGE_WITH_SESSION_MANAGEMENT_COMMANDS },
        { UwbSessionReasonCode::MaxRangignRoundRetryCountReached, UWB_SESSION_REASON_CODE_MAX_RANGING_ROUND_RETRY_COUNT_REACHED },
        { UwbSessionReasonCode::MaxNumberOfMeasurementsReached, UWB_SESSION_REASON_CODE_MAX_NUMBER_OF_MEASUREMENTS_REACHED },
        { UwbSessionReasonCode::ErrorSlotLengthNotSupported, UWB_SESSION_REASON_CODE_ERROR_SLOT_LENGTH_NOT_SUPPORTED },
        { UwbSessionReasonCode::ErrorInsufficientSlotsPerRangingRound, UWB_SESSION_REASON_CODE_ERROR_INSUFFICIENT_SLOTS_PER_RR },
        { UwbSessionReasonCode::ErrorMacAddressModeNotSupported, UWB_SESSION_REASON_CODE_ERROR_MAC_ADDRESS_MODE_NOT_SUPPORTED },
        { UwbSessionReasonCode::ErrorInvalidRangingInterval, UWB_SESSION_REASON_CODE_ERROR_INVALID_RANGING_INTERVAL },
        { UwbSessionReasonCode::ErrorInvalidStsConfiguration, UWB_SESSION_REASON_CODE_ERROR_INVALID_STS_CONFIG },
        { UwbSessionReasonCode::ErrorInvalidRFrameConfiguration, UWB_SESSION_REASON_CODE_ERROR_INVALID_RFRAME_CONFIG },
    };

    return SessionReasonCodeMap.at(uwbSessionReasonCode);
}

UWB_DEVICE_CONFIG_PARAM_TYPE
windows::devices::uwb::ddi::lrp::From(const UwbDeviceConfigurationParameterType &uwbDeviceConfigurationParameterType)
{
    static const std::unordered_map<UwbDeviceConfigurationParameterType, UWB_DEVICE_CONFIG_PARAM_TYPE> ConfigParamMap{
        { UwbDeviceConfigurationParameterType::DeviceState, UWB_DEVICE_CONFIG_PARAM_TYPE_DEVICE_STATE },
        { UwbDeviceConfigurationParameterType::LowPowerMode, UWB_DEVICE_CONFIG_PARAM_TYPE_LOW_POWER_MODE },
    };

    return ConfigParamMap.at(uwbDeviceConfigurationParameterType);
}

UWB_APP_CONFIG_PARAM_TYPE
windows::devices::uwb::ddi::lrp::From(const UwbApplicationConfigurationParameterType &uwbApplicationConfigurationParameterType)
{
    static const std::unordered_map<UwbApplicationConfigurationParameterType, UWB_APP_CONFIG_PARAM_TYPE> AppConfigParamMap{
        { UwbApplicationConfigurationParameterType::DeviceType, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE },
        { UwbApplicationConfigurationParameterType::RangingRoundUsage, UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE },
        { UwbApplicationConfigurationParameterType::StsConfiguration, UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG },
        { UwbApplicationConfigurationParameterType::MultiNodeMode, UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE },
        { UwbApplicationConfigurationParameterType::ChannelNumber, UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER },
        { UwbApplicationConfigurationParameterType::NumberOfControlees, UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES },
        { UwbApplicationConfigurationParameterType::DeviceMacAddress, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS },
        { UwbApplicationConfigurationParameterType::ControleeMacAddress, UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS },
        { UwbApplicationConfigurationParameterType::SlotDuration, UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION },
        { UwbApplicationConfigurationParameterType::RangingInterval, UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL },
        { UwbApplicationConfigurationParameterType::StsIndex, UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX },
        { UwbApplicationConfigurationParameterType::MacFcsType, UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE },
        { UwbApplicationConfigurationParameterType::RangingRoundControl, UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL },
        { UwbApplicationConfigurationParameterType::AoAResultRequest, UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ },
        { UwbApplicationConfigurationParameterType::RangeDataNotificationConfig, UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG },
        { UwbApplicationConfigurationParameterType::RangeDataNotificationProximityNear, UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR },
        { UwbApplicationConfigurationParameterType::RangeDataNotificationProximityFar, UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR },
        { UwbApplicationConfigurationParameterType::DeviceRole, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE },
        { UwbApplicationConfigurationParameterType::RFrameConfiguration, UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG },
        { UwbApplicationConfigurationParameterType::PreambleCodeIndex, UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX },
        { UwbApplicationConfigurationParameterType::SfdId, UWB_APP_CONFIG_PARAM_TYPE_SFD_ID },
        { UwbApplicationConfigurationParameterType::PsduDataRate, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE },
        { UwbApplicationConfigurationParameterType::PreambleDuration, UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION },
        { UwbApplicationConfigurationParameterType::RangingTimeStruct, UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT },
        { UwbApplicationConfigurationParameterType::SlotsPerRangingRound, UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR },
        { UwbApplicationConfigurationParameterType::TxAdaptivePayloadPower, UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER },
        { UwbApplicationConfigurationParameterType::ResponderSlotIndex, UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX },
        { UwbApplicationConfigurationParameterType::PrfMode, UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE },
        { UwbApplicationConfigurationParameterType::ScheduledMode, UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE },
        { UwbApplicationConfigurationParameterType::KeyRotation, UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION },
        { UwbApplicationConfigurationParameterType::KeyRotationRate, UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE },
        { UwbApplicationConfigurationParameterType::SessionPriority, UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY },
        { UwbApplicationConfigurationParameterType::MacAddressMode, UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE },
        { UwbApplicationConfigurationParameterType::VendorId, UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID },
        { UwbApplicationConfigurationParameterType::StaticStsIv, UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV },
        { UwbApplicationConfigurationParameterType::NumberOfStsSegments, UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS },
        { UwbApplicationConfigurationParameterType::MaxRangingRoundRetry, UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY },
        { UwbApplicationConfigurationParameterType::UwbInitiationTime, UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME },
        { UwbApplicationConfigurationParameterType::HoppingMode, UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE },
        { UwbApplicationConfigurationParameterType::BlockStrideLength, UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH },
        { UwbApplicationConfigurationParameterType::ResultReportConfig, UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG },
        { UwbApplicationConfigurationParameterType::InBandTerminationAttemptCount, UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT },
        { UwbApplicationConfigurationParameterType::SubSessionId, UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID },
        { UwbApplicationConfigurationParameterType::BprfPhrDataRate, UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE },
        { UwbApplicationConfigurationParameterType::MaxNumberOfMeasurements, UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS },
        { UwbApplicationConfigurationParameterType::StsLength, UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH },
    };

    return AppConfigParamMap.at(uwbApplicationConfigurationParameterType);
}

UWB_SESSION_STATE
windows::devices::uwb::ddi::lrp::From(const UwbSessionState uwbSessionState)
{
    static const std::unordered_map<UwbSessionState, UWB_SESSION_STATE> SessionStateMap{
        { UwbSessionState::Initialized, UWB_SESSION_STATE_INIT },
        { UwbSessionState::Deinitialized, UWB_SESSION_STATE_DEINIT },
        { UwbSessionState::Active, UWB_SESSION_STATE_ACTIVE },
        { UwbSessionState::Idle, UWB_SESSION_STATE_IDLE },
    };

    return SessionStateMap.at(uwbSessionState);
}

UWB_SESSION_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbSessionStatus &uwbSessionStatus)
{
    UWB_SESSION_STATUS sessionStatus{};
    sessionStatus.size = sizeof sessionStatus;
    sessionStatus.sessionId = uwbSessionStatus.SessionId;
    sessionStatus.state = From(uwbSessionStatus.State);
    if (uwbSessionStatus.ReasonCode.has_value()) {
        sessionStatus.reasonCode = From(uwbSessionStatus.ReasonCode.value());
    }

    return sessionStatus;
}

UwbDeviceInformationWrapper
windows::devices::uwb::ddi::lrp::From(const UwbDeviceInformation &uwbDeviceInfo)
{
    std::size_t numElements = 0;
    std::span<uint8_t> vendorSpecificInfo;

    if (uwbDeviceInfo.VendorSpecificInfo != nullptr) {
        vendorSpecificInfo = uwbDeviceInfo.VendorSpecificInfo->GetData();
        numElements = std::size(vendorSpecificInfo);
    } else {
        numElements = 1;
    }

    auto deviceInfoWrapper = UwbDeviceInformationWrapper::from_num_elements(numElements);
    UWB_DEVICE_INFO &deviceInfo = deviceInfoWrapper;
    deviceInfo.size = deviceInfoWrapper.size();
    deviceInfo.status = From(uwbDeviceInfo.Status);
    deviceInfo.uciGenericVersionMajor = uwbDeviceInfo.VersionUci.Major;
    deviceInfo.uciGenericVersionMinorAndMaintenance = uwbDeviceInfo.VersionUci.Minor | uwbDeviceInfo.VersionUci.Maintenance;
    deviceInfo.uciTestVersionMajor = uwbDeviceInfo.VersionUciTest.Major;
    deviceInfo.uciTestVersionMinorAndMaintenance = uwbDeviceInfo.VersionUciTest.Minor | uwbDeviceInfo.VersionUciTest.Maintenance;
    deviceInfo.macVersionMajor = uwbDeviceInfo.VersionMac.Major;
    deviceInfo.macVersionMinorAndMaintenance = uwbDeviceInfo.VersionMac.Minor | uwbDeviceInfo.VersionMac.Maintenance;
    deviceInfo.phyVersionMajor = uwbDeviceInfo.VersionPhy.Major;
    deviceInfo.phyVersionMinorAndMaintenance = uwbDeviceInfo.VersionPhy.Minor | uwbDeviceInfo.VersionPhy.Maintenance;
    deviceInfo.vendorSpecificInfoLength = std::size(vendorSpecificInfo);
    if (deviceInfo.vendorSpecificInfoLength > 0) {
        std::memcpy(&deviceInfo.vendorSpecificInfo[0], std::data(vendorSpecificInfo), std::size(vendorSpecificInfo));
    }

    return deviceInfoWrapper;
}

UwbDeviceCapabilitiesWrapper
windows::devices::uwb::ddi::lrp::From(const UwbCapability &uwbDeviceCapabilities)
{
    std::size_t numElements = 2; // TODO: calculate this from uwbDeviceCapabilities
    auto deviceCapabilitiesWrapper = UwbDeviceCapabilitiesWrapper::from_num_elements(numElements);

    UWB_DEVICE_CAPABILITIES &deviceCapabilities = deviceCapabilitiesWrapper;
    deviceCapabilities.size = deviceCapabilitiesWrapper.size();
    deviceCapabilities.capabilityParamsCount = numElements;

    // TODO: fill in deviceCapabilities.capabilityParams. There is currently no
    // generic list of capabilities, so, we may have to convert each capability
    // one-by-one.

    return deviceCapabilitiesWrapper;
}

UWB_DEVICE_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbStatusDevice &uwbStatusDevice)
{
    UWB_DEVICE_STATUS statusDevice{};
    statusDevice.size = sizeof statusDevice;
    statusDevice.deviceState = From(uwbStatusDevice.State);

    return statusDevice;
}

UwbRangingDataWrapper
windows::devices::uwb::ddi::lrp::From(const UwbRangingData &uwbRangingData)
{
    auto rangingDataWrapper = UwbRangingDataWrapper::from_num_elements(std::size(uwbRangingData.RangingMeasurements));
    UWB_RANGING_DATA &rangingData = rangingDataWrapper;
    rangingData.size = rangingDataWrapper.size();
    rangingData.sequenceNumber = uwbRangingData.SequenceNumber;
    rangingData.sessionId = uwbRangingData.SessionId;
    rangingData.currentRangingInterval = uwbRangingData.CurrentRangingInterval;
    rangingData.rangingMeasurementType = From(uwbRangingData.RangingMeasurementType);
    rangingData.numberOfRangingMeasurements = std::size(uwbRangingData.RangingMeasurements);

    return rangingDataWrapper;
}

UWB_NOTIFICATION_DATA
windows::devices::uwb::ddi::lrp::From(const UwbNotificationData &uwbNotificationData)
{
    static const std::unordered_map<std::type_index, UWB_NOTIFICATION_TYPE> NotificationTypeMap{
        { std::type_index(typeid(UwbStatus)), UWB_NOTIFICATION_TYPE_GENERIC_ERROR },
        { std::type_index(typeid(UwbStatusDevice)), UWB_NOTIFICATION_TYPE_DEVICE_STATUS },
        { std::type_index(typeid(UwbSessionStatus)), UWB_NOTIFICATION_TYPE_SESSION_STATUS },
        { std::type_index(typeid(UwbSessionUpdateMulicastListStatus)), UWB_NOTIFICATION_TYPE_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST },
        { std::type_index(typeid(UwbRangingData)), UWB_NOTIFICATION_TYPE_RANGING_DATA },
    };

    UWB_NOTIFICATION_DATA notificationData{};

    std::visit([&notificationData](auto &&arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        notificationData.notificationType = NotificationTypeMap.at(typeid(arg));

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            notificationData.genericError = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            notificationData.deviceStatus = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            notificationData.sessionStatus = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionUpdateMulicastListStatus>) {
            notificationData.sessionUpdateControllerMulticastList = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbRangingData>) {
            notificationData.rangingData = From(arg);
        }
        // Note: no else clause is needed here since if the type is not
        // supported, the at() call above will throw std::out_of_range, ensuring
        // this code will never be reached.
    },
        uwbNotificationData);

    return notificationData;
}

namespace detail
{
/**
 * @brief Helper to process supported parameters from a given map which
 * associates a value with a bit position. If the specified bitmap contains
 * support for the value, the value is added to the result container.
 *
 * @tparam N The size of the bitset.
 * @tparam T The type of the value.
 * @param map The map associating values with bit positions in the bitset.
 * @param support The bitset defining parameter support.
 * @param result The vector to hold supported values that are present in the bitset.
 */
template <std::size_t N, typename T>
void
ProcessSupportFromBitset(std::unordered_map<T, std::size_t> map, const std::bitset<N> &support, std::vector<T> &result)
{
    for (const auto &[value, bit] : map) {
        if (support.test(bit)) {
            result.push_back(value);
        }
    }
}
} // namespace detail

UwbCapability
windows::devices::uwb::ddi::lrp::To(const UWB_DEVICE_CAPABILITIES &deviceCapabilities)
{
    UwbCapability uwbCapability;

    for (const auto capability : wil::make_range(&deviceCapabilities.capabilityParams[0], deviceCapabilities.capabilityParamsCount)) {
        switch (capability.paramType) {
        case UWB_CAPABILITY_PARAM_TYPE_RANGING_METHOD: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<4> rangingMethods{ value };
            detail::ProcessSupportFromBitset(UwbCapability::RangingMethodBit, rangingMethods, uwbCapability.RangingMethods);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_DEVICE_ROLES: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<2> deviceRoles{ value };
            detail::ProcessSupportFromBitset(UwbCapability::DeviceRoleBit, deviceRoles, uwbCapability.DeviceRoles);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_PHY_VERSION_RANGE: {
            const auto value = *reinterpret_cast<const uint32_t *>(&capability.paramValue);
            uwbCapability.FiraPhyVersionRange = value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_MAC_VERSION_RANGE: {
            const auto value = *reinterpret_cast<const uint32_t *>(&capability.paramValue);
            uwbCapability.FiraMacVersionRange = value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_BLOCK_STRIDING: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<1> blockStriding{ value };
            uwbCapability.BlockStriding = blockStriding.test(UwbCapability::BlockStridingBit);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_HOPPING_MODE: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<1> hoppingMode{ value };
            uwbCapability.HoppingMode = hoppingMode.test(UwbCapability::HoppingModeBit);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_SCHEDULED_MODE: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<2> schedulingModes{ value };
            detail::ProcessSupportFromBitset(UwbCapability::SchedulingModeBit, schedulingModes, uwbCapability.SchedulingModes);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_RANGING_TIME_STRUCT: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<2> rangingTimeStructs{ value };
            detail::ProcessSupportFromBitset(UwbCapability::RangingModeBit, rangingTimeStructs, uwbCapability.RangingTimeStructs);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_MULTI_NODE_MODE: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<3> modes{ value };
            detail::ProcessSupportFromBitset(UwbCapability::MultiNodeModeBit, modes, uwbCapability.MultiNodeModes);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_UWB_INITIATION_TIME: {
            const uint8_t value = capability.paramValue[0];
            uwbCapability.UwbInitiationTime = !!value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_STS_CONFIG: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<3> stsConfigurations{ value };
            detail::ProcessSupportFromBitset(UwbCapability::StsConfigurationBit, stsConfigurations, uwbCapability.StsConfigurations);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_RFRAME_CONFIG: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<4> rframeConfigurations{ value };
            detail::ProcessSupportFromBitset(UwbCapability::RFrameConfigurationBit, rframeConfigurations, uwbCapability.RFrameConfigurations);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_AOA_SUPPORT: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<4> aoaTypes{ value };
            detail::ProcessSupportFromBitset(UwbCapability::AngleOfArrivalBit, aoaTypes, uwbCapability.AngleOfArrivalTypes);
            uwbCapability.AngleOfArrivalFom = aoaTypes.test(UwbCapability::AngleOfArrivalFomBit);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_EXTENDED_MAC_ADDRESS: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            uwbCapability.ExtendedMacAddress = !!value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_CC_CONSTRAINT_LENGTH: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<2> convolutionalCodeConstraintLengths{ value };
            detail::ProcessSupportFromBitset(UwbCapability::ConvolutionalCodeConstraintLengthsBit, convolutionalCodeConstraintLengths, uwbCapability.ConvolutionalCodeConstraintLengths);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_CHANNELS: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<8> channels{ value };
            detail::ProcessSupportFromBitset(UwbCapability::ChannelsBit, channels, uwbCapability.Channels);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_BPRF_PARAMETER_SETS: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<6> bprfParameterSets{ value };
            detail::ProcessSupportFromBitset(UwbCapability::BprfParameterSetsBit, bprfParameterSets, uwbCapability.BprfParameterSets);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_HPRF_PARAMETER_SETS: {
            const auto value = *reinterpret_cast<const uint8_t *>(&capability.paramValue);
            std::bitset<35> hprfParameterSets{ value };
            detail::ProcessSupportFromBitset(UwbCapability::HprfParameterSetsBit, hprfParameterSets, uwbCapability.HprfParameterSets);
            break;
        }
        default:
            // ignore unknown parameter tags
            PLOG_DEBUG << "ignoring unknown UwbCapability parameter tag " << notstd::to_underlying(capability.paramType);
            break;
        }
    }

    return uwbCapability;
}

static const std::unordered_map<UWB_STATUS, UwbStatusGeneric> StatusToMapGeneric{
    { UWB_STATUS_OK, UwbStatusGeneric::Ok },
    { UWB_STATUS_REJECTED, UwbStatusGeneric::Rejected },
    { UWB_STATUS_FAILED, UwbStatusGeneric::Failed },
    { UWB_STATUS_SYNTAX_ERROR, UwbStatusGeneric::SyntaxError },
    { UWB_STATUS_INVALID_PARAM, UwbStatusGeneric::InvalidParameter },
    { UWB_STATUS_INVALID_RANGE, UwbStatusGeneric::InvalidRange },
    { UWB_STATUS_INVALID_MESSAGE_SIZE, UwbStatusGeneric::InvalidMessageSize },
    { UWB_STATUS_UNKNOWN_GID, UwbStatusGeneric::UnknownGid },
    { UWB_STATUS_UNKNOWN_OID, UwbStatusGeneric::UnknownOid },
    { UWB_STATUS_READ_ONLY, UwbStatusGeneric::ReadOnly },
    { UWB_STATUS_COMMAND_RETRY, UwbStatusGeneric::CommandRetry },
};
static const std::unordered_map<UWB_STATUS, UwbStatusSession> StatusToMapSession{
    { UWB_STATUS_ERROR_SESSION_NOT_EXIST, UwbStatusSession::NotExist },
    { UWB_STATUS_ERROR_SESSION_DUPLICATE, UwbStatusSession::Duplicate },
    { UWB_STATUS_ERROR_SESSION_ACTIVE, UwbStatusSession::Active },
    { UWB_STATUS_ERROR_MAX_SESSIONS_EXCEEDED, UwbStatusSession::MaxSessionsExceeded },
    { UWB_STATUS_ERROR_SESSION_NOT_CONFIGURED, UwbStatusSession::NotConfigured },
    { UWB_STATUS_ERROR_ACTIVE_SESSIONS_ONGOING, UwbStatusSession::ActiveSessionsOngoing },
    { UWB_STATUS_ERROR_MULTICAST_LIST_FULL, UwbStatusSession::MulticastListFull },
    { UWB_STATUS_ERROR_ADDRESS_NOT_FOUND, UwbStatusSession::AddressNotFound },
    { UWB_STATUS_ERROR_ADDRESS_ALREADY_PRESENT, UwbStatusSession::AddressAlreadyPresent },
};

UwbStatus
windows::devices::uwb::ddi::lrp::To(const UWB_STATUS &status)
{
    static const std::unordered_map<UWB_STATUS, UwbStatusRanging> StatusToMapRanging{
        { UWB_STATUS_RANGING_TX_FAILED, UwbStatusRanging::TxFailed },
        { UWB_STATUS_RANGING_RX_TIMEOUT, UwbStatusRanging::RxTimeout },
        { UWB_STATUS_RANGING_RX_PHY_DEC_FAILED, UwbStatusRanging::RxPhyDecodingFailed },
        { UWB_STATUS_RANGING_RX_PHY_TOA_FAILED, UwbStatusRanging::RxPhyToaFailed },
        { UWB_STATUS_RANGING_RX_PHY_STS_FAILED, UwbStatusRanging::RxPhyStsFailed },
        { UWB_STATUS_RANGING_RX_MAC_DEC_FAILED, UwbStatusRanging::MacDecodingFailed },
        { UWB_STATUS_RANGING_RX_MAC_IE_DEC_FAILED, UwbStatusRanging::RxMacIeDecodingFailed },
        { UWB_STATUS_RANGING_RX_MAC_IE_MISSING, UwbStatusRanging::RxMacIeMissing },
    };

    auto enumId = notstd::to_underlying(status);
    if (enumId < notstd::to_underlying(UWB_STATUS_ERROR_SESSION_NOT_EXIST)) {
        return StatusToMapGeneric.at(status);
    }
    if (enumId < notstd::to_underlying(UWB_STATUS_RANGING_TX_FAILED)) {
        return StatusToMapSession.at(status);
    }
    return StatusToMapRanging.at(status);
}

UwbStatusDevice
windows::devices::uwb::ddi::lrp::To(const UWB_DEVICE_STATUS &deviceStatus)
{
    static const std::unordered_map<UWB_DEVICE_STATE, UwbDeviceState> DeviceStateToMap{
        { UWB_DEVICE_STATE_READY, UwbDeviceState::Ready },
        { UWB_DEVICE_STATE_ACTIVE, UwbDeviceState::Active },
        { UWB_DEVICE_STATE_ERROR, UwbDeviceState::Error }
    };

    return UwbStatusDevice{
        .State = DeviceStateToMap.at(deviceStatus.deviceState)
    };
}

UwbDeviceState
windows::devices::uwb::ddi::lrp::To(const UWB_DEVICE_STATE &deviceState)
{
    static const std::unordered_map<UWB_DEVICE_STATE, UwbDeviceState> DeviceStateMap{
        { UWB_DEVICE_STATE_READY, UwbDeviceState::Ready },
        { UWB_DEVICE_STATE_ACTIVE, UwbDeviceState::Active },
        { UWB_DEVICE_STATE_ERROR, UwbDeviceState::Error },
    };

    return DeviceStateMap.at(deviceState);
}

UwbMulticastAction
windows::devices::uwb::ddi::lrp::To(const UWB_MULTICAST_ACTION &multicastAction)
{
    static const std::unordered_map<UWB_MULTICAST_ACTION, UwbMulticastAction> ActionMap{
        { UWB_MULTICAST_ACTION_ADD_SHORT_ADDRESS, UwbMulticastAction::AddShortAddress },
        { UWB_MULTICAST_ACTION_DELETE_SHORT_ADDRESS, UwbMulticastAction::DeleteShortAddress },
    };

    return ActionMap.at(multicastAction);
}

UwbSessionState
windows::devices::uwb::ddi::lrp::To(const UWB_SESSION_STATE &sessionState)
{
    static const std::unordered_map<UWB_SESSION_STATE, UwbSessionState> SessionStateToMap{
        { UWB_SESSION_STATE_INIT, UwbSessionState::Initialized },
        { UWB_SESSION_STATE_DEINIT, UwbSessionState::Deinitialized },
        { UWB_SESSION_STATE_ACTIVE, UwbSessionState::Active },
        { UWB_SESSION_STATE_IDLE, UwbSessionState::Idle },
    };

    return SessionStateToMap.at(sessionState);
}

UwbSessionReasonCode
windows::devices::uwb::ddi::lrp::To(const UWB_SESSION_REASON_CODE &sessionReasonCode)
{
    static const std::unordered_map<UWB_SESSION_REASON_CODE, UwbSessionReasonCode> SessionReasonCodeToMap{
        { UWB_SESSION_REASON_CODE_STATE_CHANGE_WITH_SESSION_MANAGEMENT_COMMANDS, UwbSessionReasonCode::StateChangeWithSessionManagementCommands },
        { UWB_SESSION_REASON_CODE_MAX_RANGING_ROUND_RETRY_COUNT_REACHED, UwbSessionReasonCode::MaxRangignRoundRetryCountReached },
        { UWB_SESSION_REASON_CODE_MAX_NUMBER_OF_MEASUREMENTS_REACHED, UwbSessionReasonCode::MaxNumberOfMeasurementsReached },
        { UWB_SESSION_REASON_CODE_ERROR_SLOT_LENGTH_NOT_SUPPORTED, UwbSessionReasonCode::ErrorSlotLengthNotSupported },
        { UWB_SESSION_REASON_CODE_ERROR_INSUFFICIENT_SLOTS_PER_RR, UwbSessionReasonCode::ErrorInsufficientSlotsPerRangingRound },
        { UWB_SESSION_REASON_CODE_ERROR_MAC_ADDRESS_MODE_NOT_SUPPORTED, UwbSessionReasonCode::ErrorMacAddressModeNotSupported },
        { UWB_SESSION_REASON_CODE_ERROR_INVALID_RANGING_INTERVAL, UwbSessionReasonCode::ErrorInvalidRangingInterval },
        { UWB_SESSION_REASON_CODE_ERROR_INVALID_STS_CONFIG, UwbSessionReasonCode::ErrorInvalidStsConfiguration },
        { UWB_SESSION_REASON_CODE_ERROR_INVALID_RFRAME_CONFIG, UwbSessionReasonCode::ErrorInvalidRFrameConfiguration },
    };

    return SessionReasonCodeToMap.at(sessionReasonCode);
}

UwbSessionStatus
windows::devices::uwb::ddi::lrp::To(const UWB_SESSION_STATUS &sessionStatus)
{
    return UwbSessionStatus{
        .SessionId = sessionStatus.sessionId,
        .State = To(sessionStatus.state),
        .ReasonCode = To(sessionStatus.reasonCode)
    };
}

UwbNotificationData
windows::devices::uwb::ddi::lrp::To(const UWB_NOTIFICATION_DATA &notificationData)
{
    switch (notificationData.notificationType) {
    case UWB_NOTIFICATION_TYPE_DEVICE_STATUS: {
        return To(notificationData.deviceStatus);
    }
    case UWB_NOTIFICATION_TYPE_GENERIC_ERROR: {
        return To(notificationData.genericError);
    }
    case UWB_NOTIFICATION_TYPE_SESSION_STATUS: {
        return To(notificationData.sessionStatus);
    }
    case UWB_NOTIFICATION_TYPE_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST: {
        break;
    }
    case UWB_NOTIFICATION_TYPE_RANGING_DATA:
        break;
    }
    return UwbNotificationData{};
}
