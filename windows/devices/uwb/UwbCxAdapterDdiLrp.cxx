
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

using namespace ::uwb::protocol::fira;

UWB_STATUS
windows::devices::uwb::ddi::lrp::From(const UwbStatus& uwbStatus)
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

    std::visit([&status](auto&& arg) {
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

UWB_SESSION_STATE
windows::devices::uwb::ddi::lrp::From(const UwbSessionState uwbSessionState)
{
    switch (uwbSessionState) {
    case UwbSessionState::Initialized:
        return UWB_SESSION_STATE_INIT;
    case UwbSessionState::Deinitialized:
        return UWB_SESSION_STATE_DEINIT;
    case UwbSessionState::Active:
        return UWB_SESSION_STATE_ACTIVE;
    case UwbSessionState::Idle:
        return UWB_SESSION_STATE_IDLE;
    }

    throw std::runtime_error("unknown enumeration value");
}

UWB_DEVICE_INFO
windows::devices::uwb::ddi::lrp::From(const UwbDeviceInfoInformation& uwbDeviceInfo)
{
    UWB_DEVICE_INFO deviceInfo{};
    deviceInfo.size = sizeof deviceInfo;
    deviceInfo.status = From(uwbDeviceInfo.Status);
    deviceInfo.vendorSpecificInfoLength = 0;
    // TODO: fill in remaining fields
    return deviceInfo;
}

UWB_DEVICE_CAPABILITIES
windows::devices::uwb::ddi::lrp::From(const UwbDeviceCapabilities& /*uwbDeviceCapabilities*/)
{
    UWB_DEVICE_CAPABILITIES deviceCapabilities{};
    deviceCapabilities.size = sizeof deviceCapabilities;
    deviceCapabilities.capabilityParamsCount = 0;
    // TODO: implement this properly
    return deviceCapabilities;
}
