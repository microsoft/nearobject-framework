
#include <algorithm>
#include <bit>
#include <climits>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <notstd/utility.hxx>

#include <tlv/TlvSerialize.hxx>
#include <tlv/TlvSimple.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;

const std::initializer_list<RangingMethod> UwbCapability::RangingMethodsDefault = {
    RangingMethod{ RangingDirection::OneWay, MeasurementReportMode::None },
    RangingMethod{ RangingDirection::SingleSidedTwoWay, MeasurementReportMode::Deferred },
    RangingMethod{ RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::Deferred },
    RangingMethod{ RangingDirection::SingleSidedTwoWay, MeasurementReportMode::NonDeferred },
    RangingMethod{ RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::NonDeferred },
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

const std::unordered_map<RangingMethod, std::size_t> UwbCapability::RangingMethodBit = {
    { { RangingDirection::OneWay, MeasurementReportMode::None }, 0 },
    { { RangingDirection::SingleSidedTwoWay, MeasurementReportMode::Deferred }, 1 },
    { { RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::Deferred }, 2 },
    { { RangingDirection::SingleSidedTwoWay, MeasurementReportMode::NonDeferred }, 3 },
    { { RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::NonDeferred }, 4 },
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
    { Channel::C12, 5 },
    { Channel::C13, 6 },
    { Channel::C14, 7 },
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

// TODO find a better place for this function
template <class T>
void
ToOobDataObjectHelper(encoding::TlvBer::Builder& builder, encoding::TlvBer::Builder& childbuilder, uint8_t tag, const std::vector<T>& valueSet, const std::unordered_map<T, std::size_t>& bitIndexMap, std::size_t desiredLength)
{
    auto bytes = encoding::EncodeValuesAsBytes(valueSet, bitIndexMap, desiredLength);
    auto tlv = childbuilder.Reset()
                   .SetTag(tag)
                   .SetValue(bytes)
                   .Build();
    builder.AddTlv(tlv);
}

std::string
UwbCapability::ToString() const
{
    std::ostringstream ss;
    // TODO
    return ss.str();
}

std::unique_ptr<encoding::TlvBer>
UwbCapability::ToOobDataObject() const
{
    using encoding::GetBitMaskFromBitIndex, encoding::GetBytesBigEndianFromBitMap;

    auto returnTlvBer = std::make_unique<encoding::TlvBer>();
    auto builder = encoding::TlvBer::Builder();
    builder.SetTag(UwbCapability::Tag);
    auto childbuilder = encoding::TlvBer::Builder();

    // Encode FiraPhyVersionRange
    {
        auto phyRange = GetBytesBigEndianFromBitMap(FiraPhyVersionRange, 4);
        auto phyRangeTlv = childbuilder.Reset()
                               .SetTag(notstd::to_underlying(ParameterTag::FiraPhyVersionRange))
                               .SetValue(phyRange)
                               .Build();
        builder.AddTlv(phyRangeTlv);
    }

    {
        auto macRange = GetBytesBigEndianFromBitMap(FiraMacVersionRange, 4);
        auto macRangeTlv = childbuilder.Reset()
                               .SetTag(std::size_t(ParameterTag::FiraMacVersionRange))
                               .SetValue(macRange)
                               .Build();
        builder.AddTlv(macRangeTlv);
    }

    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::DeviceRoles), DeviceRoles, UwbCapability::DeviceRoleBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::RangingMethod), RangingMethods, UwbCapability::RangingMethodBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::StsConfig), StsConfigurations, UwbCapability::StsConfigurationBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::MultiNodeMode), MultiNodeModes, UwbCapability::MultiNodeModeBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::RangingMode), RangingTimeStructs, UwbCapability::RangingModeBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::ScheduledMode), SchedulingModes, UwbCapability::SchedulingModeBit, 1);
    {
        auto hoppingtlv = childbuilder.Reset()
                              .SetTag(notstd::to_underlying(ParameterTag::HoppingMode))
                              .SetValue(HoppingMode)
                              .Build();
        builder.AddTlv(hoppingtlv);
    }

    {
        auto blocktlv = childbuilder.Reset()
                            .SetTag(notstd::to_underlying(ParameterTag::BlockStriding))
                            .SetValue(BlockStriding)
                            .Build();
        builder.AddTlv(blocktlv);
    }

    {
        auto uwbtlv = childbuilder.Reset()
                          .SetTag(notstd::to_underlying(ParameterTag::UwbInitiationTime))
                          .SetValue(UwbInitiationTime)
                          .Build();
        builder.AddTlv(uwbtlv);
    }

    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::Channels), Channels, UwbCapability::ChannelsBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::RFrameConfig), RFrameConfigurations, UwbCapability::RFrameConfigurationBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::CcConstraintLength), ConvolutionalCodeConstraintLengths, UwbCapability::ConvolutionalCodeConstraintLengthsBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::BprfParameterSets), BprfParameterSets, UwbCapability::BprfParameterSetsBit, 1);
    ToOobDataObjectHelper(builder, childbuilder, notstd::to_underlying(ParameterTag::HprfParameterSets), HprfParameterSets, UwbCapability::HprfParameterSetsBit, 5);

    {
        std::size_t aoaEncoded = 0;
        for (const auto& value : AngleOfArrivalTypes) {
            auto bitIndex = AngleOfArrivalBit.at(value);
            aoaEncoded |= GetBitMaskFromBitIndex(bitIndex);
        }
        if (AngleOfArrivalFom) {
            aoaEncoded |= GetBitMaskFromBitIndex(UwbCapability::AngleOfArrivalFomBit);
        }

        auto aoaByte = GetBytesBigEndianFromBitMap(aoaEncoded, 1);
        auto aoatlv = childbuilder.Reset()
                          .SetTag(notstd::to_underlying(ParameterTag::AoaSupport))
                          .SetValue(aoaByte)
                          .Build();
        builder.AddTlv(aoatlv);
    }

    {
        auto mactlv = childbuilder.Reset()
                          .SetTag(notstd::to_underlying(ParameterTag::ExtendedMacAddress))
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
    using encoding::ReadSizeTFromBytesBigEndian, encoding::AssignValuesFromBytes, encoding::GetBitMaskFromBitIndex;

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
            uwbCapability.FiraPhyVersionRange = ReadSizeTFromBytesBigEndian<uint32_t>(object.GetValue()); // TODO verify that the bytes are indeed big endian
            break;
        }
        case ParameterTag::FiraMacVersionRange: {
            if (object.GetValue().size() != 4) {
                throw UwbCapability::IncorrectNumberOfBytesInValueError();
            }
            uwbCapability.FiraMacVersionRange = ReadSizeTFromBytesBigEndian<uint32_t>(object.GetValue());
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
            uwbCapability.RangingMethods = AssignValuesFromBytes(UwbCapability::RangingMethodBit, object.GetValue());
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

namespace detail
{
// TODO: move these to common utility code, possibly notstd lib
template <class T>
bool
leftIsSubset(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
    return std::ranges::all_of(lhs, [&rhs](const auto& elem) {
        return std::ranges::any_of(rhs, [&elem](const auto& elem2) {
            return elem == elem2;
        });
    });
}

template <class T>
bool
leftUnorderedEquals(const std::vector<T>& lhs, const std::vector<T>& rhs)
{
    return leftIsSubset(lhs, rhs) and leftIsSubset(rhs, lhs);
}
} // namespace detail

bool
uwb::protocol::fira::operator==(const UwbCapability& lhs, const UwbCapability& rhs) noexcept
{
    const auto haveSameContents = [&](const auto& v1, const auto& v2) -> bool {
        return ::detail::leftUnorderedEquals(v1, v2);
    };

    const bool basicFieldsEqual =
        std::tie(lhs.FiraPhyVersionRange, lhs.FiraMacVersionRange, lhs.ExtendedMacAddress, lhs.UwbInitiationTime, lhs.AngleOfArrivalFom, lhs.BlockStriding, lhs.HoppingMode) ==
        std::tie(rhs.FiraPhyVersionRange, rhs.FiraMacVersionRange, rhs.ExtendedMacAddress, rhs.UwbInitiationTime, rhs.AngleOfArrivalFom, rhs.BlockStriding, rhs.HoppingMode);

    // clang-format off
    return basicFieldsEqual
        && haveSameContents(lhs.MultiNodeModes, rhs.MultiNodeModes)
        && haveSameContents(lhs.DeviceRoles, rhs.DeviceRoles)
        && haveSameContents(lhs.StsConfigurations, rhs.StsConfigurations)
        && haveSameContents(lhs.RFrameConfigurations, rhs.RFrameConfigurations)
        && haveSameContents(lhs.AngleOfArrivalTypes, rhs.AngleOfArrivalTypes)
        && haveSameContents(lhs.SchedulingModes, rhs.SchedulingModes)
        && haveSameContents(lhs.RangingTimeStructs, rhs.RangingTimeStructs)
        && haveSameContents(lhs.RangingMethods, rhs.RangingMethods)
        && haveSameContents(lhs.ConvolutionalCodeConstraintLengths, rhs.ConvolutionalCodeConstraintLengths)
        && haveSameContents(lhs.Channels, rhs.Channels)
        && haveSameContents(lhs.BprfParameterSets, rhs.BprfParameterSets)
        && haveSameContents(lhs.HprfParameterSets, rhs.HprfParameterSets);
    // clang-format on
}

bool
uwb::protocol::fira::operator!=(const UwbCapability& lhs, const UwbCapability& rhs) noexcept
{
    return !(lhs == rhs);
}
