
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
    { HprfParameter::Set11, 10 },
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

std::unique_ptr<encoding::TlvBer>
UwbCapability::ToOobDataObject() const
{
    //
    return nullptr;
}

uint32_t
ReadSizeTFromBytesBigEndian(std::span<const uint8_t> bytes)
{
    uint32_t rvalue = 0;

    if (bytes.size() >= 8)
        return 0; // TODO throw error? this isn't really part of an interface so may not be necessary

    for (int i = 0; i < bytes.size(); i++) {
        rvalue += bytes[i];
        rvalue <<= 8;
    }
    return rvalue;
}

// TODO find a better place for this function
size_t
GetBitMaskFromBitIndex(size_t bitIndex)
{
    if (bitIndex == 0) {
        return 1;
    }
    if (bitIndex >= 64) {
        return 0; // TODO throw error? this isn't really part of an interface so may not be necessary
    }
    return 1 << bitIndex;
}

// TODO find a better place for this function
template <class T>
void
AssignValuesFromBytes(std::vector<T>& assignee, const std::unordered_map<T, std::size_t>& bitmaskMap, std::span<const uint8_t> bytes)
{
    auto bitmasks = ReadSizeTFromBytesBigEndian(bytes);
    assignee.clear();
    for (auto iter : bitmaskMap) {
        if (bitmasks & GetBitMaskFromBitIndex(iter.second)) {
            assignee.push_back(iter.first);
        }
    }
}

/* static */
UwbCapability
UwbCapability::FromOobDataObject(const encoding::TlvBer& tlv)
{
    UwbCapability uwbCapability;
    if (tlv.IsPrimitive()) {
        throw UwbCapability::IncorrectTlvType();
    }
    for (const auto& object : tlv.GetValues()) {
        if (object.Tag.size() != 1) {
            throw UwbCapability::IncorrectNumberOfBytesInTagError();
        }
        switch (ParameterTag(object.Tag[0])) {
        case ParameterTag::FiraPhyVersionRange: {
            if (object.Value.size() != 4) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.FiraPhyVersionRange = ReadSizeTFromBytesBigEndian(object.Value); // TODO verify that the bytes are indeed big endian
            break;
        }
        case ParameterTag::FiraMacVersionRange: {
            if (object.Value.size() != 4) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.FiraMacVersionRange = ReadSizeTFromBytesBigEndian(object.Value);
            break;
        }
        case ParameterTag::DeviceRoles: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.DeviceRoles, UwbCapability::DeviceRoleBit, object.Value);
            break;
        }
        case ParameterTag::RangingMethod: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.RangingConfigurations, UwbCapability::RangingConfigurationBit, object.Value);
            break;
        }
        case ParameterTag::StsConfig: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.StsConfigurations, UwbCapability::StsConfigurationBit, object.Value);
            break;
        }
        case ParameterTag::MultiNodeMode: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.MultiNodeModes, UwbCapability::MultiNodeModeBit, object.Value);
            break;
        }
        case ParameterTag::RangingMode: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.RangingTimeStructs, UwbCapability::RangingModeBit, object.Value);
            break;
        }
        case ParameterTag::ScheduledMode: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.SchedulingModes, UwbCapability::SchedulingModeBit, object.Value);
            break;
        }
        case ParameterTag::HoppingMode: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            auto byte = object.Value[0];
            uwbCapability.HoppingMode = byte;
            break;
        }
        case ParameterTag::BlockStriding: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            auto byte = object.Value[0];
            uwbCapability.BlockStriding = byte;
            break;
        }
        case ParameterTag::UwbInitiationTime: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            auto byte = object.Value[0];
            uwbCapability.UwbInitiationTime = byte;
            break;
        }
        case ParameterTag::Channels: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.Channels, UwbCapability::ChannelsBit, object.Value);
            break;
        }
        case ParameterTag::RFrameConfig: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.RFrameConfigurations, UwbCapability::RFrameConfigurationBit, object.Value);
            break;
        }
        case ParameterTag::CcConstraintLength: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.ConvolutionalCodeConstraintLengths, UwbCapability::ConvolutionalCodeConstraintLengthsBit, object.Value);
            break;
        }
        case ParameterTag::BprfParameterSets: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.BprfParameterSets, UwbCapability::BprfParameterSetsBit, object.Value);
            break;
        }
        case ParameterTag::HprfParameterSets: {
            if (object.Value.size() != 5) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            AssignValuesFromBytes(uwbCapability.HprfParameterSets, UwbCapability::HprfParameterSetsBit, object.Value);
            break;
        }
        case ParameterTag::AoaSupport: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            auto byte = object.Value[0];
            break;
        }
        case ParameterTag::ExtendedMaxAddress: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            auto byte = object.Value[0];
            uwbCapability.ExtendedMacAddress = byte;
            break;
        }
        }
    }

    return uwbCapability;
}