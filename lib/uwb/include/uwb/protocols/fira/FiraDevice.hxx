
#ifndef FIRA_DEVICE_HXX
#define FIRA_DEVICE_HXX

#include <array>
#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <string>
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
 * @brief See FiRa Consortium MAC Technical Requirements v1.3.0,
 * Section D.1.8 STS, Figure 19, page 70.
 */
constexpr std::size_t StaticStsInitializationVectorLength = 6;

/**
 * @brief Static STS initialization vector type.
 */
using StaticStsInitializationVector = std::array<uint8_t, StaticStsInitializationVectorLength>;

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
 * @brief See FiRa Consortium UWB Command Interface Generic Technical
 * Specification v1.1.0, Section 8.3, Table 29.
 */
enum class RangingRoundUsage : uint8_t {
    Rfu = 0x00U,
    SingleSidedTwoWayRangingWithDeferredMode = 0x01U,
    DoubleSidedTwoWayRangingWithDeferredMode = 0x02U,
    SingleSidedTwoWayRangingNonDeferredMode = 0x03U,
    DoubleSidedTwoWayRangingNonDeferredMode = 0x04U,
};

/**
 * @brief See FiRa Consortium UWB Command Interface Generic Technical
 * Specification v1.1.0, Section 8.3, Table 29.
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
ToString(const std::unordered_set<ResultReportConfiguration>& resultReportConfiguration);

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

/**
 * @brief Determines if the specified UWB status describes a successful result.
 *
 * @param uwbStatus The status to check.
 * @return true
 * @return false
 */
bool
IsUwbStatusOk(const UwbStatus& uwbStatus) noexcept;

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
    Uninitialized,
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

    auto
    operator<=>(const UwbStatusDevice&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
     *
     * @return std::string
     */
    std::string
    ToString() const;
};

struct UwbDeviceInfoVendor
{
    virtual ~UwbDeviceInfoVendor() = default;

    /**
     * @brief Provides a view of the vendor-specific data.
     *
     * @return std::span<const uint8_t>
     */
    virtual std::span<const uint8_t>
    GetData() const noexcept = 0;
};

struct UwbDeviceInformation
{
    UwbVersion VersionUci;
    UwbVersion VersionUciTest;
    UwbVersion VersionMac;
    UwbVersion VersionPhy;
    UwbStatus Status;
    std::shared_ptr<UwbDeviceInfoVendor> VendorSpecificInfo;

    std::string
    ToString() const;

    auto
    operator<=>(const UwbDeviceInformation&) const noexcept = default;
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

enum class PsduDataRate : uint8_t {
    Rate6810kbps = 0x00U,
    Rate7800kbps = 0x01U,
    Rate2720kbps = 0x02U,
    Rate3120kbps = 0x03U,
    Rate850kbps = 0x04U,
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

/**
 * @brief See FiRa Consortium - UCI Generic Specification v1.1.0, Section 8.3,
 * Table 29, 'APP Configuration Parameter IDs'.
 */
enum class UwbApplicationConfigurationParameterType : uint8_t {
    DeviceType = 0x00U,
    RangingRoundUsage = 0x01U,
    StsConfiguration = 0x02U,
    MultiNodeMode = 0x03U,
    ChannelNumber = 0x04U,
    NumberOfControlees = 0x05U,
    DeviceMacAddress = 0x06U,
    DestinationMacAddresses = 0x07U,
    SlotDuration = 0x08U,
    RangingInterval = 0x09U,
    StsIndex = 0x0AU,
    MacFcsType = 0x0BU,
    RangingRoundControl = 0x0CU,
    AoAResultRequest = 0x0DU,
    RangeDataNotificationConfig = 0x0EU,
    RangeDataNotificationProximityNear = 0x0FU,
    RangeDataNotificationProximityFar = 0x10U,
    DeviceRole = 0x11U,
    RFrameConfiguration = 0x12U,
    PreambleCodeIndex = 0x14U,
    SfdId = 0x15U,
    PsduDataRate = 0x16U,
    PreambleDuration = 0x17U,
    RangingTimeStruct = 0x1AU,
    SlotsPerRangingRound = 0x1BU,
    TxAdaptivePayloadPower = 0x1CU,
    ResponderSlotIndex = 0x1EU,
    PrfMode = 0x1FU,
    ScheduledMode = 0x22U,
    KeyRotation = 0x23U,
    KeyRotationRate = 0x24U,
    SessionPriority = 0x25U,
    MacAddressMode = 0x26U,
    VendorId = 0x27U,
    StaticStsIv = 0x28U,
    NumberOfStsSegments = 0x29U,
    MaxRangingRoundRetry = 0x2AU,
    UwbInitiationTime = 0x2BU,
    HoppingMode = 0x2CU,
    BlockStrideLength = 0x2DU,
    ResultReportConfig = 0x2EU,
    InBandTerminationAttemptCount = 0x2FU,
    SubSessionId = 0x30U,
    BprfPhrDataRate = 0x31U,
    MaxNumberOfMeasurements = 0x32U,
    StsLength = 0x35U,
};

// clang-format off
using UwbApplicationConfigurationParameterValue = std::variant<
    bool, // HOPPING_MODE, tag 0x2C, size 1
    uint8_t, // NUMBER_OF_CONTROLLEES, tag 0x05, PREAMBLE_CODE_INDEX, tag 0x14, SFD_ID, tag 0x15, SLOTS_PER_RR, tag 0x1B, RESPONDER_SLOT_INDEX, tag 0x1E, KEY_ROTATION_RATE, tag 0x24, SESSION_PRIORITY, tag 0x25, NUMBER_OF_STS_SEGMENTS, tag 0x29, BLOCK_STRIDE_LENGTH, tag 0x2D, IN_BAND_TERMMINATION_ATTEMPT_COUNT, tag 0x2F, size 1
    uint16_t, // SLOT_DURATION, tag 0x08, RANGE_DATA_NTF_PROXIMITY_NEAR, tag 0x0F, RANGE_DATA_NTF_PROXIMITY_FAR, tag 0x10, VENDOR_ID, tag 0x27, MAX_RR_RETRY, tag 0x2A, MAX_NUMBER_OF_MEASUREMENTS, tag 0x32, size 2
    uint32_t, // RANGING_INTERVAL, tag 0x09, STS_INDEX, tag 0x0A, UWB_INITIATION_TIME, tag 0x2B, SUB_SESSION_ID, tag 0x30, size 4
    AoAResult, // AOA_RESULT_REQ, tag 0x0D, size 1
    BprfPhrDataRate, // BPRF_PHR_DATA_RATE, tag 0x31, size 1
    Channel, // CHANNEL_NUMBER, tag 0x04, size 1
    DeviceRole, // DEVICE_ROLE, tag 0x11, size 1
    DeviceType, // DEVICE_TYPE, tag 0x00, size 1
    KeyRotation, // KEY_ROTATION, tag 0x23, size 1
    MultiNodeMode, // MULTI_NODE_MODE, tag 0x03, size 1
    PreambleDuration, // PREAMBLE_DURATION, tag 0x17, size 1
    PrfMode, // PRF_MODE, tag 0x1F, size 1
    PsduDataRate, // PSDU_DATA_RATE, tag 0x16, size 1
    RangeDataNotificationConfiguration, // RANGE_DATA_NTF_CONFIG, tag 0x0E, size 1
    RangingRoundUsage, // RANGING_ROUND_USAGE, tag0x01, size 1
    RangingMode, // RANGING_TIME_STRUCT, tag 0x1A, size 1
    RangingRoundControl, // RANGING_ROUND_CONTROL, tag 0x0C, size 1,
    std::unordered_set<ResultReportConfiguration>, // RESULT_REPORT_CONFIG, tag 0x2E, size 1 // TODO this should really be a set
    SchedulingMode, // SCHEDULED_MODE, tag 0x22, size 1
    StsConfiguration, // STS_CONFIG, tag 0x02, size 1
    StsLength, // STS_LENGTH, tag 0x035, length 1,
    StsPacketConfiguration, // RFRAME_CONFIG, tag 0x12, size 1
    TxAdaptivePayloadPower, // TX_ADAPTIVE_PAYLOAD_POWER, tag 0x1C, size 1
    ::uwb::UwbMacAddress, // DEVICE_MAC_ADDRESS, tag 0x06, size 2/8
    ::uwb::UwbMacAddressFcsType, // MAC_FCS_TYPE, tag 0x0B, size 1
    ::uwb::UwbMacAddressType, // MAC_ADDRESS_MODE, tag 0x26, size 1
    std::array<uint8_t, StaticStsInitializationVectorLength>, // STATIC_STS_IV, tag 0x28, size 6
    std::unordered_set<::uwb::UwbMacAddress>>; // DST_MAC_ADDRESS, tag 0x07, size 2/8*N
// clang-format on

/**
 * @brief Represents a FiRa UWB Application Configuration Parameter as
 * described in the FiRa Consortium UWB Command Interface Generic Technical
 * Specification.
 */
struct UwbApplicationConfigurationParameter
{
    UwbApplicationConfigurationParameterType Type;
    UwbApplicationConfigurationParameterValue Value;

    bool
    operator==(const UwbApplicationConfigurationParameter&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
     * 
     * @return std::string 
     */
    std::string
    ToString() const;
};

struct UwbMulticastListStatus
{
    uwb::UwbMacAddress ControleeMacAddress;
    uint32_t SubSessionId;
    UwbStatusMulticast Status;

    auto
    operator<=>(const UwbMulticastListStatus&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
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
    UwbMacAddress ControleeMacAddress;
    uint32_t SubSessionId;

    auto
    operator<=>(const UwbSessionUpdateMulticastListEntry&) const noexcept = default;
};

struct UwbSessionUpdateMulicastList
{
    uint32_t SessionId;
    UwbMulticastAction Action;
    std::vector<UwbSessionUpdateMulticastListEntry> Controlees;

    auto
    operator<=>(const UwbSessionUpdateMulicastList&) const noexcept = default;
};

struct UwbSessionUpdateMulicastListStatus
{
    uint32_t SessionId;
    std::vector<UwbMulticastListStatus> Status;

    auto
    operator<=>(const UwbSessionUpdateMulicastListStatus&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
     *
     * @return std::string
     */
    std::string
    ToString() const;
};

struct UwbSessionStatus
{
    uint32_t SessionId;
    UwbSessionState State{ UwbSessionState::Deinitialized };
    std::optional<UwbSessionReasonCode> ReasonCode;

    auto
    operator<=>(const UwbSessionStatus&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
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

    auto
    operator<=>(const UwbRangingMeasurementData&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
     *
     * @return std::string
     */
    std::string
    ToString() const;
};

struct UwbRangingMeasurement
{
    uint8_t SlotIndex;
    uint16_t Distance;
    UwbStatus Status;
    UwbMacAddress PeerMacAddress;
    UwbLineOfSightIndicator LineOfSightIndicator;
    UwbRangingMeasurementData AoAAzimuth;
    UwbRangingMeasurementData AoAElevation;
    UwbRangingMeasurementData AoaDestinationAzimuth;
    UwbRangingMeasurementData AoaDestinationElevation;

    auto
    operator<=>(const UwbRangingMeasurement&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
     *
     * @return std::string
     */
    std::string
    ToString() const;
};

struct UwbRangingData
{
    uint32_t SequenceNumber;
    uint32_t SessionId;
    uint32_t CurrentRangingInterval;
    UwbRangingMeasurementType RangingMeasurementType;
    std::vector<UwbRangingMeasurement> RangingMeasurements;

    auto
    operator<=>(const UwbRangingData&) const noexcept = default;

    /**
     * @brief Returns a string representation of the object.
     *
     * @return std::string
     */
    std::string
    ToString() const;
};

using UwbNotificationData = std::variant<UwbStatus, UwbStatusDevice, UwbSessionStatus, UwbSessionUpdateMulicastListStatus, UwbRangingData>;

/**
 * @brief Returns a string representation of the object.
 *
 * @param uwbStatus
 * @return std::string
 */
std::string
ToString(const UwbStatus& uwbStatus);

/**
 * @brief Returns a string representation of the object.
 *
 * @param uwbNotificationData
 * @return std::string
 */
std::string
ToString(const UwbNotificationData& uwbNotificationData);

/**
 * @brief Returns a string representation of the object.
 * 
 * @param uwbApplicationConfigurationParameterValue 
 * @return std::string 
 */
std::string
ToString(const UwbApplicationConfigurationParameterValue &uwbApplicationConfigurationParameterValue);

} // namespace uwb::protocol::fira

#endif // FIRA_DEVICE_HXX
