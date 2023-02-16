
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <variant>

#include <uwb/protocols/fira/UwbCapability.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

using namespace ::uwb::protocol::fira;

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
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbDeviceState &uwbDeviceState)
{
    static const std::unordered_map<UwbDeviceState, UWB_DEVICE_STATE> DeviceStateMap{
        { UwbDeviceState::Ready, UWB_DEVICE_STATE_READY },
        { UwbDeviceState::Active, UWB_DEVICE_STATE_ACTIVE },
        { UwbDeviceState::Error, UWB_DEVICE_STATE_ERROR },
    };

    return DeviceStateMap.at(uwbDeviceState);
}

UWB_MULTICAST_ACTION
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbMulticastAction &uwbMulticastAction)
{
    static const std::unordered_map<UwbMulticastAction, UWB_MULTICAST_ACTION> ActionMap{
        { UwbMulticastAction::AddShortAddress, UWB_MULTICAST_ACTION_ADD_SHORT_ADDRESS },
        { UwbMulticastAction::DeleteShortAddress, UWB_MULTICAST_ACTION_DELETE_SHORT_ADDRESS },
    };

    return ActionMap.at(uwbMulticastAction);
}

UWB_MULTICAST_STATUS
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbStatusMulticast &uwbStatusMulticast)
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
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbMulticastListStatus &uwbStatusMulticastList)
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
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbSessionUpdateMulticastListEntry &uwbSessionUpdateMulticastListEntry)
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

UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbSessionUpdateMulicastList &uwbSessionUpdateMulicastList)
{
    UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST sessionUpdateControllerMulticastList{};
    sessionUpdateControllerMulticastList.size = sizeof sessionUpdateControllerMulticastList; // TODO: update for variable length
    sessionUpdateControllerMulticastList.sessionId = uwbSessionUpdateMulicastList.SessionId;
    sessionUpdateControllerMulticastList.action = From(uwbSessionUpdateMulicastList.Action);
    sessionUpdateControllerMulticastList.numberOfControlees = std::size(uwbSessionUpdateMulicastList.Controlees);
    // TODO: append controlee information

    return sessionUpdateControllerMulticastList;
}

UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus &uwbSessionUpdateMulicastListStatus)
{
    UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF multicastListStatus{};
    multicastListStatus.size = sizeof multicastListStatus; // TODO: update for variable length
    multicastListStatus.sessionId = uwbSessionUpdateMulicastListStatus.SessionId;
    multicastListStatus.numberOfControlees = std::size(uwbSessionUpdateMulicastListStatus.Status);
    multicastListStatus.remainingMulticastListSize = 0;
    // TODO: append status information

    return multicastListStatus;
}

UWB_RANGING_MEASUREMENT_TYPE
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbRangingMeasurementType &uwbRangingType)
{
    static const std::unordered_map<UwbRangingMeasurementType, UWB_RANGING_MEASUREMENT_TYPE> RangingTypeMap{
        { UwbRangingMeasurementType::TwoWay, UWB_RANGING_MEASUREMENT_TYPE_TWO_WAY },
    };

    return RangingTypeMap.at(uwbRangingType);
}

UWB_SESSION_REASON_CODE
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbSessionReasonCode &uwbSessionReasonCode)
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
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbDeviceConfigurationParameterType &uwbDeviceConfigurationParameterType)
{
    static const std::unordered_map<UwbDeviceConfigurationParameterType, UWB_DEVICE_CONFIG_PARAM_TYPE> ConfigParamMap{
        { UwbDeviceConfigurationParameterType::DeviceState, UWB_DEVICE_CONFIG_PARAM_TYPE_DEVICE_STATE },
        { UwbDeviceConfigurationParameterType::LowPowerMode, UWB_DEVICE_CONFIG_PARAM_TYPE_LOW_POWER_MODE },
    };

    return ConfigParamMap.at(uwbDeviceConfigurationParameterType);
}

UWB_APP_CONFIG_PARAM_TYPE
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbApplicationConfigurationParameterType &uwbApplicationConfigurationParameterType)
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
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbSessionStatus &uwbSessionStatus)
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

UWB_DEVICE_INFO
windows::devices::uwb::ddi::lrp::From(const UwbDeviceInformation &uwbDeviceInfo)
{
    UWB_DEVICE_INFO deviceInfo{};
    deviceInfo.size = sizeof deviceInfo;
    deviceInfo.status = From(uwbDeviceInfo.Status);
    deviceInfo.uciGenericVersionMajor = uwbDeviceInfo.VersionUci.Major;
    deviceInfo.uciGenericVersionMinorAndMaintenance = uwbDeviceInfo.VersionUci.Minor | uwbDeviceInfo.VersionUci.Maintenance; // TODO: FIXME
    deviceInfo.uciTestVersionMajor = uwbDeviceInfo.VersionUciTest.Major;
    deviceInfo.uciTestVersionMinorAndMaintenance = uwbDeviceInfo.VersionUciTest.Minor | uwbDeviceInfo.VersionUciTest.Maintenance;
    deviceInfo.macVersionMajor = uwbDeviceInfo.VersionMac.Major;
    deviceInfo.macVersionMinorAndMaintenance = uwbDeviceInfo.VersionMac.Minor | uwbDeviceInfo.VersionMac.Maintenance;
    deviceInfo.phyVersionMajor = uwbDeviceInfo.VersionPhy.Major;
    deviceInfo.phyVersionMinorAndMaintenance = uwbDeviceInfo.VersionPhy.Minor | uwbDeviceInfo.VersionPhy.Maintenance;

    if (uwbDeviceInfo.VendorSpecificInfo != nullptr) {
        auto vendorSpecificInfo = uwbDeviceInfo.VendorSpecificInfo->GetData();
        deviceInfo.vendorSpecificInfoLength = std::size(vendorSpecificInfo);
        // TODO: append std::data(vendorSpecificInfo) to end of structure.
    } else {
        deviceInfo.vendorSpecificInfoLength = 0;
    }

    return deviceInfo;
}

UWB_DEVICE_CAPABILITIES
windows::devices::uwb::ddi::lrp::From(const UwbCapability &uwbDeviceCapabilities)
{
    UWB_DEVICE_CAPABILITIES deviceCapabilities{};
    deviceCapabilities.size = sizeof deviceCapabilities;
    deviceCapabilities.capabilityParamsCount = 0;
    // TODO: implement this properly
    return deviceCapabilities;
}

UWB_DEVICE_STATUS
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbStatusDevice &uwbStatusDevice)
{
    UWB_DEVICE_STATUS statusDevice{};
    statusDevice.size = sizeof statusDevice;
    statusDevice.deviceState = From(uwbStatusDevice.State);

    return statusDevice;
}

UWB_RANGING_DATA
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbRangingData &uwbRangingData)
{
    UWB_RANGING_DATA rangingData{};            // TODO: this must be allocated to account for 'RangingMeasurements' in uwbRangingData.
    rangingData.size = sizeof rangingData + 0; // TODO: fix this to account for variable length
    rangingData.sequenceNumber = uwbRangingData.SequenceNumber;
    rangingData.sessionId = uwbRangingData.SessionId;
    rangingData.currentRangingInterval = uwbRangingData.CurrentRangingInterval;
    rangingData.rangingMeasurementType = From(uwbRangingData.RangingMeasurementType);
    rangingData.numberOfRangingMeasurements = std::size(uwbRangingData.RangingMeasurements);

    return rangingData;
}

UWB_NOTIFICATION_DATA
windows::devices::uwb::ddi::lrp::From(const ::uwb::protocol::fira::UwbNotificationData &uwbNotificationData)
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
        // supported, at at() call above will throw std::out_of_range, ensuring
        // this code will never be reached.
    },
        uwbNotificationData);

    return notificationData;
}

::uwb::protocol::fira::UwbNotificationData
windows::devices::uwb::ddi::lrp::To(const UWB_NOTIFICATION_DATA& /* notificationData */)
{
    UwbNotificationData uwbNotificationData{};
    // TODO: implement this
    return uwbNotificationData;
}
