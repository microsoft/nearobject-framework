
#include <bit>
#include <climits>
#include <iterator>
#include <stdexcept>

#include <notstd/utility.hxx>

#include <TlvSimple.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;

const std::array<UwbCapability::ParameterTag, 18> UwbCapability::ParameterTags = {
    UwbCapability::ParameterTag::FiraPhyVersionRange,
    UwbCapability::ParameterTag::FiraMacVersionRange,
    UwbCapability::ParameterTag::DeviceRoles,
    UwbCapability::ParameterTag::RangingMethod,
    UwbCapability::ParameterTag::StsConfig,
    UwbCapability::ParameterTag::MultiNodeMode,
    UwbCapability::ParameterTag::RangingMode,
    UwbCapability::ParameterTag::ScheduledMode,
    UwbCapability::ParameterTag::HoppingMode,
    UwbCapability::ParameterTag::BlockStriding,
    UwbCapability::ParameterTag::UwbInitiationTime,
    UwbCapability::ParameterTag::Channels,
    UwbCapability::ParameterTag::RFrameConfig,
    UwbCapability::ParameterTag::CcConstraintLength,
    UwbCapability::ParameterTag::BprfParameterSets,
    UwbCapability::ParameterTag::HprfParameterSets,
    UwbCapability::ParameterTag::AoaSupport,
    UwbCapability::ParameterTag::ExtendedMacAddress
};

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
 * @brief Get the Bytes Big Endian From std::size_t
 * 
 * @param value the bitmap to encode
 * @param desiredLength the desired number of bytes in the encoding, padding with zeros if necessary. 
 *                      If the value is too large, this will only encode the lowest desiredLength bytes
 * @return std::vector<uint8_t> 
 */
std::vector<uint8_t>
GetBytesBigEndianFromBitMap(std::size_t value, std::size_t desiredLength)
{
    if (desiredLength > sizeof value) {
        throw std::runtime_error("desired length exceeds std::size_t width, this is a bug!");
    }

    std::vector<uint8_t> bytes;

    for (auto i = 0; i < desiredLength; i++) {
        const auto b = static_cast<uint8_t>(value & 0xFFU);
        bytes.push_back(b);
        value >>= 8U;
    }

    return {
        std::make_move_iterator(std::rbegin(bytes)),
        std::make_move_iterator(std::rend(bytes))
    };
}

/**
 * @brief Get the Bytes Big Endian From std::size_t
 * 
 * @param value the number to encode in big endian
 * @param desiredLength the desired number of bytes in the encoding, padding with zeros if necessary. 
 *                      If the value is too large, this will only encode the lowest desiredLength bytes
 * @return std::vector<uint8_t> 
 */
std::vector<uint8_t>
GetBytesBigEndianFromSizeT(std::size_t value, std::size_t desiredLength)
{
    if (std::endian::native != std::endian::big) {
        value = value & 0x000000FF << 24 | value & 0x0000FF00 << 8 | value & 0x00FF0000 >> 8 | value & 0xFF000000 >> 24;
    }
    return GetBytesBigEndianFromBitMap(value, desiredLength);
}

/**
 * @brief Parses a span of bytes as a std::size_t number encoded in big endian
 * 
 * @param bytes 
 * @return std::size_t 
 */
std::size_t
ReadSizeTFromBytesBigEndian(std::span<const uint8_t> bytes)
{
    std::size_t rvalue = 0;

    if (bytes.size() >= sizeof rvalue)
        return 0; // TODO throw error? this isn't really part of an interface so may not be necessary

    for (int i = 0; i < bytes.size(); i++) {
        rvalue *= 0x100;
        rvalue += bytes[i];
    }
    return rvalue;
}

// TODO find a better place for this function
/**
 * @brief Get the Bit Mask From Bit Index object
 * 
 * @param bitIndex 
 * @return std::size_t 
 */
std::size_t
GetBitMaskFromBitIndex(std::size_t bitIndex)
{
    if (bitIndex >= sizeof(std::size_t) * CHAR_BIT) {
        return 0; // TODO throw error? this isn't really part of an interface so may not be necessary
    }
    return static_cast<std::size_t>(1UL) << bitIndex;
}

/**
 * @brief Get the Bit Index From Bit Mask object. 
 * 
 * @param bitMask 
 * @return std::size_t 
 */
std::size_t
GetBitIndexFromBitMask(std::size_t bitMask)
{
    for (int index = 0; index < (sizeof bitMask) * CHAR_BIT; index++) {
        if (bitMask == GetBitMaskFromBitIndex(index)) {
            return index;
        }
    }
    throw std::runtime_error("bit index not found");
}

/**
 * @brief lookup a value associated with a key from an unordered map. 
 *          TODO this function only exists because a good hash function has not been agreed upon yet
 * 
 * @tparam T 
 * @param bitIndexMap 
 * @param keyTarget 
 * @return std::size_t 
 */
template <class T>
std::size_t
unordered_map_lookup(const std::unordered_map<T, std::size_t>& bitIndexMap, const T keyTarget)
{
    for (auto [key, value] : bitIndexMap) {
        if (key == keyTarget) {
            return value;
        }
    }
    throw std::exception();
}

// TODO find a better place for this function
/**
 * @brief helper function to encode a given valueSet into a bitset according to bitIndexMap, using desiredLength bytes
 * 
 * @tparam T 
 * @param valueSet 
 * @param bitIndexMap 
 * @param desiredLength 
 * @return std::vector<uint8_t> 
 */
template <class T>
std::vector<uint8_t>
EncodeValuesAsBytes(const std::vector<T>& valueSet, const std::unordered_map<T, std::size_t>& bitIndexMap, std::size_t desiredLength)
{
    std::size_t valueSetEncoded = 0;
    for (const auto& value : valueSet) {
        auto bitIndex = unordered_map_lookup(bitIndexMap, value);
        valueSetEncoded |= GetBitMaskFromBitIndex(bitIndex);
    }
    return GetBytesBigEndianFromBitMap(valueSetEncoded, desiredLength);
}

// TODO find a better place for this function
/**
 * @brief helper function that writes the bitset for a given valueSet according to bitIndexMap, using desiredLength bytes
 * 
 * @tparam T 
 * @param assignee the destination of the bitset
 * @param bitIndexMap 
 * @param bytes 
 */
template <class T>
std::vector<T>
AssignValuesFromBytes(const std::unordered_map<T, std::size_t>& bitIndexMap, std::span<const uint8_t> bytes)
{
    std::vector<T> assignee;
    auto bitmasks = ReadSizeTFromBytesBigEndian(bytes);
    for (auto [key, value] : bitIndexMap) {
        if (bitmasks & GetBitMaskFromBitIndex(value)) {
            assignee.push_back(key);
        }
    }
    return assignee;
}

// TODO find a better place for this function
template <class T>
void
ToOobDataObjectHelper(encoding::TlvBer::Builder& builder, encoding::TlvBer::Builder& childbuilder, uint8_t tag, const std::vector<T>& valueSet, const std::unordered_map<T, std::size_t>& bitIndexMap, std::size_t desiredLength)
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
    builder.SetTag(UwbCapability::Tag);
    auto childbuilder = encoding::TlvBer::Builder();

    // Encode FiraPhyVersionRange
    {
        auto phyRange = GetBytesBigEndianFromSizeT(FiraPhyVersionRange, 4);
        auto phyRangeTlv = childbuilder.Reset()
                               .SetTag(notstd::to_underlying(ParameterTag::FiraPhyVersionRange))
                               .SetValue(phyRange)
                               .Build();
        builder.AddTlv(phyRangeTlv);
    }

    {
        auto macRange = GetBytesBigEndianFromSizeT(FiraMacVersionRange, 4);
        auto macRangeTlv = childbuilder.Reset()
                               .SetTag(std::size_t(ParameterTag::FiraMacVersionRange))
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
        std::size_t aoaEncoded = 0;
        for (auto value : AngleOfArrivalTypes) {
            // auto bitIndex = UwbCapability::AngleOfArrivalBit.at(value);
            auto bitIndex = unordered_map_lookup(UwbCapability::AngleOfArrivalBit, value);
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
    if (tlv.GetTag().size() != 1 || tlv.GetTag()[0] != UwbCapability::Tag) {
        throw UwbCapability::IncorrectTlvTag();
    }
    for (const auto& object : tlv.GetValues()) {
        if (object.GetTag().size() != 1) {
            throw UwbCapability::IncorrectNumberOfBytesInTagError();
        }
        switch (ParameterTag(object.GetTag()[0])) {
        case ParameterTag::FiraPhyVersionRange: {
            if (object.GetValue().size() != 4) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.FiraPhyVersionRange = ReadSizeTFromBytesBigEndian(object.GetValue()); // TODO verify that the bytes are indeed big endian
            break;
        }
        case ParameterTag::FiraMacVersionRange: {
            if (object.GetValue().size() != 4) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.FiraMacVersionRange = ReadSizeTFromBytesBigEndian(object.GetValue());
            break;
        }
        case ParameterTag::DeviceRoles: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.DeviceRoles = AssignValuesFromBytes(UwbCapability::DeviceRoleBit, object.GetValue());
            break;
        }
        case ParameterTag::RangingMethod: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.RangingConfigurations = AssignValuesFromBytes(UwbCapability::RangingConfigurationBit, object.GetValue());
            break;
        }
        case ParameterTag::StsConfig: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.StsConfigurations = AssignValuesFromBytes(UwbCapability::StsConfigurationBit, object.GetValue());
            break;
        }
        case ParameterTag::MultiNodeMode: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.MultiNodeModes = AssignValuesFromBytes(UwbCapability::MultiNodeModeBit, object.GetValue());
            break;
        }
        case ParameterTag::RangingMode: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.RangingTimeStructs = AssignValuesFromBytes(UwbCapability::RangingModeBit, object.GetValue());
            break;
        }
        case ParameterTag::ScheduledMode: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.SchedulingModes = AssignValuesFromBytes(UwbCapability::SchedulingModeBit, object.GetValue());
            break;
        }
        case ParameterTag::HoppingMode: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.HoppingMode = object.GetValue()[0];
            break;
        }
        case ParameterTag::BlockStriding: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.BlockStriding = object.GetValue()[0];
            break;
        }
        case ParameterTag::UwbInitiationTime: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.UwbInitiationTime = object.GetValue()[0];
            break;
        }
        case ParameterTag::Channels: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.Channels = AssignValuesFromBytes(UwbCapability::ChannelsBit, object.GetValue());
            break;
        }
        case ParameterTag::RFrameConfig: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.RFrameConfigurations = AssignValuesFromBytes(UwbCapability::RFrameConfigurationBit, object.GetValue());
            break;
        }
        case ParameterTag::CcConstraintLength: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.ConvolutionalCodeConstraintLengths = AssignValuesFromBytes(UwbCapability::ConvolutionalCodeConstraintLengthsBit, object.GetValue());
            break;
        }
        case ParameterTag::BprfParameterSets: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.BprfParameterSets = AssignValuesFromBytes(UwbCapability::BprfParameterSetsBit, object.GetValue());
            break;
        }
        case ParameterTag::HprfParameterSets: {
            if (object.GetValue().size() != 5) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.HprfParameterSets = AssignValuesFromBytes(UwbCapability::HprfParameterSetsBit, object.GetValue());
            break;
        }
        case ParameterTag::AoaSupport: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }

            uwbCapability.AngleOfArrivalTypes = AssignValuesFromBytes(UwbCapability::AngleOfArrivalBit, object.GetValue());
            uwbCapability.AngleOfArrivalFom = (object.GetValue()[0] & GetBitMaskFromBitIndex(UwbCapability::AngleOfArrivalFomBit));
            break;
        }
        case ParameterTag::ExtendedMacAddress: {
            if (object.GetValue().size() != 1) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.ExtendedMacAddress = object.GetValue()[0];
            break;
        }
        }
    }

    return uwbCapability;
}