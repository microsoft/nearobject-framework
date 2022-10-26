
#include <TlvSimple.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;

const std::initializer_list<RangingConfiguration> UwbCapability::RangingConfigurationsDefault = {
    RangingConfiguration{ RangingMethod::OneWay, MeasurementReportMode::None },
    RangingConfiguration{ RangingMethod::SingleSidedTwoWay, MeasurementReportMode::Deferred },
    RangingConfiguration{ RangingMethod::DoubleSidedTwoWay, MeasurementReportMode::Deferred },
    RangingConfiguration{ RangingMethod::SingleSidedTwoWay, MeasurementReportMode::NonDeferred },
    RangingConfiguration{ RangingMethod::DoubleSidedTwoWay, MeasurementReportMode::NonDeferred },
};

const std::unordered_map<MultiNodeMode, std::size_t> UwbCapability::MultiNodeModeBit = {
    { MultiNodeMode::Unicast, 0 },
    { MultiNodeMode::OneToMany, 1 },
    { MultiNodeMode::ManyToMany, 2 },
};

const std::unordered_map<DeviceRole, std::size_t> UwbCapability::DeviceRoleBit = {
    { DeviceRole::Responder, 0 },
    { DeviceRole::Initiator, 1 },
};

const std::unordered_map<StsConfiguration, std::size_t> UwbCapability::StsConfigurationBit = {
    { StsConfiguration::Static, 0 },
    { StsConfiguration::Dynamic, 1 },
    { StsConfiguration::DynamicWithResponderSubSessionKey, 2 },
};

const std::unordered_map<StsPacketConfiguration, std::size_t> UwbCapability::RFrameConfigurationBit = {
    { StsPacketConfiguration::SP0, 0 },
    { StsPacketConfiguration::SP1, 1 },
    { StsPacketConfiguration::SP2, 2 },
    { StsPacketConfiguration::SP3, 3 },
};

const std::unordered_map<AngleOfArrival, std::size_t> UwbCapability::AngleOfArrivalBit = {
    { AngleOfArrival::Azimuth90, 0 },
    { AngleOfArrival::Azimuth180, 1 },
    { AngleOfArrival::Elevation, 2 },
};

const std::unordered_map<SchedulingMode, std::size_t> UwbCapability::SchedulingModeBit = {
    { SchedulingMode::Contention, 0 },
    { SchedulingMode::Time, 1 },
};

const std::unordered_map<RangingMode, std::size_t> UwbCapability::RangingModeBit = {
    { RangingMode::Block, 0 },
    { RangingMode::Interval, 1 },
};

const std::unordered_map<RangingConfiguration, std::size_t> UwbCapability::RangingConfigurationBit = {
    { { RangingMethod::OneWay, MeasurementReportMode::None }, 0 },
    { { RangingMethod::SingleSidedTwoWay, MeasurementReportMode::Deferred }, 1 },
    { { RangingMethod::DoubleSidedTwoWay, MeasurementReportMode::Deferred }, 2 },
    { { RangingMethod::SingleSidedTwoWay, MeasurementReportMode::NonDeferred }, 3 },
    { { RangingMethod::DoubleSidedTwoWay, MeasurementReportMode::NonDeferred }, 4 },
};

const std::unordered_map<ConvolutionalCodeConstraintLength, std::size_t> UwbCapability::ConvolutionalCodeConstraintLengthsBit = {
    { ConvolutionalCodeConstraintLength::K3, 0 },
    { ConvolutionalCodeConstraintLength::K7, 1 },
};

const std::unordered_map<Channel, std::size_t> UwbCapability::ChannelsBit = {
    { Channel::C5, 0 },
    { Channel::C6, 1 },
    { Channel::C8, 2 },
    { Channel::C9, 3 },
    { Channel::C10, 4 },
    { Channel::C11, 5 },
    { Channel::C12, 6 },
    { Channel::C13, 7 },
    { Channel::C14, 8 },
};

const std::unordered_map<BprfParameter, std::size_t> UwbCapability::BprfParameterSetsBit = {
    { BprfParameter::Set1, 0 },
    { BprfParameter::Set2, 1 },
    { BprfParameter::Set3, 2 },
    { BprfParameter::Set4, 3 },
    { BprfParameter::Set5, 4 },
    { BprfParameter::Set6, 5 },
};

const std::unordered_map<HprfParameter, std::size_t> UwbCapability::HprfParameterSetsBit = {
    { HprfParameter::Set1, 0 },
    { HprfParameter::Set2, 1 },
    { HprfParameter::Set3, 2 },
    { HprfParameter::Set4, 3 },
    { HprfParameter::Set5, 4 },
    { HprfParameter::Set6, 5 },
    { HprfParameter::Set7, 6 },
    { HprfParameter::Set8, 7 },
    { HprfParameter::Set9, 8 },
    { HprfParameter::Set10, 9 },
    { HprfParameter::Set11, 10  },
    { HprfParameter::Set12, 11 },
    { HprfParameter::Set13, 12 },
    { HprfParameter::Set14, 13 },
    { HprfParameter::Set15, 14 },
    { HprfParameter::Set16, 15 },
    { HprfParameter::Set17, 16 },
    { HprfParameter::Set18, 17 },
    { HprfParameter::Set19, 18 },
    { HprfParameter::Set20, 19 },
    { HprfParameter::Set21, 20 },
    { HprfParameter::Set22, 21 },
    { HprfParameter::Set23, 22 },
    { HprfParameter::Set24, 23 },
    { HprfParameter::Set25, 24 },
    { HprfParameter::Set26, 25 },
    { HprfParameter::Set27, 26 },
    { HprfParameter::Set28, 27 },
    { HprfParameter::Set29, 28 },
    { HprfParameter::Set30, 29 },
    { HprfParameter::Set31, 30 },
    { HprfParameter::Set32, 31 },
    { HprfParameter::Set33, 32 },
    { HprfParameter::Set34, 33 },
    { HprfParameter::Set35, 34 },
};

std::unique_ptr<encoding::Tlv>
UwbCapability::ToOobDataObject() const
{
    std::vector<std::byte> data{};
    // TODO: perform actual conversion
    return std::make_unique<encoding::TlvSimple>(static_cast<std::byte>(Tag), data);
}

/* static */
UwbCapability
UwbCapability::FromOobDataObject(const encoding::Tlv& tlv)
{
    UwbCapability uwbCapability;
    // TODO: perform actual conversion
    return uwbCapability;
}
