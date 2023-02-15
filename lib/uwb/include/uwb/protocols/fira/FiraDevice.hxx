
#ifndef FIRA_DEVICE_HXX
#define FIRA_DEVICE_HXX

#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbVersion.hxx>

namespace uwb::protocol::fira
{
using ::uwb::UwbMacAddress;
using ::uwb::UwbVersion;

/**
 * @brief Converts the binary representation of the Fira PHY and Mac version to
 * a string.
 *
 * @param input
 * @return std::string
 */
std::string
VersionToString(uint32_t input) noexcept;

/**
 * @brief Converts the string representation of the Fira PHY and Mac version to
 * the binary.
 *
 * @param input
 * @return std::optional<uint32_t>
 */
std::optional<uint32_t>
StringToVersion(const std::string& input) noexcept;

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.1.
 */
enum class DeviceRole : uint8_t {
    Responder = 0,
    Initiator = 1,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.1.
 */
enum class DeviceType : uint8_t {
    Controlee = 0,
    Controller = 1,
};

/**
 * @brief TODO Add spec reference.
 */
enum class StsConfiguration : uint8_t {
    Static = 0,
    Dynamic = 1,
    DynamicWithResponderSubSessionKey = 2,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.3.
 */
enum class StsPacketConfiguration : uint8_t {
    SP0 = 0,
    SP1 = 1,
    SP2 = 2,
    SP3 = 3,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.4.
 */
enum class RangingDirection : uint8_t {
    OneWay = 0,
    SingleSidedTwoWay = 1,
    DoubleSidedTwoWay = 2,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.5.
 */
enum class MultiNodeMode : uint8_t {
    Unicast = 0,
    OneToMany = 1,
    ManyToMany = 2,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.6.
 */
enum class SchedulingMode {
    Contention = 0,
    Time = 1,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 7.5.3.3, Table 53.
 */
enum class RangingMode : uint8_t {
    Interval = 0,
    Block = 1,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.7.
 */
enum class MeasurementReportMode {
    None,
    Deferred,
    NonDeferred,
};

/**
 * @brief TODO Add spec reference.
 */
enum class AngleOfArrival {
    Azimuth90,
    Azimuth180,
    Elevation,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 7.5.3.3, Table 53.
 */
enum class ConvolutionalCodeConstraintLength {
    K3 = 0,
    K7 = 1,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 7.5.3.3, Table 53.
 *
 * Note: The spec does not define channels 7 nor 11 which is why they're missing
 * here.
 */
enum class Channel {
    C5 = 5,
    C6 = 6,
    C8 = 8,
    C9 = 9,
    C10 = 10,
    C12 = 12,
    C13 = 13,
    C14 = 14,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 7.5.3.3, Table 53.
 */
enum class PrfMode {
    Bprf = 0,
    Hprf = 1,
};

/**
 * @brief TODO Add spec reference.
 */
enum class BprfParameter {
    Set1,
    Set2,
    Set3,
    Set4,
    Set5,
    Set6,
};

/**
 * @brief TODO Add spec reference.
 */
enum class HprfParameter {
    Set1,
    Set2,
    Set3,
    Set4,
    Set5,
    Set6,
    Set7,
    Set8,
    Set9,
    Set10,
    Set11,
    Set12,
    Set13,
    Set14,
    Set15,
    Set16,
    Set17,
    Set18,
    Set19,
    Set20,
    Set21,
    Set22,
    Set23,
    Set24,
    Set25,
    Set26,
    Set27,
    Set28,
    Set29,
    Set30,
    Set31,
    Set32,
    Set33,
    Set34,
    Set35,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 7.5.3.3, Table 53.
 */
enum class ResultReportConfiguration : uint8_t {
    TofReport = 0b00000001,
    AoAAzimuthReport = 0b00000010,
    AoAElevationReport = 0b00000100,
    AoAFoMReport = 0b00001000,
};

/**
 * @brief Converts a std::unordered_set of ResultReportConfiguration to string
 *
 * @return std::string
 */
std::string
ResultReportConfigurationToString(const std::unordered_set<ResultReportConfiguration>& resultReportConfiguration);

/**
 * @brief Converts a string to vector of ResultReportConfiguration.
 *
 * @param input The string input to convert.
 * @return std::optional<std::unordered_set<ResultReportConfiguration>>
 */
std::optional<std::unordered_set<ResultReportConfiguration>>
StringToResultReportConfiguration(const std::string& input);

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
enum class UwbSessionState {
    Initialized,
    Deinitialized,
    Active,
    Idle,
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
    NonLineOfSight,
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

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string
    ToString() const;
};

struct UwbDeviceInfoVendor
{
    virtual ~UwbDeviceInfoVendor() = default;
};

struct UwbDeviceInformation
{
    UwbVersion VersionUwb;
    UwbVersion VersionUci;
    UwbVersion VersionMac;
    UwbVersion VersionPhy;
    UwbStatus Status;
    std::shared_ptr<UwbDeviceInfoVendor> VendorSpecificInfo;
};

struct UwbDeviceConfigurationParameter
{};

enum class RangingRoundControl : uint8_t {
    RangingResultReportMessage = 0b00000001U,
    ControlMessage = 0b00000010U,
    MeasurementReportMessage = 0b10000000U,
};

enum class AoAResult : uint8_t {
    Disable = 0U,
    Enable = 1U,
};

enum class RangeDataNotificationConfiguration : uint8_t {
    Disable = 0U,
    Enable = 1U,
};

enum class PreambleDuration : uint8_t {
    Symbols32 = 0x00U,
    Symbols64 = 0x01U,
};

enum class TxAdaptivePayloadPower : uint8_t {
    Disable = 0U,
    Enable = 1U,
};

enum class PrfModeDetailed : uint8_t {
    Bprf62MHz = 0x00U,
    Hprf124MHz = 0x01U,
    Hprf249MHz = 0x02U,
};

enum class KeyRotation : uint8_t {
    Disable = 0U,
    Enable = 1U,
};

enum class BprfPhrDataRate : uint8_t {
    Rate850kbps = 0x00U,
    Rate6Mbps = 0x01U,
};

enum class StsLength : uint8_t {
    Symbols32 = 0x00U,
    Symbols64 = 0x01U,
    Symbols128 = 0x02U,
};

struct UwbMulticastListStatus
{
    uint16_t ControleeMacAddress; // why is this uint16_t? TODO: replace with uwb::UwbMacAddress
    uint32_t SubSessionId;
    UwbStatusMulticast Status;

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string
    ToString() const;
};

enum class UwbMulticastAction {
    AddShortAddress,
    DeleteShortAddress,
};

struct UwbSessionUpdateMulticastListEntry 
{
    UwbMacAddress ShortAddress;
    uint32_t SubSessionId;
};

struct UwbSessionUpdateMulicastList
{
    uint32_t SessionId;
    UwbMulticastAction Action;
    std::vector<UwbSessionUpdateMulticastListEntry> Controlees;
};

struct UwbSessionUpdateMulicastListStatus
{
    uint32_t SessionId;
    std::vector<UwbMulticastListStatus> Status;
};

struct UwbSessionStatus
{
    uint32_t SessionId;
    UwbSessionState State;
    std::optional<UwbSessionReasonCode> ReasonCode;

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string
    ToString() const;
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

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string
    ToString() const;
};

using UwbNotificationData = std::variant<UwbStatus, UwbStatusDevice, UwbSessionStatus, UwbSessionUpdateMulicastListStatus, UwbRangingData>;

std::string
ToString(const UwbNotificationData& uwbNotificationData);

} // namespace uwb::protocol::fira

#endif // FIRA_DEVICE_HXX
