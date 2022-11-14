
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

/**
 * @brief Get the Bytes Big Endian From size_t
 * 
 * @param value the value to encode in big endian
 * @param desiredLength the desired number of bytes in the encoding, padding with zeros if necessary. 
 *                      If the value is too large, this will only encode the lowest desiredLength bytes
 * @return std::span<const uint8_t> 
 */
std::span<const uint8_t>
GetBytesBigEndianFromSizeT(size_t value, int desiredLength)
{
    std::vector<uint8_t> bytes;

    size_t bytemask = 0xFF << (desiredLength * 8);

    for (int i = 0; i < desiredLength; i++) {
        bytes.push_back(bytemask & value);
        value <<= 8;
    }
    return std::span(std::cbegin(bytes), std::cend(bytes));
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

/**
 * @brief Get the Bit Index From Bit Mask object. 
 * 
 * @param bitMask 
 * @return size_t 
 */
size_t
GetBitIndexFromBitMask(size_t bitMask)
{
    for (int index = 0; index < 64; index++) {
        if (bitMask == GetBitMaskFromBitIndex(index)) {
            return index;
        }
    }
}

// TODO find a better place for this function
template <class T>
std::span<const uint8_t>
EncodeValuesAsBytes(const std::vector<T>& valueSet, const std::unordered_map<T, std::size_t>& bitIndexMap, int desiredLength)
{
    size_t valueSetEncoded = 0;
    for (auto value : valueSet) {
        auto bitIndex = bitIndexMap.at(value);
        valueSetEncoded = GetBitMaskFromBitIndex(bitIndex);
    }
    return GetBytesBigEndianFromSizeT(valueSetEncoded, desiredLength);
}

// TODO find a better place for this function
template <class T>
void
AssignValuesFromBytes(std::vector<T>& assignee, const std::unordered_map<T, std::size_t>& bitIndexMap, std::span<const uint8_t> bytes)
{
    auto bitmasks = ReadSizeTFromBytesBigEndian(bytes);
    assignee.clear();
    for (auto iter : bitIndexMap) {
        if (bitmasks & GetBitMaskFromBitIndex(iter.second)) {
            assignee.push_back(iter.first);
        }
    }
}

// TODO find a better place for this function
template <class T>
void
ToOobDataObjectHelper(encoding::TlvBer::Builder& builder, encoding::TlvBer::Builder& childbuilder, uint8_t tag, const std::vector<T>& valueSet, const std::unordered_map<T, std::size_t>& bitIndexMap, int desiredLength)
{
    auto bytes = EncodeValuesAsBytes(valueSet, bitIndexMap, desiredLength);
    auto tlv = childbuilder.Reset()
                   .SetTag(tag)
                   .SetValue(bytes)
                   .Build();
    builder.AddTlv(tlv);
}

std::unique_ptr<encoding::TlvBer>
UwbCapability::ToOobDataObject() const
{
    auto returnTlvBer = std::make_unique<encoding::TlvBer>();
    auto builder = encoding::TlvBer::Builder();
    builder.Reset()
        .SetTag(UwbCapability::Tag);
    auto childbuilder = encoding::TlvBer::Builder();

    {
        auto phyRange = GetBytesBigEndianFromSizeT(FiraPhyVersionRange, 4);
        auto phyRangeTlv = childbuilder.Reset()
                               .SetTag(size_t(ParameterTag::FiraPhyVersionRange))
                               .SetValue(phyRange)
                               .Build();
        builder.AddTlv(phyRangeTlv);
    }

    {
        auto macRange = GetBytesBigEndianFromSizeT(FiraMacVersionRange, 4);
        auto macRangeTlv = childbuilder.Reset()
                               .SetTag(size_t(ParameterTag::FiraMacVersionRange))
                               .SetValue(macRange)
                               .Build();
        builder.AddTlv(macRangeTlv);
    }

    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::DeviceRoles), DeviceRoles, UwbCapability::DeviceRoleBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::RangingMethod), RangingConfigurations, UwbCapability::RangingConfigurationBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::StsConfig), StsConfigurations, UwbCapability::StsConfigurationBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::MultiNodeMode), MultiNodeModes, UwbCapability::MultiNodeModeBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::RangingMode), RangingTimeStructs, UwbCapability::RangingModeBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::ScheduledMode), SchedulingModes, UwbCapability::SchedulingModeBit, 1);
    {
        auto hoppingtlv = childbuilder.Reset()
                              .SetTag(uint8_t(ParameterTag::HoppingMode))
                              .SetValue(HoppingMode)
                              .Build();
        builder.AddTlv(hoppingtlv);
    }

    {
        auto blocktlv = childbuilder.Reset()
                            .SetTag(uint8_t(ParameterTag::BlockStriding))
                            .SetValue(BlockStriding)
                            .Build();
        builder.AddTlv(blocktlv);
    }

    {
        auto uwbtlv = childbuilder.Reset()
                          .SetTag(uint8_t(ParameterTag::UwbInitiationTime))
                          .SetValue(UwbInitiationTime)
                          .Build();
        builder.AddTlv(uwbtlv);
    }

    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::Channels), Channels, UwbCapability::ChannelsBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::RFrameConfig), RFrameConfigurations, UwbCapability::RFrameConfigurationBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::CcConstraintLength), ConvolutionalCodeConstraintLengths, UwbCapability::ConvolutionalCodeConstraintLengthsBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::BprfParameterSets), BprfParameterSets, UwbCapability::BprfParameterSetsBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, uint8_t(ParameterTag::HprfParameterSets), HprfParameterSets, UwbCapability::HprfParameterSetsBit, 5);

    {
        size_t aoaEncoded = 0;
        for (auto value : AngleOfArrivalTypes) {
            auto bitIndex = UwbCapability::AngleOfArrivalBit.at(value);
            aoaEncoded |= GetBitMaskFromBitIndex(bitIndex);
        }
        if (AngleOfArrivalFom) {
            aoaEncoded |= GetBitMaskFromBitIndex(UwbCapability::AngleOfArrivalFomBit);
        }

        auto aoaByte = GetBytesBigEndianFromSizeT(aoaEncoded, 1);
        auto aoatlv = childbuilder.Reset()
                          .SetTag(uint8_t(ParameterTag::AoaSupport))
                          .SetValue(aoaByte)
                          .Build();
        builder.AddTlv(aoatlv);
    }

    {
        auto mactlv = childbuilder.Reset()
                          .SetTag(uint8_t(ParameterTag::ExtendedMacAddress))
                          .SetValue(ExtendedMacAddress)
                          .Build();
        builder.AddTlv(mactlv);
    }

    *returnTlvBer = builder.Build();
    return returnTlvBer;
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
            uwbCapability.HoppingMode = object.Value[0];
            break;
        }
        case ParameterTag::BlockStriding: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.BlockStriding = object.Value[0];
            break;
        }
        case ParameterTag::UwbInitiationTime: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.UwbInitiationTime = object.Value[0];
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

            AssignValuesFromBytes(uwbCapability.AngleOfArrivalTypes, UwbCapability::AngleOfArrivalBit, object.Value);
            uwbCapability.AngleOfArrivalFom = (object.Value[0] | GetBitMaskFromBitIndex(UwbCapability::AngleOfArrivalFomBit));
            break;
        }
        case ParameterTag::ExtendedMacAddress: {
            if (object.Value.size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.ExtendedMacAddress = object.Value[0];
            break;
        }
        }
    }

    return uwbCapability;
}