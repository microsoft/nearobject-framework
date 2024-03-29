
#include <algorithm>
#include <bit>
#include <bitset>
#include <functional>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <variant>
#include <vector>

#include <notstd/utility.hxx>
#include <plog/Log.h>
#include <wil/common.h>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

#include <magic_enum.hpp>

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

UWB_SESSION_TYPE
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbSessionType &uwbSessionType)
{
    static const std::unordered_map<UwbSessionType, UWB_SESSION_TYPE> SessionTypeMap{
        { UwbSessionType::RangingSession, UWB_SESSION_TYPE_RANGING_SESSION },
        { UwbSessionType::TestMode, UWB_SESSION_TYPE_DEVICE_TEST_MODE },
    };

    return SessionTypeMap.at(uwbSessionType);
}

UWB_LINE_OF_SIGHT_INDICATOR
windows::devices::uwb::ddi::lrp::From(const UwbLineOfSightIndicator &uwbLineOfSightIndicator)
{
    static const std::unordered_map<UwbLineOfSightIndicator, UWB_LINE_OF_SIGHT_INDICATOR> LineOfSightIndicatorMap{
        { UwbLineOfSightIndicator::LineOfSight, UWB_LINE_OF_SIGHT_INDICATOR_LINE_OF_SIGHT },
        { UwbLineOfSightIndicator::NonLineOfSight, UWB_LINE_OF_SIGHT_INDICATOR_NON_LINE_OF_SIGHT },
        { UwbLineOfSightIndicator::Indeterminant, UWB_LINE_OF_SIGHT_INDICATOR_UNABLE_TO_DETERMINE },
    };

    return LineOfSightIndicatorMap.at(uwbLineOfSightIndicator);
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
    auto sessionUpdateControllerMulticastListWrapper = UwbSessionUpdateMulicastListWrapper::from_num_elements<decltype(UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST::controleeList)>(std::size(uwbSessionUpdateMulicastList.Controlees));
    UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST &sessionUpdateControllerMulticastList = sessionUpdateControllerMulticastListWrapper;
    sessionUpdateControllerMulticastList.size = sessionUpdateControllerMulticastListWrapper.size();
    sessionUpdateControllerMulticastList.sessionId = uwbSessionUpdateMulicastList.SessionId;
    sessionUpdateControllerMulticastList.action = From(uwbSessionUpdateMulicastList.Action);
    sessionUpdateControllerMulticastList.numberOfControlees = std::size(uwbSessionUpdateMulicastList.Controlees);

    for (auto i = 0; i < sessionUpdateControllerMulticastList.numberOfControlees; i++) {
        auto &controlee = sessionUpdateControllerMulticastList.controleeList[i];
        controlee = From(uwbSessionUpdateMulicastList.Controlees[i]);
    }

    return sessionUpdateControllerMulticastListWrapper;
}

UwbSessionUpdateMulticastListStatusWrapper
windows::devices::uwb::ddi::lrp::From(const UwbSessionUpdateMulticastListStatus &uwbSessionUpdateMulticastListStatus)
{
    auto multicastListStatusWrapper = UwbSessionUpdateMulticastListStatusWrapper::from_num_elements<decltype(UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF::statusList)>(std::size(uwbSessionUpdateMulticastListStatus.Status));
    UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF &multicastListStatus = multicastListStatusWrapper;
    multicastListStatus.size = multicastListStatusWrapper.size();
    multicastListStatus.sessionId = uwbSessionUpdateMulticastListStatus.SessionId;
    multicastListStatus.numberOfControlees = std::size(uwbSessionUpdateMulticastListStatus.Status);
    multicastListStatus.remainingMulticastListSize = 0;

    for (auto i = 0; i < multicastListStatus.numberOfControlees; i++) {
        auto &status = multicastListStatus.statusList[i];
        status = From(uwbSessionUpdateMulticastListStatus.Status[i]);
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
        { UwbApplicationConfigurationParameterType::DestinationMacAddresses, UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS },
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
    std::span<const uint8_t> vendorSpecificInfo;

    if (uwbDeviceInfo.VendorSpecificInfo != nullptr) {
        vendorSpecificInfo = uwbDeviceInfo.VendorSpecificInfo->GetData();
        numElements = std::size(vendorSpecificInfo);
    } else {
        numElements = 1;
    }

    auto deviceInfoWrapper = UwbDeviceInformationWrapper::from_num_elements<decltype(UWB_DEVICE_INFO::vendorSpecificInfo)>(numElements);
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
    auto deviceCapabilitiesWrapper = UwbDeviceCapabilitiesWrapper::from_num_elements<decltype(UWB_DEVICE_CAPABILITIES::capabilityParams)>(numElements);

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

UWB_MAC_ADDRESS_MODE_INDICATOR
windows::devices::uwb::ddi::lrp::From(const ::uwb::UwbMacAddressType &uwbMacAddressModeIndicator)
{
    static const std::unordered_map<::uwb::UwbMacAddressType, UWB_MAC_ADDRESS_MODE_INDICATOR> MacAddressModeIndicatorMap{
        { ::uwb::UwbMacAddressType::Short, UWB_MAC_ADDRESS_MODE_INDICATOR_SHORT },
        { ::uwb::UwbMacAddressType::Extended, UWB_MAC_ADDRESS_MODE_INDICATOR_EXTENDED },
    };

    return MacAddressModeIndicatorMap.at(uwbMacAddressModeIndicator);
}

UWB_MAC_ADDRESS
windows::devices::uwb::ddi::lrp::From(const ::uwb::UwbMacAddress &uwbMacAddress)
{
    UWB_MAC_ADDRESS macAddress{
        .size = sizeof(macAddress),
        .addressType = From(uwbMacAddress.GetType())
    };

    uint8_t *value = nullptr;

    switch (macAddress.addressType) {
    case UWB_MAC_ADDRESS_MODE_INDICATOR_SHORT:
        value = &macAddress.addressShort[0];
        break;
    case UWB_MAC_ADDRESS_MODE_INDICATOR_EXTENDED:
        value = &macAddress.addressExtended[0];
        break;
    default:
        throw std::runtime_error("unknown mac address type");
    }

    auto valueUwb = uwbMacAddress.GetValue();
    std::memcpy(value, std::data(valueUwb), std::size(valueUwb));

    return macAddress;
}

UWB_RANGING_MEASUREMENT
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbRangingMeasurement &uwbRangingMeasurement)
{
    // clang-format off
    UWB_RANGING_MEASUREMENT rangingMeasurement{
        .size = sizeof rangingMeasurement,
        .macAddrPeer = From(uwbRangingMeasurement.PeerMacAddress),
        .status = From(uwbRangingMeasurement.Status),
        .lineOfSightIndicator = From(uwbRangingMeasurement.LineOfSightIndicator),
        .distance = uwbRangingMeasurement.Distance,
        .aoaAzimuth = uwbRangingMeasurement.AoAAzimuth.Result,
        .aoaAzimuthFigureOfMerit = uwbRangingMeasurement.AoAAzimuth.FigureOfMerit.value_or(0),
        .aoaElevation = uwbRangingMeasurement.AoAElevation.Result,
        .aoaElevationFigureOfMerit = uwbRangingMeasurement.AoAElevation.FigureOfMerit.value_or(0),
        .aoaDestinationAzimuth = uwbRangingMeasurement.AoaDestinationAzimuth.Result,
        .aoaDestinationAzimuthFigureOfMerit = uwbRangingMeasurement.AoaDestinationAzimuth.FigureOfMerit.value_or(0),
        .aoaDestinationElevation = uwbRangingMeasurement.AoaDestinationElevation.Result,
        .aoaDestinationElevationFigureOfMerit = uwbRangingMeasurement.AoaDestinationElevation.FigureOfMerit.value_or(0),
        .slotIndex = uwbRangingMeasurement.SlotIndex
    };
    // clang-format on

    return rangingMeasurement;
}

UwbRangingDataWrapper
windows::devices::uwb::ddi::lrp::From(const UwbRangingData &uwbRangingData)
{
    auto rangingDataWrapper = UwbRangingDataWrapper::from_num_elements<decltype(UWB_RANGING_DATA::rangingMeasurements)>(std::size(uwbRangingData.RangingMeasurements));
    UWB_RANGING_DATA &rangingData = rangingDataWrapper;
    rangingData.size = rangingDataWrapper.size();
    rangingData.sequenceNumber = uwbRangingData.SequenceNumber;
    rangingData.sessionId = uwbRangingData.SessionId;
    rangingData.currentRangingInterval = uwbRangingData.CurrentRangingInterval;
    rangingData.rangingMeasurementType = From(uwbRangingData.RangingMeasurementType);
    rangingData.numberOfRangingMeasurements = std::size(uwbRangingData.RangingMeasurements);

    for (std::size_t i = 0; i < rangingData.numberOfRangingMeasurements; i++) {
        auto &rangingMeasurement = rangingData.rangingMeasurements[i];
        rangingMeasurement = From(uwbRangingData.RangingMeasurements[i]);
    }

    return rangingDataWrapper;
}

UwbNotificationDataWrapper
windows::devices::uwb::ddi::lrp::From(const UwbNotificationData &uwbNotificationData)
{
    static const std::unordered_map<std::type_index, UWB_NOTIFICATION_TYPE> NotificationTypeMap{
        { std::type_index(typeid(UwbStatus)), UWB_NOTIFICATION_TYPE_GENERIC_ERROR },
        { std::type_index(typeid(UwbStatusDevice)), UWB_NOTIFICATION_TYPE_DEVICE_STATUS },
        { std::type_index(typeid(UwbSessionStatus)), UWB_NOTIFICATION_TYPE_SESSION_STATUS },
        { std::type_index(typeid(UwbSessionUpdateMulticastListStatus)), UWB_NOTIFICATION_TYPE_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST },
        { std::type_index(typeid(UwbRangingData)), UWB_NOTIFICATION_TYPE_RANGING_DATA },
    };

    // The total size required must be calculated, which includes the size of
    // all the non-discriminated structure members plus the size of the
    // discriminated type within the union. Taking the sum of each individual
    // member is error-prone because 1) if the structure is ever updated, this
    // code will break, and 2) such a calculation would not include compiler
    // added padding for alignment. Conseauently, we take the size of the entire
    // structure here, which will include the size of the largest union member,
    // resulting in an overestimate. While this wastes some memory, it provides
    // a strong guarantee that enough memory will be allocated, avoiding the
    // possibility of buffer overrun and heap corruption.
    std::size_t totalSize = sizeof(UWB_NOTIFICATION_DATA);
    std::unique_ptr<UwbNotificationDataWrapper> notificationDataWrapper;

    std::visit([&](auto &&arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            totalSize += sizeof(UWB_NOTIFICATION_DATA::genericError);
            notificationDataWrapper = std::make_unique<UwbNotificationDataWrapper>(totalSize);
            UWB_NOTIFICATION_DATA &notificationData = notificationDataWrapper->value();
            notificationData.notificationType = NotificationTypeMap.at(typeid(arg));
            notificationData.genericError = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            totalSize += sizeof(UWB_NOTIFICATION_DATA::deviceStatus);
            notificationDataWrapper = std::make_unique<UwbNotificationDataWrapper>(totalSize);
            UWB_NOTIFICATION_DATA &notificationData = notificationDataWrapper->value();
            notificationData.notificationType = NotificationTypeMap.at(typeid(arg));
            notificationData.deviceStatus = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            totalSize += sizeof(UWB_NOTIFICATION_DATA::sessionStatus);
            notificationDataWrapper = std::make_unique<UwbNotificationDataWrapper>(totalSize);
            UWB_NOTIFICATION_DATA &notificationData = notificationDataWrapper->value();
            notificationData.notificationType = NotificationTypeMap.at(typeid(arg));
            notificationData.sessionStatus = From(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionUpdateMulticastListStatus>) {
            auto uwbSessionUpdateMulticastListStatusWrapper = From(arg);
            totalSize += std::size(uwbSessionUpdateMulticastListStatusWrapper);
            notificationDataWrapper = std::make_unique<UwbNotificationDataWrapper>(totalSize);
            UWB_NOTIFICATION_DATA &notificationData = notificationDataWrapper->value();
            notificationData.notificationType = NotificationTypeMap.at(typeid(arg));
            auto data = uwbSessionUpdateMulticastListStatusWrapper.data();
            std::memcpy(&notificationData.sessionUpdateControllerMulticastList, std::data(data), std::size(data));
        } else if constexpr (std::is_same_v<ValueType, UwbRangingData>) {
            auto uwbRangingDataWrapper = From(arg);
            totalSize += std::size(uwbRangingDataWrapper);
            notificationDataWrapper = std::make_unique<UwbNotificationDataWrapper>(totalSize);
            UWB_NOTIFICATION_DATA &notificationData = notificationDataWrapper->value();
            notificationData.notificationType = NotificationTypeMap.at(typeid(arg));
            auto data = uwbRangingDataWrapper.data();
            std::memcpy(&notificationData.rangingData, std::data(data), std::size(data));
        }
        // Note: no else clause is needed here since if the type is not
        // supported, the at() call above will throw std::out_of_range, ensuring
        // this code will never be reached.
    },
        uwbNotificationData);

    notificationDataWrapper->value().size = totalSize;
    return std::move(*notificationDataWrapper);
}

UwbGetApplicationConfigurationParametersWrapper
windows::devices::uwb::ddi::lrp::From(const UwbGetApplicationConfigurationParameters &uwbGetApplicationConfigurationParameters)
{
    const std::size_t totalSize = offsetof(UWB_GET_APP_CONFIG_PARAMS, appConfigParams[std::size(uwbGetApplicationConfigurationParameters.ParameterTypes)]);
    UwbGetApplicationConfigurationParametersWrapper getApplicationConfigurationParameterTypesWrapper(totalSize);
    auto &getApplicationConfigurationParameterTypes = getApplicationConfigurationParameterTypesWrapper.value();
    getApplicationConfigurationParameterTypes.size = totalSize;
    getApplicationConfigurationParameterTypes.sessionId = uwbGetApplicationConfigurationParameters.SessionId;
    getApplicationConfigurationParameterTypes.appConfigParamsCount = std::size(uwbGetApplicationConfigurationParameters.ParameterTypes);

    for (auto i = 0; i < getApplicationConfigurationParameterTypes.appConfigParamsCount; i++) {
        getApplicationConfigurationParameterTypes.appConfigParams[i] = From(uwbGetApplicationConfigurationParameters.ParameterTypes[i]);
    }

    return std::move(getApplicationConfigurationParameterTypesWrapper);
}

UWB_APP_CONFIG_PARAM_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbSetApplicationConfigurationParameterStatus &uwbSetApplicationConfigurationParameterStatus)
{
    UWB_APP_CONFIG_PARAM_STATUS applicationConfigurationParameterStatus = {
        .size = sizeof applicationConfigurationParameterStatus,
        .paramType = From(uwbSetApplicationConfigurationParameterStatus.ParameterType),
        .status = From(uwbSetApplicationConfigurationParameterStatus.Status),
    };

    return applicationConfigurationParameterStatus;
}

UwbSetApplicationConfigurationParametersWrapper
windows::devices::uwb::ddi::lrp::From(const UwbSetApplicationConfigurationParameters &uwbSetApplicationConfigurationParameters)
{
    // Convert each individual parameter to its DDI wrapper.
    std::vector<UwbApplicationConfigurationParameterWrapper> uwbApplicationConfigurationParameterWrappers{};
    std::ranges::transform(uwbSetApplicationConfigurationParameters.Parameters, std::back_inserter(uwbApplicationConfigurationParameterWrappers), [](const auto &uwbApplicationConfigurationParameter) {
        return From(uwbApplicationConfigurationParameter);
    });

    // Calculate the total size required for the UWB_APP_CONFIG_PARAMS instance.
    const std::size_t totalSize = std::accumulate(
        std::cbegin(uwbApplicationConfigurationParameterWrappers),
        std::cend(uwbApplicationConfigurationParameterWrappers),
        static_cast<std::size_t>(offsetof(UWB_SET_APP_CONFIG_PARAMS, appConfigParams[0])),
        [&](std::size_t totalSize, const auto &uwbApplicationConfigurationParameterWrapper) {
            return totalSize + std::size(uwbApplicationConfigurationParameterWrapper);
        });

    // Instantiate a wrapper for UWB_SET_APP_CONFIG_PARAMS and fill it in.
    UwbSetApplicationConfigurationParametersWrapper setApplicationConfigurationParametersWrapper{ totalSize };
    UWB_SET_APP_CONFIG_PARAMS &setAppConfigParams = setApplicationConfigurationParametersWrapper.value();
    setAppConfigParams.size = totalSize;
    setAppConfigParams.sessionId = uwbSetApplicationConfigurationParameters.SessionId;
    setAppConfigParams.appConfigParamsCount = std::size(uwbApplicationConfigurationParameterWrappers);

    // Copy each of the converted UWB_APP_CONFIG_PARAM values into the UWB_SET_APP_CONFIG_PARAMS flex-array.
    UWB_APP_CONFIG_PARAM *appConfigParam = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(&setAppConfigParams.appConfigParams[0]);
    for (auto &uwbApplicationConfigurationParameterWrapper : uwbApplicationConfigurationParameterWrappers) {
        UWB_APP_CONFIG_PARAM &uwbAppConfigParam = uwbApplicationConfigurationParameterWrapper.value();
        std::memcpy(appConfigParam, &uwbAppConfigParam, uwbAppConfigParam.size);
        appConfigParam = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(reinterpret_cast<uintptr_t>(appConfigParam) + appConfigParam->size);
    }

    return std::move(setApplicationConfigurationParametersWrapper);
}

UwbSetApplicationConfigurationParametersStatusWrapper
windows::devices::uwb::ddi::lrp::From(const UwbSetApplicationConfigurationParametersStatus &uwbSetApplicationConfigurationParametersStatus)
{
    auto uwbSetApplicationConfigurationParametersStatusWrapper = UwbSetApplicationConfigurationParametersStatusWrapper::from_num_elements<UWB_APP_CONFIG_PARAM_STATUS>(std::size(uwbSetApplicationConfigurationParametersStatus.ParameterStatuses));
    auto &setApplicationConfigurationParametersStatus = uwbSetApplicationConfigurationParametersStatusWrapper.value();
    setApplicationConfigurationParametersStatus.size = std::size(uwbSetApplicationConfigurationParametersStatusWrapper);
    setApplicationConfigurationParametersStatus.status = From(uwbSetApplicationConfigurationParametersStatus.Status);
    setApplicationConfigurationParametersStatus.appConfigParamsCount = std::size(uwbSetApplicationConfigurationParametersStatus.ParameterStatuses);

    auto parameterStatuses = std::span(setApplicationConfigurationParametersStatus.appConfigParamsStatus, setApplicationConfigurationParametersStatus.appConfigParamsCount);
    std::ranges::transform(uwbSetApplicationConfigurationParametersStatus.ParameterStatuses, std::ranges::begin(parameterStatuses), [&](const auto &parameterStatus) {
        return From(parameterStatus);
    });

    return std::move(uwbSetApplicationConfigurationParametersStatusWrapper);
}

UwbApplicationConfigurationParameterWrapper
windows::devices::uwb::ddi::lrp::From(const UwbApplicationConfigurationParameter &uwbApplicationConfigurationParameter)
{
    std::size_t parameterLength = 0;
    std::size_t totalSize = offsetof(UWB_APP_CONFIG_PARAM, paramValue[0]);
    std::unique_ptr<UwbApplicationConfigurationParameterWrapper> applicationConfigurationParameterWrapper;

    std::visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_enum_v<T>) {
            const auto value = notstd::to_underlying(arg);
            parameterLength = sizeof(value);
            totalSize += parameterLength;
            applicationConfigurationParameterWrapper = std::make_unique<UwbApplicationConfigurationParameterWrapper>(totalSize);
            UWB_APP_CONFIG_PARAM &applicationConfigurationParameter = applicationConfigurationParameterWrapper->value();
            std::memcpy(&applicationConfigurationParameter.paramValue[0], &value, parameterLength);
        } else if constexpr (std::is_integral_v<T> || std::is_same_v<T, std::array<uint8_t, StaticStsInitializationVectorLength>>) {
            parameterLength = sizeof arg;
            totalSize += parameterLength;
            applicationConfigurationParameterWrapper = std::make_unique<UwbApplicationConfigurationParameterWrapper>(totalSize);
            UWB_APP_CONFIG_PARAM &applicationConfigurationParameter = applicationConfigurationParameterWrapper->value();
            std::memcpy(&applicationConfigurationParameter.paramValue[0], &arg, parameterLength);
        } else if constexpr (std::is_same_v<T, ::uwb::UwbMacAddress>) {
            const auto value = arg.GetValue();
            parameterLength = std::size(value);
            totalSize += parameterLength;
            applicationConfigurationParameterWrapper = std::make_unique<UwbApplicationConfigurationParameterWrapper>(totalSize);
            UWB_APP_CONFIG_PARAM &applicationConfigurationParameter = applicationConfigurationParameterWrapper->value();
            std::memcpy(&applicationConfigurationParameter.paramValue[0], std::data(value), parameterLength);
        } else if constexpr (std::is_same_v<T, std::unordered_set<ResultReportConfiguration>>) {
            uint8_t value = 0;
            parameterLength = sizeof value;
            totalSize += parameterLength;
            for (const auto &resultReportConfiguration : arg) {
                value |= notstd::to_underlying(resultReportConfiguration);
            }
            applicationConfigurationParameterWrapper = std::make_unique<UwbApplicationConfigurationParameterWrapper>(totalSize);
            UWB_APP_CONFIG_PARAM &applicationConfigurationParameter = applicationConfigurationParameterWrapper->value();
            applicationConfigurationParameter.paramValue[0] = value;
        } else if constexpr (std::is_same_v<T, std::unordered_set<::uwb::UwbMacAddress>>) {
            std::vector<uint8_t> valueBuffer{};
            for (const auto &uwbMacAddress : arg) {
                const auto value = uwbMacAddress.GetValue();
                parameterLength += std::size(value);
                valueBuffer.insert(std::end(valueBuffer), std::cbegin(value), std::cend(value));
            }
            totalSize += parameterLength;
            applicationConfigurationParameterWrapper = std::make_unique<UwbApplicationConfigurationParameterWrapper>(totalSize);
            UWB_APP_CONFIG_PARAM &applicationConfigurationParameter = applicationConfigurationParameterWrapper->value();
            std::memcpy(&applicationConfigurationParameter.paramValue[0], std::data(valueBuffer), parameterLength);
        } else {
            throw std::runtime_error("unknown UwbApplicationConfigurationParameter variant value encountered");
        }
    },
        uwbApplicationConfigurationParameter.Value);

    UWB_APP_CONFIG_PARAM &applicationConfigurationParameter = applicationConfigurationParameterWrapper->value();
    applicationConfigurationParameter.size = totalSize;
    applicationConfigurationParameter.paramType = From(uwbApplicationConfigurationParameter.Type);
    applicationConfigurationParameter.paramLength = parameterLength;

    return std::move(*applicationConfigurationParameterWrapper);
}

namespace detail
{
/**
 * @brief Process an array of UWB_APP_CONFIG_PARAM objects from a
 * UWB_APP_CONFIG_PARAMS object.
 *
 * Note the this function does not trap any exceptions generated by the
 * callback, so the caller is responsible to handle them.
 *
 * @tparam Func The type of the callback function to invoke.
 * @param appConfigParams The container structure holding the array.
 * @param callback The callback function to invoke.
 */
template <typename Func>
void
ForEachApplicationConfigurationParameterImpl(UWB_APP_CONFIG_PARAMS &appConfigParams, Func &&callback)
{
    auto *appConfigParam = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(&appConfigParams.appConfigParams[0]);
    for (auto i = 0; i < appConfigParams.appConfigParamsCount; i++) {
        callback(*appConfigParam);
        appConfigParam = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(reinterpret_cast<uintptr_t>(appConfigParam) + appConfigParam->size);
    }
}

void
ForEachApplicationConfigurationParameter(const UWB_APP_CONFIG_PARAMS &appConfigParams, std::function<void(const UWB_APP_CONFIG_PARAM &)> callback)
{
    ForEachApplicationConfigurationParameterImpl(const_cast<UWB_APP_CONFIG_PARAMS &>(appConfigParams), callback);
}

void
ForEachApplicationConfigurationParameter(UWB_APP_CONFIG_PARAMS &appConfigParams, std::function<void(UWB_APP_CONFIG_PARAM &)> callback)
{
    ForEachApplicationConfigurationParameterImpl(appConfigParams, callback);
}
} // namespace detail

UwbApplicationConfigurationParametersWrapper
windows::devices::uwb::ddi::lrp::From(const std::vector<UwbApplicationConfigurationParameter> &uwbApplicationConfigurationParameters)
{
    // Convert each individual parameter to its DDI wrapper.
    std::vector<UwbApplicationConfigurationParameterWrapper> uwbApplicationConfigurationParameterWrappers{};
    std::ranges::transform(uwbApplicationConfigurationParameters, std::back_inserter(uwbApplicationConfigurationParameterWrappers), [](const auto &uwbApplicationConfigurationParameter) {
        return From(uwbApplicationConfigurationParameter);
    });

    // Calculate the total size required for the UWB_APP_CONFIG_PARAMS instance.
    const std::size_t totalSize = std::accumulate(
        std::cbegin(uwbApplicationConfigurationParameterWrappers),
        std::cend(uwbApplicationConfigurationParameterWrappers),
        static_cast<std::size_t>(offsetof(UWB_APP_CONFIG_PARAMS, appConfigParams[0])),
        [&](std::size_t totalSize, const auto &uwbApplicationConfigurationParameterWrapper) {
            return totalSize + std::size(uwbApplicationConfigurationParameterWrapper);
        });

    // Instantiate a wrapper for UWB_APP_CONFIG_PARAMS and fill it in.
    UwbApplicationConfigurationParametersWrapper applicationConfigurationParametersWrapper{ totalSize };
    UWB_APP_CONFIG_PARAMS &appConfigParams = applicationConfigurationParametersWrapper.value();
    appConfigParams.size = totalSize;
    appConfigParams.status = UWB_STATUS_OK;
    appConfigParams.appConfigParamsCount = std::size(uwbApplicationConfigurationParameterWrappers);

    // Copy each of the converted UWB_APP_CONFIG_PARAM values into the UWB_APP_CONFIG_PARAMS flex-array.
    UWB_APP_CONFIG_PARAM *appConfigParam = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(&appConfigParams.appConfigParams[0]);
    for (auto &uwbApplicationConfigurationParameterWrapper : uwbApplicationConfigurationParameterWrappers) {
        UWB_APP_CONFIG_PARAM &uwbAppConfigParam = uwbApplicationConfigurationParameterWrapper.value();
        std::memcpy(appConfigParam, &uwbAppConfigParam, uwbAppConfigParam.size);
        appConfigParam = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(reinterpret_cast<uintptr_t>(appConfigParam) + uwbAppConfigParam.size);
    }

    return std::move(applicationConfigurationParametersWrapper);
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

UwbStatus
windows::devices::uwb::ddi::lrp::To(const UWB_STATUS &status)
{
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

UwbSessionType
windows::devices::uwb::ddi::lrp::To(const UWB_SESSION_TYPE &sessionType)
{
    static const std::unordered_map<UWB_SESSION_TYPE, UwbSessionType> SessionTypeMap{
        { UWB_SESSION_TYPE_RANGING_SESSION, UwbSessionType::RangingSession },
        { UWB_SESSION_TYPE_DEVICE_TEST_MODE, UwbSessionType::TestMode },
    };

    return SessionTypeMap.at(sessionType);
}

UwbDeviceConfigurationParameterType
windows::devices::uwb::ddi::lrp::To(const UWB_DEVICE_CONFIG_PARAM_TYPE &deviceConfigurationParameterType)
{
    static const std::unordered_map<UWB_DEVICE_CONFIG_PARAM_TYPE, UwbDeviceConfigurationParameterType> ConfigParamMap{
        { UWB_DEVICE_CONFIG_PARAM_TYPE_DEVICE_STATE, UwbDeviceConfigurationParameterType::DeviceState },
        { UWB_DEVICE_CONFIG_PARAM_TYPE_LOW_POWER_MODE, UwbDeviceConfigurationParameterType::LowPowerMode },
    };

    return ConfigParamMap.at(deviceConfigurationParameterType);
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

UwbLineOfSightIndicator
windows::devices::uwb::ddi::lrp::To(const UWB_LINE_OF_SIGHT_INDICATOR &lineOfSightIndicator)
{
    static const std::unordered_map<UWB_LINE_OF_SIGHT_INDICATOR, UwbLineOfSightIndicator> LineOfSightIndicatorMap{
        { UWB_LINE_OF_SIGHT_INDICATOR_LINE_OF_SIGHT, UwbLineOfSightIndicator::LineOfSight },
        { UWB_LINE_OF_SIGHT_INDICATOR_NON_LINE_OF_SIGHT, UwbLineOfSightIndicator::NonLineOfSight },
        { UWB_LINE_OF_SIGHT_INDICATOR_UNABLE_TO_DETERMINE, UwbLineOfSightIndicator::Indeterminant },
    };

    return LineOfSightIndicatorMap.at(lineOfSightIndicator);
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

UwbStatusMulticast
windows::devices::uwb::ddi::lrp::To(const UWB_MULTICAST_STATUS &statusMulticast)
{
    static const std::unordered_map<UWB_MULTICAST_STATUS, UwbStatusMulticast> StatusMap{
        { UWB_MULTICAST_STATUS_OK_MULTICAST_LIST_UPDATE, UwbStatusMulticast::OkUpdate },
        { UWB_MULTICAST_STATUS_ERROR_MULTICAST_LIST_FULL, UwbStatusMulticast::ErrorListFull },
        { UWB_MULTICAST_STATUS_ERROR_KEY_FETCH_FAIL, UwbStatusMulticast::ErrorKeyFetchFail },
        { UWB_MULTICAST_STATUS_ERROR_SUB_SESSION_ID_NOT_FOUND, UwbStatusMulticast::ErrorSubSessionIdNotFound },
    };

    return StatusMap.at(statusMulticast);
}

UwbMulticastListStatus
windows::devices::uwb::ddi::lrp::To(const UWB_MULTICAST_LIST_STATUS &multicastListStatus)
{
    ::uwb::UwbMacAddress controleeMacAddress{ std::array<uint8_t, ::uwb::UwbMacAddressLength::Short>{
        (multicastListStatus.controleeMacAddress & 0x00FFU) >> 0U,
        (multicastListStatus.controleeMacAddress & 0xFF00U) >> 8U,
    } };

    UwbMulticastListStatus uwbMulticastListStatus{
        .ControleeMacAddress = std::move(controleeMacAddress),
        .SubSessionId = multicastListStatus.subSessionId,
        .Status = To(multicastListStatus.status)
    };

    return uwbMulticastListStatus;
}

UwbSessionUpdateMulticastListEntry
windows::devices::uwb::ddi::lrp::To(const UWB_MULTICAST_CONTROLEE_LIST_ENTRY &sessionUpdateMulticastListEntry)
{
    ::uwb::UwbMacAddress controleeMacAddress{ std::array<uint8_t, ::uwb::UwbMacAddressLength::Short>{
        (sessionUpdateMulticastListEntry.shortAddress & 0x00FFU) >> 0U,
        (sessionUpdateMulticastListEntry.shortAddress & 0xFF00U) >> 8U,
    } };

    UwbSessionUpdateMulticastListEntry uwbSessionUpdateMulticastListEntry{
        .ControleeMacAddress = std::move(controleeMacAddress),
        .SubSessionId = sessionUpdateMulticastListEntry.subSessionId
    };

    return uwbSessionUpdateMulticastListEntry;
}

UwbSessionUpdateMulicastList
windows::devices::uwb::ddi::lrp::To(const UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST &sessionUpdateMulicastList)
{
    std::vector<UwbSessionUpdateMulticastListEntry> controlees{};
    for (std::size_t i = 0; i < sessionUpdateMulicastList.numberOfControlees; i++) {
        const auto &multicastControleeListEntry = sessionUpdateMulicastList.controleeList[i];
        controlees.push_back(To(multicastControleeListEntry));
    }

    UwbSessionUpdateMulicastList uwbSessionUpdateMulicastList{
        .SessionId = sessionUpdateMulicastList.sessionId,
        .Action = To(sessionUpdateMulicastList.action),
        .Controlees = std::move(controlees)
    };

    return uwbSessionUpdateMulicastList;
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

UwbSessionUpdateMulticastListStatus
windows::devices::uwb::ddi::lrp::To(const UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF &sessionUpdateControllerMulticastListNtf)
{
    std::vector<UwbMulticastListStatus> status{};
    for (std::size_t i = 0; i < sessionUpdateControllerMulticastListNtf.numberOfControlees; i++) {
        const auto &multicastListStatus = sessionUpdateControllerMulticastListNtf.statusList[i];
        status.push_back(To(multicastListStatus));
    }

    UwbSessionUpdateMulticastListStatus uwbSessionUpdateMulticastListStatus{
        .SessionId = sessionUpdateControllerMulticastListNtf.sessionId,
        .Status = std::move(status)
    };

    return uwbSessionUpdateMulticastListStatus;
}

UwbRangingMeasurementType
windows::devices::uwb::ddi::lrp::To(const UWB_RANGING_MEASUREMENT_TYPE &rangingMeasurementType)
{
    static const std::unordered_map<UWB_RANGING_MEASUREMENT_TYPE, UwbRangingMeasurementType> RangingTypeMap{
        { UWB_RANGING_MEASUREMENT_TYPE_TWO_WAY, UwbRangingMeasurementType::TwoWay },
    };

    return RangingTypeMap.at(rangingMeasurementType);
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

UwbApplicationConfigurationParameterType
windows::devices::uwb::ddi::lrp::To(const UWB_APP_CONFIG_PARAM_TYPE &appConfigParameterType)
{
    static const std::unordered_map<UWB_APP_CONFIG_PARAM_TYPE, UwbApplicationConfigurationParameterType> AppConfigParamMap{
        { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE, UwbApplicationConfigurationParameterType::DeviceType },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE, UwbApplicationConfigurationParameterType::RangingRoundUsage },
        { UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG, UwbApplicationConfigurationParameterType::StsConfiguration },
        { UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE, UwbApplicationConfigurationParameterType::MultiNodeMode },
        { UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER, UwbApplicationConfigurationParameterType::ChannelNumber },
        { UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES, UwbApplicationConfigurationParameterType::NumberOfControlees },
        { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS, UwbApplicationConfigurationParameterType::DeviceMacAddress },
        { UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS, UwbApplicationConfigurationParameterType::DestinationMacAddresses },
        { UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION, UwbApplicationConfigurationParameterType::SlotDuration },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL, UwbApplicationConfigurationParameterType::RangingInterval },
        { UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX, UwbApplicationConfigurationParameterType::StsIndex },
        { UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE, UwbApplicationConfigurationParameterType::MacFcsType },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL, UwbApplicationConfigurationParameterType::RangingRoundControl },
        { UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ, UwbApplicationConfigurationParameterType::AoAResultRequest },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG, UwbApplicationConfigurationParameterType::RangeDataNotificationConfig },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR, UwbApplicationConfigurationParameterType::RangeDataNotificationProximityNear },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR, UwbApplicationConfigurationParameterType::RangeDataNotificationProximityFar },
        { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE, UwbApplicationConfigurationParameterType::DeviceRole },
        { UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG, UwbApplicationConfigurationParameterType::RFrameConfiguration },
        { UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX, UwbApplicationConfigurationParameterType::PreambleCodeIndex },
        { UWB_APP_CONFIG_PARAM_TYPE_SFD_ID, UwbApplicationConfigurationParameterType::SfdId },
        { UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE, UwbApplicationConfigurationParameterType::PsduDataRate },
        { UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION, UwbApplicationConfigurationParameterType::PreambleDuration },
        { UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT, UwbApplicationConfigurationParameterType::RangingTimeStruct },
        { UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR, UwbApplicationConfigurationParameterType::SlotsPerRangingRound },
        { UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER, UwbApplicationConfigurationParameterType::TxAdaptivePayloadPower },
        { UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX, UwbApplicationConfigurationParameterType::ResponderSlotIndex },
        { UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE, UwbApplicationConfigurationParameterType::PrfMode },
        { UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE, UwbApplicationConfigurationParameterType::ScheduledMode },
        { UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION, UwbApplicationConfigurationParameterType::KeyRotation },
        { UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE, UwbApplicationConfigurationParameterType::KeyRotationRate },
        { UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY, UwbApplicationConfigurationParameterType::SessionPriority },
        { UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE, UwbApplicationConfigurationParameterType::MacAddressMode },
        { UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID, UwbApplicationConfigurationParameterType::VendorId },
        { UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV, UwbApplicationConfigurationParameterType::StaticStsIv },
        { UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS, UwbApplicationConfigurationParameterType::NumberOfStsSegments },
        { UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY, UwbApplicationConfigurationParameterType::MaxRangingRoundRetry },
        { UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME, UwbApplicationConfigurationParameterType::UwbInitiationTime },
        { UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE, UwbApplicationConfigurationParameterType::HoppingMode },
        { UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH, UwbApplicationConfigurationParameterType::BlockStrideLength },
        { UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG, UwbApplicationConfigurationParameterType::ResultReportConfig },
        { UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT, UwbApplicationConfigurationParameterType::InBandTerminationAttemptCount },
        { UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID, UwbApplicationConfigurationParameterType::SubSessionId },
        { UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE, UwbApplicationConfigurationParameterType::BprfPhrDataRate },
        { UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS, UwbApplicationConfigurationParameterType::MaxNumberOfMeasurements },
        { UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH, UwbApplicationConfigurationParameterType::StsLength },
    };

    return AppConfigParamMap.at(appConfigParameterType);
}

::uwb::UwbMacAddressType
windows::devices::uwb::ddi::lrp::To(const UWB_MAC_ADDRESS_MODE_INDICATOR &macAddressModeIndicator)
{
    static const std::unordered_map<UWB_MAC_ADDRESS_MODE_INDICATOR, ::uwb::UwbMacAddressType> MacAddressModeIndicatorMap{
        { UWB_MAC_ADDRESS_MODE_INDICATOR_SHORT, ::uwb::UwbMacAddressType::Short },
        { UWB_MAC_ADDRESS_MODE_INDICATOR_EXTENDED, ::uwb::UwbMacAddressType::Extended },
    };

    return MacAddressModeIndicatorMap.at(macAddressModeIndicator);
}

::uwb::UwbMacAddress
windows::devices::uwb::ddi::lrp::To(const UWB_MAC_ADDRESS &macAddress)
{
    ::uwb::UwbMacAddress uwbMacAddress{};

    switch (macAddress.addressType) {
    case UWB_MAC_ADDRESS_MODE_INDICATOR_SHORT: {
        const auto data = std::to_array<const uint8_t, ::uwb::UwbMacAddressLength::Short>(macAddress.addressShort);
        uwbMacAddress = ::uwb::UwbMacAddress(data);
        break;
    }
    case UWB_MAC_ADDRESS_MODE_INDICATOR_EXTENDED: {
        const auto data = std::to_array<const uint8_t, ::uwb::UwbMacAddressLength::Extended>(macAddress.addressExtended);
        uwbMacAddress = ::uwb::UwbMacAddress(data);
        break;
    }
    default: {
        throw std::runtime_error("unknown mac address type");
    }
    }

    return uwbMacAddress;
}

UwbRangingMeasurement
windows::devices::uwb::ddi::lrp::To(const UWB_RANGING_MEASUREMENT &rangingMeasurement)
{
    UwbRangingMeasurement uwbRangingMeasurement{
        .SlotIndex = rangingMeasurement.slotIndex,
        .Distance = rangingMeasurement.distance,
        .Status = To(rangingMeasurement.status),
        .PeerMacAddress = To(rangingMeasurement.macAddrPeer),
        .LineOfSightIndicator = To(rangingMeasurement.lineOfSightIndicator),
        .AoAAzimuth = { .Result = std::bit_cast<uint16_t>(rangingMeasurement.aoaAzimuth) },
        .AoAElevation = { .Result = std::bit_cast<uint16_t>(rangingMeasurement.aoaElevation) },
        .AoaDestinationAzimuth = { .Result = std::bit_cast<uint16_t>(rangingMeasurement.aoaDestinationAzimuth) },
        .AoaDestinationElevation = { .Result = std::bit_cast<uint16_t>(rangingMeasurement.aoaDestinationElevation) },
    };

    if (rangingMeasurement.aoaAzimuthFigureOfMerit != 0) {
        uwbRangingMeasurement.AoAAzimuth.FigureOfMerit = rangingMeasurement.aoaAzimuthFigureOfMerit;
    }
    if (rangingMeasurement.aoaElevationFigureOfMerit != 0) {
        uwbRangingMeasurement.AoAElevation.FigureOfMerit = rangingMeasurement.aoaElevationFigureOfMerit;
    }
    if (rangingMeasurement.aoaDestinationAzimuthFigureOfMerit != 0) {
        uwbRangingMeasurement.AoaDestinationAzimuth.FigureOfMerit = rangingMeasurement.aoaDestinationAzimuthFigureOfMerit;
    }
    if (rangingMeasurement.aoaDestinationElevationFigureOfMerit != 0) {
        uwbRangingMeasurement.AoaDestinationElevation.FigureOfMerit = rangingMeasurement.aoaDestinationElevationFigureOfMerit;
    }

    return uwbRangingMeasurement;
}

UwbRangingData
windows::devices::uwb::ddi::lrp::To(const UWB_RANGING_DATA &rangingData)
{
    UwbRangingData uwbRangingData{
        .SequenceNumber = rangingData.sequenceNumber,
        .SessionId = rangingData.sessionId,
        .CurrentRangingInterval = rangingData.currentRangingInterval,
        .RangingMeasurementType = To(rangingData.rangingMeasurementType)
    };

    for (std::size_t i = 0; i < rangingData.numberOfRangingMeasurements; i++) {
        const auto &rangingMeasurement = rangingData.rangingMeasurements[i];
        uwbRangingData.RangingMeasurements.push_back(To(rangingMeasurement));
    }

    return uwbRangingData;
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

namespace detail
{
/**
 * @brief Untyped UwbDeviceInfoVendor implementation that stores the data in a vector.
 */
class UwbDeviceInfoVendorImpl :
    public UwbDeviceInfoVendor
{
public:
    UwbDeviceInfoVendorImpl(const uint8_t *data, std::size_t length) :
        m_data(data, data + length)
    {}

    std::span<const uint8_t>
    GetData() const noexcept override
    {
        return std::span<const uint8_t>(std::data(m_data), std::size(m_data));
    }

private:
    std::vector<uint8_t> m_data;
};
} // namespace detail

UwbDeviceInformation
windows::devices::uwb::ddi::lrp::To(const UWB_DEVICE_INFO &deviceInfo)
{
    UwbDeviceInformation uwbDeviceInformation{
        .VersionUci = UwbVersion::FromUci(deviceInfo.uciGenericVersionMajor, deviceInfo.uciGenericVersionMinorAndMaintenance),
        .VersionUciTest = UwbVersion::FromUci(deviceInfo.uciTestVersionMajor, deviceInfo.uciTestVersionMinorAndMaintenance),
        .VersionMac = UwbVersion::FromUci(deviceInfo.macVersionMajor, deviceInfo.macVersionMinorAndMaintenance),
        .VersionPhy = UwbVersion::FromUci(deviceInfo.phyVersionMajor, deviceInfo.phyVersionMinorAndMaintenance),
        .Status = To(deviceInfo.status),
    };

    if (deviceInfo.vendorSpecificInfoLength > 0) {
        uwbDeviceInformation.VendorSpecificInfo = std::make_shared<detail::UwbDeviceInfoVendorImpl>(&deviceInfo.vendorSpecificInfo[0], deviceInfo.vendorSpecificInfoLength);
    }

    return uwbDeviceInformation;
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
        return To(notificationData.sessionUpdateControllerMulticastList);
    }
    case UWB_NOTIFICATION_TYPE_RANGING_DATA: {
        return To(notificationData.rangingData);
    }
    }

    PLOG_WARNING << "unknown UwbNotificationData type encountered; returning default constructed instance";
    return UwbNotificationData{};
}

UwbGetApplicationConfigurationParameters
windows::devices::uwb::ddi::lrp::To(const UWB_GET_APP_CONFIG_PARAMS &getApplicationConfigurationParameters)
{
    UwbGetApplicationConfigurationParameters uwbGetApplicationConfigurationParameters{
        .SessionId = getApplicationConfigurationParameters.sessionId
    };
    uwbGetApplicationConfigurationParameters.ParameterTypes.reserve(getApplicationConfigurationParameters.appConfigParamsCount);

    std::span applicationConfigurationParameterTypes(getApplicationConfigurationParameters.appConfigParams, getApplicationConfigurationParameters.appConfigParamsCount);
    std::ranges::transform(applicationConfigurationParameterTypes, std::back_inserter(uwbGetApplicationConfigurationParameters.ParameterTypes), [](const auto &applicationConfigurationParameterType) {
        return To(applicationConfigurationParameterType);
    });

    return std::move(uwbGetApplicationConfigurationParameters);
}

UwbSetApplicationConfigurationParameterStatus
windows::devices::uwb::ddi::lrp::To(const UWB_APP_CONFIG_PARAM_STATUS &applicationConfigurationParameterStatus)
{
    UwbSetApplicationConfigurationParameterStatus uwbSetApplicationConfigurationParameterStatus{
        .Status = To(applicationConfigurationParameterStatus.status),
        .ParameterType = To(applicationConfigurationParameterStatus.paramType),
    };

    return uwbSetApplicationConfigurationParameterStatus;
}

UwbSetApplicationConfigurationParameters
windows::devices::uwb::ddi::lrp::To(const UWB_SET_APP_CONFIG_PARAMS &setApplicationConfigurationParameters)
{
    UwbSetApplicationConfigurationParameters uwbSetApplicationConfigurationParameters{
        .SessionId = setApplicationConfigurationParameters.sessionId
    };
    uwbSetApplicationConfigurationParameters.Parameters.reserve(setApplicationConfigurationParameters.appConfigParamsCount);

    std::optional<::uwb::UwbMacAddressType> macAddressMode;
    std::optional<UWB_APP_CONFIG_PARAM> destinationMacAddresses;
    auto *appConfigParam = reinterpret_cast<const UWB_APP_CONFIG_PARAM *>(&setApplicationConfigurationParameters.appConfigParams[0]);
    for (auto i = 0; i < setApplicationConfigurationParameters.appConfigParamsCount; i++) {
        // If parameter is MacAddressMode or DestinationMacAddresses, store value
        if (To(appConfigParam->paramType) == UwbApplicationConfigurationParameterType::MacAddressMode) {
            macAddressMode = std::get<::uwb::UwbMacAddressType>(To(*appConfigParam).Value);
        } else if (To(appConfigParam->paramType) == UwbApplicationConfigurationParameterType::DestinationMacAddresses) {
            destinationMacAddresses = *appConfigParam;
        } else {
            auto uwbAppConfigParam = To(*appConfigParam);
            uwbSetApplicationConfigurationParameters.Parameters.push_back(std::move(uwbAppConfigParam));
        }

        appConfigParam = reinterpret_cast<const UWB_APP_CONFIG_PARAM *>(reinterpret_cast<uintptr_t>(appConfigParam) + appConfigParam->size);
    }

    // Now that the other parameters have been converted, convert DestinationMacAddresses with special conversion function
    if (destinationMacAddresses.has_value()) {
        UwbApplicationConfigurationParameter uwbAppConfigParam;
        if (macAddressMode.has_value()) {
            uwbAppConfigParam = To(destinationMacAddresses.value(), macAddressMode.value());
        } else {
            uwbAppConfigParam = To(destinationMacAddresses.value(), ::uwb::UwbMacAddressType::Short);
        }
        uwbSetApplicationConfigurationParameters.Parameters.push_back(std::move(uwbAppConfigParam));
    }

    return std::move(uwbSetApplicationConfigurationParameters);
}

UwbSetApplicationConfigurationParametersStatus
windows::devices::uwb::ddi::lrp::To(const UWB_SET_APP_CONFIG_PARAMS_STATUS &setApplicationConfigurationParametersStatus)
{
    UwbSetApplicationConfigurationParametersStatus uwbSetApplicationConfigurationParameterResult{
        .Status = To(setApplicationConfigurationParametersStatus.status),
    };

    uwbSetApplicationConfigurationParameterResult.ParameterStatuses.reserve(setApplicationConfigurationParametersStatus.appConfigParamsCount);
    auto parameterStatusesRange = std::span(setApplicationConfigurationParametersStatus.appConfigParamsStatus, setApplicationConfigurationParametersStatus.appConfigParamsCount);
    std::ranges::transform(parameterStatusesRange, std::back_inserter(uwbSetApplicationConfigurationParameterResult.ParameterStatuses), [&](const auto &parameterStatus) {
        return To(parameterStatus);
    });

    return std::move(uwbSetApplicationConfigurationParameterResult);
}

namespace detail
{
/**
 * @brief Converts a UWB_APP_CONFIG_PARAM value containing data mapping to a
 * enum-class or integral neutral type.
 *
 * @tparam T The enum class type to convert to.
 * @param applicationConfigurationParameter The parameter to convert from.
 * @param uwbApplicationConfigurationParameter The wrapper to convert to.
 */
template <typename T>
// clang-format off
requires (std::is_enum_v<T> || std::is_integral_v<T>)
// clang-format on
void ConvertUwbApplicationConfigurationParameterTo(const UWB_APP_CONFIG_PARAM &applicationConfigurationParameter, UwbApplicationConfigurationParameter &uwbApplicationConfigurationParameter)
{
    T value{};

    if constexpr (std::is_enum_v<T>) {
        using U = std::underlying_type_t<decltype(value)>;
        const U *valueUnderlying = reinterpret_cast<const U *>(&applicationConfigurationParameter.paramValue[0]);
        value = static_cast<T>(*valueUnderlying);
    } else if constexpr (std::is_integral_v<T>) {
        const T *valuePointer = reinterpret_cast<const T *>(&applicationConfigurationParameter.paramValue[0]);
        std::memcpy(&value, valuePointer, sizeof value);
    }

    uwbApplicationConfigurationParameter.Value = std::move(value);
}
} // namespace detail

UwbApplicationConfigurationParameter
windows::devices::uwb::ddi::lrp::To(const UWB_APP_CONFIG_PARAM &applicationConfigurationParameter)
{
    UwbApplicationConfigurationParameter uwbApplicationConfigurationParameter{
        .Type = To(applicationConfigurationParameter.paramType)
    };

    switch (applicationConfigurationParameter.paramType) {
    case UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE:
        detail::ConvertUwbApplicationConfigurationParameterTo<bool>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    // uint8_t direct encodings
    case UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES:
    case UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX:
    case UWB_APP_CONFIG_PARAM_TYPE_SFD_ID:
    case UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR:
    case UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX:
    case UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE:
    case UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY:
    case UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS:
    case UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH:
    case UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT:
        detail::ConvertUwbApplicationConfigurationParameterTo<uint8_t>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    // uint16_t direct encodings
    case UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION:
    case UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR:
    case UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR:
    case UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID:
    case UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY:
    case UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS:
        detail::ConvertUwbApplicationConfigurationParameterTo<uint16_t>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    // uint32_t direct encodings
    case UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL:
    case UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX:
    case UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME:
    case UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID:
        detail::ConvertUwbApplicationConfigurationParameterTo<uint32_t>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ:
        detail::ConvertUwbApplicationConfigurationParameterTo<AoAResult>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE:
        detail::ConvertUwbApplicationConfigurationParameterTo<BprfPhrDataRate>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER:
        detail::ConvertUwbApplicationConfigurationParameterTo<Channel>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE:
        detail::ConvertUwbApplicationConfigurationParameterTo<DeviceRole>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE:
        detail::ConvertUwbApplicationConfigurationParameterTo<DeviceType>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION:
        detail::ConvertUwbApplicationConfigurationParameterTo<KeyRotation>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE:
        detail::ConvertUwbApplicationConfigurationParameterTo<MultiNodeMode>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION:
        detail::ConvertUwbApplicationConfigurationParameterTo<PreambleDuration>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE:
        detail::ConvertUwbApplicationConfigurationParameterTo<PrfModeDetailed>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE:
        detail::ConvertUwbApplicationConfigurationParameterTo<PsduDataRate>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG:
        detail::ConvertUwbApplicationConfigurationParameterTo<RangeDataNotificationConfiguration>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE:
        detail::ConvertUwbApplicationConfigurationParameterTo<RangingRoundUsage>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT:
        detail::ConvertUwbApplicationConfigurationParameterTo<RangingMode>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL:
        detail::ConvertUwbApplicationConfigurationParameterTo<RangingRoundControl>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG: {
        std::unordered_set<ResultReportConfiguration> configs;
        const uint8_t value = applicationConfigurationParameter.paramValue[0];
        for (const auto bitmap : magic_enum::enum_values<ResultReportConfiguration>()) {
            const auto underlyingMap = notstd::to_underlying(bitmap);
            if (value & underlyingMap) {
                configs.insert(bitmap);
            }
        }
        uwbApplicationConfigurationParameter.Value = std::move(configs);
        break;
    }
    case UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE:
        detail::ConvertUwbApplicationConfigurationParameterTo<SchedulingMode>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG:
        detail::ConvertUwbApplicationConfigurationParameterTo<StsConfiguration>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH:
        detail::ConvertUwbApplicationConfigurationParameterTo<StsLength>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG:
        detail::ConvertUwbApplicationConfigurationParameterTo<StsPacketConfiguration>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER:
        detail::ConvertUwbApplicationConfigurationParameterTo<TxAdaptivePayloadPower>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS: {
        ::uwb::UwbMacAddress value{};
        switch (applicationConfigurationParameter.paramLength) {
        case ::uwb::UwbMacAddressLength::Short: {
            ::uwb::UwbMacAddress::ShortType data{};
            std::memcpy(std::data(data), &applicationConfigurationParameter.paramValue[0], std::size(data));
            value = ::uwb::UwbMacAddress(std::move(data));
            uwbApplicationConfigurationParameter.Value = std::move(value);
            break;
        }
        case ::uwb::UwbMacAddressLength::Extended: {
            ::uwb::UwbMacAddress::ExtendedType data{};
            std::memcpy(std::data(data), &applicationConfigurationParameter.paramValue[0], std::size(data));
            value = ::uwb::UwbMacAddress(std::move(data));
            uwbApplicationConfigurationParameter.Value = std::move(value);
            break;
        }
        default:
            LOG_WARNING << "unsupported mac address length (" << applicationConfigurationParameter.paramLength << ") encountered";
            break;
        }
        break;
    }
    case UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE:
        detail::ConvertUwbApplicationConfigurationParameterTo<::uwb::UwbMacAddressFcsType>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE:
        detail::ConvertUwbApplicationConfigurationParameterTo<::uwb::UwbMacAddressType>(applicationConfigurationParameter, uwbApplicationConfigurationParameter);
        break;
    case UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV: {
        std::array<uint8_t, StaticStsInitializationVectorLength> value{};
        std::memcpy(std::data(value), &applicationConfigurationParameter.paramValue[0], sizeof value);
        uwbApplicationConfigurationParameter.Value = std::move(value);
        break;
    }
    case UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS: {
        PLOG_ERROR << "unexpected uwb application configuration parameter type encountered for this conversion function, type=" << std::showbase << std::hex << notstd::to_underlying(applicationConfigurationParameter.paramType);
        break;
    }
    default:
        PLOG_ERROR << "unknown uwb application configuration parameter type encountered, type=" << std::showbase << std::hex << notstd::to_underlying(applicationConfigurationParameter.paramType);
        break;
    }

    return uwbApplicationConfigurationParameter;
}

UwbApplicationConfigurationParameter
windows::devices::uwb::ddi::lrp::To(const UWB_APP_CONFIG_PARAM &applicationConfigurationParameter, const ::uwb::UwbMacAddressType macAddressMode)
{
    UwbApplicationConfigurationParameter uwbApplicationConfigurationParameter{
        .Type = To(applicationConfigurationParameter.paramType)
    };

    if (applicationConfigurationParameter.paramType == UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS) {
        std::unordered_set<::uwb::UwbMacAddress> value{};

        if (macAddressMode == ::uwb::UwbMacAddressType::Short) {
            for (auto i = 0; i < (applicationConfigurationParameter.paramLength / ::uwb::UwbMacAddress::ShortLength); i++) {
                ::uwb::UwbMacAddress::ShortType data{};
                auto offset = i * ::uwb::UwbMacAddress::ShortLength;
                std::memcpy(std::data(data), &applicationConfigurationParameter.paramValue[0] + offset, std::size(data));
                value.insert(::uwb::UwbMacAddress(std::move(data)));
            }
        } else { // ::uwb::UwbMacAddressType::Extended
            for (auto i = 0; i < (applicationConfigurationParameter.paramLength / ::uwb::UwbMacAddress::ExtendedLength); i++) {
                ::uwb::UwbMacAddress::ExtendedType data{};
                auto offset = i * ::uwb::UwbMacAddress::ExtendedLength;
                std::memcpy(std::data(data), &applicationConfigurationParameter.paramValue[0] + offset, std::size(data));
                value.insert(::uwb::UwbMacAddress(std::move(data)));
            }
        }

        uwbApplicationConfigurationParameter.Value = std::move(value);
    } else {
        PLOG_ERROR << "unexpected uwb application configuration parameter type encountered, type=" << std::showbase << std::hex << notstd::to_underlying(applicationConfigurationParameter.paramType);
        throw std::runtime_error("incorrect conversion function used for the given parameter type. Expected UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS");
    }

    return uwbApplicationConfigurationParameter;
}

std::vector<UwbApplicationConfigurationParameter>
windows::devices::uwb::ddi::lrp::To(const UWB_APP_CONFIG_PARAMS &applicationConfigurationParameters)
{
    std::vector<UwbApplicationConfigurationParameter> uwbApplicationConfigurationParameters{};

    std::optional<::uwb::UwbMacAddressType> macAddressMode;
    std::optional<UWB_APP_CONFIG_PARAM> destinationMacAddresses;
    detail::ForEachApplicationConfigurationParameter(applicationConfigurationParameters, [&uwbApplicationConfigurationParameters, &macAddressMode, &destinationMacAddresses](const auto &applicationConfigurationParameter) {
        // If parameter is MacAddressMode, store value
        if (To(applicationConfigurationParameter.paramType) == UwbApplicationConfigurationParameterType::MacAddressMode) {
            macAddressMode = std::get<::uwb::UwbMacAddressType>(To(applicationConfigurationParameter).Value);
        }

        // If parameter is DestinationMacAddresses, store until other parameters have been converted.
        if (To(applicationConfigurationParameter.paramType) == UwbApplicationConfigurationParameterType::DestinationMacAddresses) {
            destinationMacAddresses = applicationConfigurationParameter;
        } else {
            auto uwbApplicationConfigurationParameter = To(applicationConfigurationParameter);
            uwbApplicationConfigurationParameters.push_back(std::move(uwbApplicationConfigurationParameter));
        }
    });

    // Now that the other parameters have been converted, convert DestinationMacAddresses with special conversion function
    if (destinationMacAddresses.has_value()) {
        if (macAddressMode.has_value()) {
            uwbApplicationConfigurationParameters.push_back(std::move(To(destinationMacAddresses.value(), macAddressMode.value())));
        } else {
            uwbApplicationConfigurationParameters.push_back(std::move(To(destinationMacAddresses.value(), ::uwb::UwbMacAddressType::Short)));
        }
    } else {
        PLOG_ERROR << "missing DestinationMacAddresses value";
    }

    return std::move(uwbApplicationConfigurationParameters);
}
