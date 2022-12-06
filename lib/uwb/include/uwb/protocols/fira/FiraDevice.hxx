
#ifndef FIRA_DEVICE_HXX
#define FIRA_DEVICE_HXX

#include <iostream>

namespace uwb::protocol::fira
{
/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.1.
 */
enum class DeviceRole {
    Initiator,
    Responder,
};

// std::istream& operator>>(std::istream& is, DeviceRole& deviceRole);
std::ostream& operator<<(std::ostream& is, const DeviceRole& deviceRole);

bool lexical_cast(const std::string &input, DeviceRole& dr);

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
enum class StsConfiguration {
    Static,
    Dynamic,
    DynamicWithResponderSubSessionKey,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.3.
 */
enum class StsPacketConfiguration {
    SP0,
    SP1,
    SP2,
    SP3,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.4.
 */
enum class RangingMethod {
    OneWay,
    SingleSidedTwoWay,
    DoubleSidedTwoWay,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.5.
 */
enum class MultiNodeMode {
    Unicast,
    OneToMany,
    ManyToMany,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.6.
 */
enum class SchedulingMode {
    Contention,
    Time,
};

/**
 * @brief TODO Add spec reference. 
 */
enum class RangingMode {
    Interval,
    Block,
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
 * @brief TODO Add spec reference. 
 */
enum class ConvolutionalCodeConstraintLength {
    K3,
    K7,
};

/**
 * @brief TODO Add spec reference. 
 * 
 * Note: The spec does not define channel 7 or 11 which is why it's missing here.
 */
enum class Channel {
    C5,
    C6,
    C8,
    C9,
    C10,
    C12,
    C13,
    C14,
};

/**
 * @brief TODO Add spec reference.
 */
enum class PrfMode {
    Bprf,
    Hprf,
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
 * @brief TODO: Add spec reference.
 */
enum class ResultReportConfiguration {
    TofReport,
    AoAAzimuthReport,
    AoAElevationReport,
    AoAFoMReport,
};

} // namespace uwb::protocol::fira

#endif // FIRA_DEVICE_HXX
