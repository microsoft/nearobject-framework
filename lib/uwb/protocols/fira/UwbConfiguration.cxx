
#include <cstring>
#include <functional>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include <variant>

#include <magic_enum.hpp>
#include <notstd/type_traits.hxx>

#include <tlv/TlvBer.hxx>
#include <tlv/TlvSerialize.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace uwb::protocol::fira;

/* static */
UwbConfiguration::Builder
UwbConfiguration::Create() noexcept
{
    return UwbConfiguration::Builder();
}

/* static */
const std::unordered_set<ResultReportConfiguration> UwbConfiguration::ResultReportConfigurationsDefault = {
    ResultReportConfiguration::AoAAzimuthReport
};

std::unique_ptr<encoding::TlvBer>
UwbConfiguration::ToDataObject() const
{
    using encoding::TlvBer, encoding::GetBytesBigEndianFromBitMap;

    auto builder = TlvBer::Builder().SetTag(Tag);

    for (const auto& [parameterTag, parameterValueVariant] : m_values) {
        auto valueBuilder = TlvBer::Builder().SetTag(notstd::to_underlying(parameterTag));
        std::visit([&valueBuilder, &parameterTag](auto&& parameterValue) {
            using ParameterValueT = std::decay_t<decltype(parameterValue)>;
            if constexpr (std::is_enum_v<ParameterValueT>) {
                valueBuilder.SetValue(notstd::to_underlying(parameterValue));
            } else if constexpr (std::is_same_v<ParameterValueT, uint8_t> || std::is_same_v<ParameterValueT, std::array<uint8_t, StaticStsInitializationVectorLength>>) {
                valueBuilder.SetValue(parameterValue);
            } else if constexpr (std::is_unsigned_v<ParameterValueT>) {
                auto valueBytes = GetBytesBigEndianFromBitMap(parameterValue, sizeof parameterValue);
                valueBuilder.SetValue(valueBytes);
            } else if constexpr (std::is_same_v<ParameterValueT, ::uwb::UwbMacAddress>) {
                auto valueBytes = parameterValue.GetValue();
                valueBuilder.SetValue(valueBytes);
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<::uwb::UwbMacAddress>>) {
                std::vector<uint8_t> valueBytes{};
                for (const auto& uwbMacAddress : parameterValue) {
                    auto value = parameterValue.GetValue();
                    std::ranges::copy(valueBytes, std::back_inserter(value));
                }
                valueBuilder.SetValue(valueBytes);
            } else if constexpr (std::is_same_v<ParameterValueT, std::unordered_set<ResultReportConfiguration>>) {
                uint8_t value = 0;
                for (const auto& resultReportConfiguration : parameterValue) {
                    value |= notstd::to_underlying(resultReportConfiguration);
                }
                valueBuilder.SetValue(value);
            }
        },
            parameterValueVariant);
        builder.AddTlv(valueBuilder.Build());
    }

    auto tlvBerResult = std::make_unique<TlvBer>();
    *tlvBerResult = builder.Build();

    return tlvBerResult;
}

namespace detail
{
template <typename EnumT>
void
SetEnumValue(uint8_t parameterValue, std::function<void(EnumT)> setter)
{
    auto valueEnum = magic_enum::enum_cast<EnumT>(parameterValue);
    if (valueEnum.has_value()) {
        setter(*valueEnum);
    }
}
} // namespace detail

/* static */
UwbConfiguration
UwbConfiguration::FromDataObject(const encoding::TlvBer& tlvBer)
{
    UwbConfiguration::Builder builder{};

    // clang-format off
    // setter map for 8-bit values
    static const std::unordered_map<ParameterTag, std::function<void(uint8_t)>> setters8 {
        { ParameterTag::KeyRotationRate, [&](uint8_t value) { builder.SetKeyRotationRate(value); }},
        { ParameterTag::MaxContentionPhaseLength, [&](uint8_t value) { builder.SetMaxContentionPhaseLength(value); }},
        { ParameterTag::PreambleCodeIndex, [&](uint8_t value) { builder.SetPreambleCodeIndex(value); }},
        { ParameterTag::SlotsPerRr, [&](uint8_t value) { builder.SetMaxSlotsPerRangingRound(value); }},
        { ParameterTag::Sp0PhySetNumber, [&](uint8_t value) { builder.SetSp0PhySetNumber(value); }},
        { ParameterTag::Sp1PhySetNumber, [&](uint8_t value) { builder.SetSp1PhySetNumber(value); }},
        { ParameterTag::Sp3PhySetNumber, [&](uint8_t value) { builder.SetSp3PhySetNumber(value); }},
    };
    // setter map for 16-bit values
    static const std::unordered_map<ParameterTag, std::function<void(uint16_t)>> setters16 {
        { ParameterTag::FiraMacVersion, [&](uint16_t value){ builder.SetFiraVersionMac(value); }},
        { ParameterTag::FiraPhyVersion, [&](uint16_t value){ builder.SetFiraVersionPhy(value); }},
        { ParameterTag::MaxRrRetry, [&](uint16_t value){ builder.SetMaxRangingRoundRetry(value); }},
        { ParameterTag::RangingInterval, [&](uint16_t value){ builder.SetRangingInterval(value); }},
        { ParameterTag::SlotDuration, [&](uint16_t value){ builder.SetSlotDuration(value); }},
    };
    // clang-format on

    std::vector<encoding::TlvBer> tlvBerValues = tlvBer.GetValues();

    for (const auto& tlvBerValue : tlvBerValues) {
        auto tagValue = tlvBerValue.GetTag();
        // All tags for UwbConfiguration are 1-byte long, so ignore all others.
        if (std::size(tagValue) != 1) {
            continue;
        }

        // Ensure the tag has a corresponding enumeration value.
        auto parameterTag = magic_enum::enum_cast<ParameterTag>(tagValue.front());
        if (!parameterTag.has_value()) {
            continue;
        }

        // Ensure all values have non-zero payload.
        auto& parameterValue = tlvBerValue.GetValue();
        if (std::empty(parameterValue)) {
            continue;
        }

        switch (*parameterTag) {
        // boolean values
        case ParameterTag::HoppingMode: {
            bool value = !!parameterValue.front();
            builder.SetHoppingMode(value);
            break;
        }
        case ParameterTag::BlockStriding: {
            bool value = !!parameterValue.front();
            builder.SetBlockStriding(value);
            break;
        }

        // 8-bit values
        case ParameterTag::KeyRotationRate:
        case ParameterTag::MaxContentionPhaseLength:
        case ParameterTag::SlotsPerRr:
        case ParameterTag::PreambleCodeIndex:
        case ParameterTag::Sp0PhySetNumber:
        case ParameterTag::Sp1PhySetNumber:
        case ParameterTag::Sp3PhySetNumber: {
            uint8_t value = parameterValue.front();
            setters8.at(*parameterTag)(value);
            break;
        }

        // 16-bit values
        case ParameterTag::FiraPhyVersion:
        case ParameterTag::FiraMacVersion: {
        case ParameterTag::SlotDuration:
        case ParameterTag::RangingInterval:
        case ParameterTag::MaxRrRetry:
            uint16_t value = 0; // assumes endianness matches host
            std::memcpy(&value, std::data(parameterValue), std::size(parameterValue));
            setters16.at(*parameterTag)(value);
            break;
        }

        // 32-bit values
        case ParameterTag::UwbInitiationTime: {
            uint32_t value = 0; // assumes endianness matches host
            std::memcpy(&value, std::data(parameterValue), std::size(parameterValue));
            builder.SetUwbInitiationTime(value);
            break;
        }

        // enumerations
        case ParameterTag::DeviceRole: {
            ::detail::SetEnumValue<DeviceRole>(parameterValue.front(), [&](auto&& value) {
                builder.SetDeviceRole(value);
            });
            break;
        }
        case ParameterTag::StsConfig: {
            ::detail::SetEnumValue<StsConfiguration>(parameterValue.front(), [&](auto&& value) {
                builder.SetStsConfiguration(value);
            });
            break;
        }
        case ParameterTag::MultiNodeMode: {
            ::detail::SetEnumValue<MultiNodeMode>(parameterValue.front(), [&](auto&& value) {
                builder.SetMultiNodeMode(value);
            });
            break;
        }
        case ParameterTag::RangingTimeStruct: {
            ::detail::SetEnumValue<RangingMode>(parameterValue.front(), [&](auto&& value) {
                builder.SetRangingTimeStruct(value);
            });
            break;
        }
        case ParameterTag::ScheduledMode: {
            ::detail::SetEnumValue<SchedulingMode>(parameterValue.front(), [&](auto&& value) {
                builder.SetSchedulingMode(value);
            });
            break;
        }
        case ParameterTag::PrfMode: {
            ::detail::SetEnumValue<PrfMode>(parameterValue.front(), [&](auto&& value) {
                builder.SetPrfMode(value);
            });
            break;
        }
        case ParameterTag::RFrameConfig: {
            ::detail::SetEnumValue<StsPacketConfiguration>(parameterValue.front(), [&](auto&& value) {
                builder.SetStsPacketConfiguration(value);
            });
            break;
        }
        case ParameterTag::ChannelNumber: {
            ::detail::SetEnumValue<Channel>(parameterValue.front(), [&](auto&& value) {
                builder.SetChannel(value);
            });
            break;
        }
        case ParameterTag::CcConstraintLength: {
            ::detail::SetEnumValue<ConvolutionalCodeConstraintLength>(parameterValue.front(), [&](auto&& value) {
                builder.SetConvolutionalCodeConstraintLength(value);
            });
            break;
        }
        case ParameterTag::MacAddressMode: {
            ::detail::SetEnumValue<uwb::UwbMacAddressType>(parameterValue.front(), [&](auto&& value) {
                builder.SetMacAddressType(value);
            });
            break;
        }
        case ParameterTag::MacFcsType: {
            ::detail::SetEnumValue<uwb::UwbMacAddressFcsType>(parameterValue.front(), [&](auto&& value) {
                builder.SetMacAddressFcsType(value);
            });
            break;
        }

        // special cases
        case ParameterTag::RangingMethod: {
            uint8_t value = parameterValue.front();
            RangingDirection rangingDirection;
            MeasurementReportMode measurementReportMode;

            switch (value) {
            case 0:
                rangingDirection = RangingDirection::OneWay;
                measurementReportMode = MeasurementReportMode::None;
                break;
            case (int)RangingRoundUsage::SingleSidedTwoWayRangingWithDeferredMode:
                rangingDirection = RangingDirection::SingleSidedTwoWay;
                measurementReportMode = MeasurementReportMode::Deferred;
                break;
            case (int)RangingRoundUsage::DoubleSidedTwoWayRangingWithDeferredMode:
                rangingDirection = RangingDirection::DoubleSidedTwoWay;
                measurementReportMode = MeasurementReportMode::Deferred;
                break;
            case (int)RangingRoundUsage::SingleSidedTwoWayRangingNonDeferredMode:
                rangingDirection = RangingDirection::SingleSidedTwoWay;
                measurementReportMode = MeasurementReportMode::NonDeferred;
                break;
            case (int)RangingRoundUsage::DoubleSidedTwoWayRangingNonDeferredMode:
                rangingDirection = RangingDirection::DoubleSidedTwoWay;
                measurementReportMode = MeasurementReportMode::NonDeferred;
                break;
            }

            RangingMethod rangingMethod(rangingDirection, measurementReportMode);
            builder.SetRangingMethod(rangingMethod);
            break;
        }

        case ParameterTag::ResultReportConfig: {
            uint8_t value = parameterValue.front();
            for (const auto resultReportConfiguration : magic_enum::enum_values<ResultReportConfiguration>()) {
                if ((value & notstd::to_underlying(resultReportConfiguration)) != 0) {
                    builder.AddResultReportConfiguration(resultReportConfiguration);
                }
            }
            break;
        }
        case ParameterTag::ControleeShortMacAddress: {
            std::array<uint8_t, 2> addressData{ parameterValue[0], parameterValue[1] };
            uwb::UwbMacAddress uwbMacAddressShort(addressData);
            builder.SetMacAddressControleeShort(uwbMacAddressShort);
            break;
        }
        case ParameterTag::ControllerMacAddress: {
            std::optional<uwb::UwbMacAddress> uwbMacAddress;
            switch (std::size(parameterValue)) {
            case uwb::UwbMacAddress::ShortLength: {
                std::array<uint8_t, uwb::UwbMacAddress::ShortLength> addressData{ parameterValue[0], parameterValue[1] };
                uwbMacAddress = UwbMacAddress(addressData);
                break;
            }
            case uwb::UwbMacAddress::ExtendedLength: {
                std::array<uint8_t, uwb::UwbMacAddress::ExtendedLength> addressData{ 
                    parameterValue[0], parameterValue[1], parameterValue[2], parameterValue[3], 
                    parameterValue[4], parameterValue[5], parameterValue[6], parameterValue[7] };
                uwbMacAddress = UwbMacAddress(addressData);
                break;
            }
            default: {
                break;
            }
            }
            if (uwbMacAddress.has_value()) {
                builder.SetMacAddressController(uwbMacAddress.value());
            }
            break;
        }
        default:
            break;
        } // switch (parameterTag)
    }

    return builder;
}

std::optional<uint16_t>
UwbConfiguration::GetFiraPhyVersion() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::FiraPhyVersion);
}

std::optional<uint16_t>
UwbConfiguration::GetFiraMacVersion() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::FiraMacVersion);
}

std::optional<DeviceRole>
UwbConfiguration::GetDeviceRole() const noexcept
{
    return GetValue<DeviceRole>(ParameterTag::DeviceRole);
}

std::optional<RangingMethod>
UwbConfiguration::GetRangingMethod() const noexcept
{
    return GetValue<RangingMethod>(ParameterTag::RangingMethod);
}

std::optional<StsConfiguration>
UwbConfiguration::GetStsConfiguration() const noexcept
{
    return GetValue<StsConfiguration>(ParameterTag::StsConfig);
}

std::optional<MultiNodeMode>
UwbConfiguration::GetMultiNodeMode() const noexcept
{
    return GetValue<MultiNodeMode>(ParameterTag::MultiNodeMode);
}

std::optional<RangingMode>
UwbConfiguration::GetRangingTimeStruct() const noexcept
{
    return GetValue<RangingMode>(ParameterTag::RangingTimeStruct);
}

std::optional<SchedulingMode>
UwbConfiguration::GetSchedulingMode() const noexcept
{
    return GetValue<SchedulingMode>(ParameterTag::ScheduledMode);
}

std::optional<bool>
UwbConfiguration::GetHoppingMode() const noexcept
{
    return GetValue<bool>(ParameterTag::HoppingMode);
}

std::optional<bool>
UwbConfiguration::GetBlockStriding() const noexcept
{
    return GetValue<bool>(ParameterTag::BlockStriding);
}

std::optional<uint32_t>
UwbConfiguration::GetUwbInitiationTime() const noexcept
{
    return GetValue<uint32_t>(ParameterTag::UwbInitiationTime);
}

std::optional<Channel>
UwbConfiguration::GetChannel() const noexcept
{
    return GetValue<Channel>(ParameterTag::ChannelNumber);
}

std::optional<StsPacketConfiguration>
UwbConfiguration::GetRFrameConfig() const noexcept
{
    return GetValue<StsPacketConfiguration>(ParameterTag::RFrameConfig);
}

std::optional<ConvolutionalCodeConstraintLength>
UwbConfiguration::GetConvolutionalCodeConstraintLength() const noexcept
{
    return GetValue<ConvolutionalCodeConstraintLength>(ParameterTag::CcConstraintLength);
}

std::optional<PrfMode>
UwbConfiguration::GetPrfMode() const noexcept
{
    return GetValue<PrfMode>(ParameterTag::PrfMode);
}

std::optional<uint8_t>
UwbConfiguration::GetSp0PhySetNumber() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::Sp0PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetSp1PhySetNumber() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::Sp1PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetSp3PhySetNumber() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::Sp3PhySetNumber);
}

std::optional<uint8_t>
UwbConfiguration::GetPreambleCodeIndex() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::PreambleCodeIndex);
}

std::unordered_set<ResultReportConfiguration>
UwbConfiguration::GetResultReportConfigurations() const noexcept
{
    auto it = m_values.find(ParameterTag::ResultReportConfig);
    return (it != std::cend(m_values))
        ? std::get<std::unordered_set<ResultReportConfiguration>>(it->second)
        : std::unordered_set<ResultReportConfiguration>{};
}

std::optional<uwb::UwbMacAddressType>
UwbConfiguration::GetMacAddressMode() const noexcept
{
    return GetValue<uwb::UwbMacAddressType>(ParameterTag::MacAddressMode);
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControleeShortMacAddress() const noexcept
{
    return GetValue<uwb::UwbMacAddress>(ParameterTag::ControleeShortMacAddress);
}

std::optional<uwb::UwbMacAddress>
UwbConfiguration::GetControllerMacAddress() const noexcept
{
    return GetValue<uwb::UwbMacAddress>(ParameterTag::ControllerMacAddress);
}

std::optional<uint8_t>
UwbConfiguration::GetSlotsPerRangingRound() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::SlotsPerRr);
}

std::optional<uint8_t>
UwbConfiguration::GetMaxContentionPhaseLength() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::MaxContentionPhaseLength);
}

std::optional<uint16_t>
UwbConfiguration::GetSlotDuration() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::SlotDuration);
}

std::optional<uint16_t>
UwbConfiguration::GetRangingInterval() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::RangingInterval);
}

std::optional<uint8_t>
UwbConfiguration::GetKeyRotationRate() const noexcept
{
    return GetValue<uint8_t>(ParameterTag::KeyRotationRate);
}

std::optional<uwb::UwbMacAddressFcsType>
UwbConfiguration::GetMacAddressFcsType() const noexcept
{
    return GetValue<uwb::UwbMacAddressFcsType>(ParameterTag::MacFcsType);
}

std::optional<uint16_t>
UwbConfiguration::GetMaxRangingRoundRetry() const noexcept
{
    return GetValue<uint16_t>(ParameterTag::MaxRrRetry);
}

const std::unordered_map<UwbConfiguration::ParameterTag, UwbConfiguration::ParameterTypesVariant>&
UwbConfiguration::GetValueMap() const noexcept
{
    return m_values;
}
