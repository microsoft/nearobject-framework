
#ifndef FIRA_DEVICE_HXX
#define FIRA_DEVICE_HXX

#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace uwb::protocol::fira
{
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
enum class DeviceType {
    Controller,
    Controlee,
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

} // namespace uwb::protocol::fira

#endif // FIRA_DEVICE_HXX
