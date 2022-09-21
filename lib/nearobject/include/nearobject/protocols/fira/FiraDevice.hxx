
#ifndef FIRA_DEVICE_HXX
#define FIRA_DEVICE_HXX

namespace nearobject::protocol::fira
{
/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.1.
 */
enum class DeviceRole {
    Initiator,
    Responder,
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
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.3.
 */
enum class StsPacketConfiguration {
    SP0,
    SP1,
    SP3,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.4.
 */
enum class RangingMethod {
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
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.6.
 */
enum class SchedulingMode {
    TimeBased,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.7.
 */
enum class MeasurementReportMode {
    Deferred,
    NonDeferred,
};

/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 5.8.
 */
enum class BlockBasedMode {
    RoundHopping,
    HoppingSequence,
    BlockStriding,
};

} // namespace nearobject::protocol::fira

#endif // FIRA_DEVICE_HXX
