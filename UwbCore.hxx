
#ifndef UWB_CORE_HXX
#define UWB_CORE_HXX

#include <memory>
#include <optional>
#include <variant>

#include "UwbVersion.hxx"

namespace windows::devices::uwb
{
enum class UwbStatusSession {
    NotExist,
    Duplicate,
    Active,
    MaxSessionsExceeded,
    NotConfigured,
    ActiveSessionsOngoing,
    MulticastListFull,
    AddressNotFound,
    AddressAlreadyPresent,
};
enum class UwbStatusRanging {
    TxFailed,
    RxTimeout,
    RxPhyDecodingFailed,
    RxPhyToaFailed,
    RxPhyStsFailed,
    MacDecodingFailed,
    RxMacIeDecodingFailed,
    RxMacIeMissing,
};
enum class UwbStatusGeneric {
    Ok,
    Rejected,
    Failed,
    SyntaxError,
    InvalidParameter,
    InvalidRange,
    InvalidMessageSize,
    UnknownGid,
    UnknownOid,
    ReadOnly,
    CommandRetry,
};

constexpr auto UwbStatusOk = UwbStatusGeneric::Ok;

using UwbStatus = std::variant<UwbStatusGeneric, UwbStatusSession, UwbStatusRanging>;

enum class UwbStatusMulticast {
    OkUpdate,
    ErrorListFull,
    ErrorKeyFetchFail,
    ErrorSubSessionIdNotFound,
};
enum class UwbDeviceState {
    Ready,
    Active,
    Error,
};
enum class UwbSessionType {
    RangingSession,
    TestMode,
};
enum class UwbSessionReasonCode {
    StateChangeWithSessionManagementCommands,
    MaxRangignRoundRetryCountReached,
    MaxNumberOfMeasurementsReached,
    ErrorSlotLengthNotSupported,
    ErrorInsufficientSlotsPerRangingRound,
    ErrorMacAddressModeNotSupported,
    ErrorInvalidRangingInterval,
    ErrorInvalidStsConfiguration,
    ErrorInvalidRFrameConfiguration,
};
enum class UwbLineOfSightIndicator {
    LineOfSight,
    NoneLineOfSight,
    Indeterminant,
};
enum class UwbRangingMeasurementType {
    TwoWay,
};
enum class UwbDeviceConfigurationParameterType {
    DeviceState,
    LowPowerMode,
};

struct UwbStatusDevice
{
    UwbDeviceState State;
};

struct UwbDeviceInfoVendor
{
    virtual ~UwbDeviceInfoVendor() = default;
};

struct UwbDeviceInfo
{
    Version VersionUwb;
    Version VersionUci;
    Version VersionMac;
    Version VersionPhy;
    UwbStatus Status;
    std::unique_ptr<UwbDeviceInfoVendor> VendorSpecificInfo;
};

struct UwbDeviceCapabilities
{};

struct UwbDeviceConfigurationParameter
{};

enum class UwbApplicationConfigurationParameterType {
    DeviceType,
    // TODO: replace with NOF definitions when moved to repo
};

struct UwbApplicationConfigurationParameter
{};

struct UwbSessionState
{
    uint32_t Id;
};

struct UwbMacAddress
{};

struct UwbStatusMulticastList
{
    uint16_t ControleeMacAddress; // why is this uint16_t?
    uint32_t SubSessionId;
    UwbStatusMulticast Status;
};

enum class UwbMulticastAction {
    AddShortAddress,
    DeleteShortAddress,
};

struct UwbSessionUpdateControllerMulticastListEvent
{
    uint32_t SessionId;
    uint32_t RemainingMulticastListSize;
    uint32_t NumberOfControlees;
    std::vector<UwbStatusMulticastList> Status;
};
struct UwbRangingMeasurementData
{
    uint16_t Result;
    std::optional<uint8_t> FigureOfMerit;
    decltype(FigureOfMerit)& FoM = FigureOfMerit;
};

struct UwbRangingMeasurement
{
    uint8_t SlotIndex;
    uint16_t Distance;
    UwbStatus Status;
    UwbMacAddress PeerMacAddress;
    UwbLineOfSightIndicator LineOfSignIndicator;
    UwbRangingMeasurementData AoAAzimuth;
    UwbRangingMeasurementData AoAElevation;
    UwbRangingMeasurementData AoaDestinationAzimuth;
    UwbRangingMeasurementData AoaDestinationElevation;
};
struct UwbRangingData
{
    uint32_t SequenceNumber;
    uint32_t SessionId;
    uint32_t CurrentRangingInterval;
    UwbRangingMeasurementType RangingMeasurementType;
    std::vector<UwbRangingMeasurement> RangingMeasurements;
};

using UwbNotificationData = std::variant<UwbStatus, UwbStatusDevice, UwbStatusSession, UwbStatusMulticast, UwbRangingData>;

} // namespace windows::devices::uwb

#endif // UWB_CORE_HXX
