
#include <uwb/protocols/fira/FiraDevice.hxx>

#include <numeric>
#include <sstream>
#include <string_view>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

#include <magic_enum.hpp>
#include <notstd/tostring.hxx>

using namespace uwb::protocol::fira;
using namespace strings::ostream_operators;

std::string
uwb::protocol::fira::VersionToString(uint32_t input) noexcept
{
    auto first_byte = input & 0xFFU;
    auto second_byte = (input >> 8) & 0xFFU;
    std::ostringstream ss;
    ss << first_byte << "." << second_byte;
    return ss.str();
}

/**
 * @brief split a string into tokens with the specified delimiter
 * TODO find better place for this
 *
 * @param input
 * @param delimiter
 * @return std::vector<std::string>
 */
std::vector<std::string>
tokenize(const std::string& input, char delimiter)
{
    std::vector<std::string> output;
    std::istringstream ss{ input };
    while (ss.good()) {
        std::string a;
        getline(ss, a, delimiter);
        output.push_back(a);
    }
    return output;
}

std::optional<uint32_t>
uwb::protocol::fira::StringToVersion(const std::string& input) noexcept
{
    auto tokens = tokenize(input, '.');
    if (tokens.size() != 2) {
        return std::nullopt;
    }

    uint32_t first_byte, second_byte;
    try {
        std::istringstream ss{ tokens[0] };
        std::istringstream ss2{ tokens[1] };
        ss >> first_byte;
        ss2 >> second_byte;
    } catch (const std::exception& /* e */) {
        return std::nullopt;
    }
    if (first_byte > 0xFFU or second_byte > 0xFFU) {
        return std::nullopt;
    }

    return (first_byte << 8) | second_byte;
}

double
uwb::protocol::fira::ConvertQ97FormatToIEEE(uint16_t q97)
{
    static const double pow2 = std::pow(2, -7);
    static const uint16_t signMask = 0b1000'0000'0000'0000U;
    static const uint16_t unsignedIntegerMask = 0b0111'1111'1000'0000U;
    static const uint16_t fractionMask = ~(signMask | unsignedIntegerMask);

    bool sign = q97 & signMask;
    uint16_t unsignedIntegerPart = (q97 & unsignedIntegerMask) >> 7U;
    uint16_t fractionPart = q97 & fractionMask;

    if (sign) {
        unsignedIntegerPart = (~unsignedIntegerPart + 1U) & 0x00FF;
    }

    double unsignedNumber = static_cast<double>(unsignedIntegerPart + (fractionPart * pow2));

    return (sign ? -1 : 1) * unsignedNumber;
}

std::string
uwb::protocol::fira::ToString(const std::unordered_set<ResultReportConfiguration>& input)
{
    return std::transform_reduce(
        std::cbegin(input),
        std::cend(input),
        std::string{ "" },
        [](std::string a, std::string b) {
            if (a.empty() or b.empty()) {
                return std::move(a) + std::move(b);
            }
            return std::move(a) + "," + std::move(b);
        },
        [](const ResultReportConfiguration& resultReportConfiguration) {
            return std::string{ magic_enum::enum_name(resultReportConfiguration) };
        });
}

std::optional<std::unordered_set<ResultReportConfiguration>>
uwb::protocol::fira::StringToResultReportConfiguration(const std::string& input)
{
    std::unordered_set<ResultReportConfiguration> output{};

    for (const auto& token : tokenize(input, ',')) {
        auto resultReportConfiguration = magic_enum::enum_cast<ResultReportConfiguration>(token);
        if (resultReportConfiguration.has_value()) {
            output.insert(resultReportConfiguration.value());
        }
    }

    return output;
}

std::string
UwbDeviceInformation::ToString() const
{
    std::ostringstream ss;
    if (IsUwbStatusOk(Status)) {
        ss << "FiRa Uci v" << VersionUci << ", "
           << "FiRa Uci Test v" << VersionUciTest << ", "
           << "FiRa MAC v" << VersionMac << ", "
           << "FiRa PHY v" << VersionPhy;
        if (VendorSpecificInfo != nullptr) {
            ss << "\nVendor Specific Info: ";
            for (auto& val : VendorSpecificInfo->GetData()) {
                ss << val << " ";
            }
        }
    } else {
        ss << "(error=" << ::ToString(Status) << ")";
    }

    return ss.str();
}

std::string
UwbStatusDevice::ToString() const
{
    std::ostringstream ss;
    ss << "State: " << magic_enum::enum_name(State);
    return ss.str();
}

std::string
UwbMulticastListStatus::ToString() const
{
    std::ostringstream ss{};
    ss << "SubSessionId: " << SubSessionId << ", "
       << "ControleeMacAddress: " << ControleeMacAddress << ", "
       << "Status: " << magic_enum::enum_name(Status);
    return ss.str();
}

std::string
UwbSessionStatus::ToString() const
{
    std::ostringstream ss{};
    ss << "SessionId: " << SessionId << ", "
       << "State: " << magic_enum::enum_name(State) << ", "
       << "ReasonCode: ";
    if (ReasonCode.has_value()) {
        ss << magic_enum::enum_name(ReasonCode.value());
    } else {
        ss << "None";
    }
    return ss.str();
}

std::string
UwbRangingMeasurementData::ToString() const
{
    std::ostringstream ss{};
    ss << ConvertQ97FormatToIEEE(Result) << " (FoM=" << +FigureOfMerit.value_or(0) << ")";
    return ss.str();
}

std::string
UwbSessionUpdateMulticastListStatus::ToString() const
{
    std::ostringstream ss{};
    ss << "SessionId: " << SessionId << '\n'
       << "Statuses:\n";
    for (auto statusIndex = 0; statusIndex < std::size(Status); statusIndex++) {
        ss << " [" << statusIndex << "] " << Status[statusIndex] << '\n';
    }
    return ss.str();
}

std::string
UwbRangingMeasurement::ToString() const
{
    std::ostringstream ss{};
    ss << "SlotIndex: " << +SlotIndex << "\n"
       << "Distance: " << Distance << "\n"
       << "Status: " << ::ToString(Status) << "\n"
       << "Peer Mac Address: " << PeerMacAddress << "\n"
       << "Line Of Sight Indicator: " << magic_enum::enum_name(LineOfSightIndicator) << "\n"
       << "Angle of Arrival Azimuth: " << AoAAzimuth << "\n"
       << "Angle of Arrival Elevation: " << AoAElevation << "\n"
       << "Angle of Arrival Destination Azimuth: " << AoaDestinationAzimuth << "\n"
       << "Angle of Arrival Destination Elevation: " << AoaDestinationElevation;
    return ss.str();
}

std::string
UwbRangingData::ToString() const
{
    std::ostringstream ss{};
    ss << "Session Id: " << SessionId << "\n"
       << "Sequence Number: " << SequenceNumber << "\n"
       << "Ranging Interval: " << CurrentRangingInterval << "\n"
       << "Measurement Type: " << magic_enum::enum_name(RangingMeasurementType) << "\n\n"
       << "Measurements:\n";
    for (auto rangingMeasurementIndex = 0; rangingMeasurementIndex < std::size(RangingMeasurements); rangingMeasurementIndex++) {
        ss << "[" << rangingMeasurementIndex << "] " << RangingMeasurements[rangingMeasurementIndex] << '\n';
    }
    return ss.str();
}

std::string
uwb::protocol::fira::ToString(const UwbStatus& uwbStatus)
{
    std::ostringstream ss{};

    std::visit([&ss](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;
        if constexpr (std::is_enum_v<ValueType>) {
            ss << magic_enum::enum_name(arg);
        }
    },
        uwbStatus);

    return ss.str();
}

std::string
uwb::protocol::fira::ToString(const UwbNotificationData& uwbNotificationData)
{
    const std::unordered_map<std::type_index, std::string_view> TypeNameMap{
        { std::type_index(typeid(UwbStatus)), "Status" },
        { std::type_index(typeid(UwbStatusDevice)), "Device Status" },
        { std::type_index(typeid(UwbSessionStatus)), "Session Status" },
        { std::type_index(typeid(UwbSessionUpdateMulticastListStatus)), "Session Multicast List Status" },
        { std::type_index(typeid(UwbRangingData)), "Ranging Data" },
    };

    std::ostringstream ss{};

    std::visit([&](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;
        ss << TypeNameMap.at(typeid(ValueType)) << "\n";
        ss << "----------------------------------------\n";
        if constexpr (std::is_same_v<ValueType, UwbStatusDevice> || std::is_same_v<ValueType, UwbSessionStatus> || std::is_same_v<ValueType, UwbSessionUpdateMulticastListStatus> || std::is_same_v<ValueType, UwbRangingData>) {
            ss << arg;
        } else if constexpr (std::is_enum_v<ValueType>) {
            ss << magic_enum::enum_name(arg);
        }
    },
        uwbNotificationData);

    return ss.str();
}

bool
uwb::protocol::fira::IsUwbStatusOk(const UwbStatus& uwbStatus) noexcept
{
    const auto* status = std::get_if<UwbStatusGeneric>(&uwbStatus);
    return (status != nullptr) && (*status == UwbStatusGeneric::Ok);
}

bool
uwb::protocol::fira::IsUwbStatusRetry(const UwbStatus& uwbStatus) noexcept
{
    const auto* status = std::get_if<UwbStatusGeneric>(&uwbStatus);
    return (status != nullptr) && (*status == UwbStatusGeneric::CommandRetry);
}

std::string
UwbApplicationConfigurationParameter::ToString() const
{
    std::ostringstream ss{};
    ss << magic_enum::enum_name(Type) << ": " << ::ToString(Value);
    return ss.str();
}

/**
 * @brief Returns a string representation of the object.
 *
 * @param uwbApplicationConfigurationParameterValue
 * @return std::string
 */
std::string
uwb::protocol::fira::ToString(const UwbApplicationConfigurationParameterValue& uwbApplicationConfigurationParameterValue)
{
    std::ostringstream ss{};
    std::visit([&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_enum_v<T>) {
            ss << magic_enum::enum_name(arg);
        } else if constexpr (std::is_integral_v<T>) {
            ss << +arg;
        } else if constexpr (std::is_same_v<T, ::uwb::UwbMacAddress>) {
            ss << arg;
        } else if constexpr (std::is_same_v<T, std::unordered_set<::uwb::UwbMacAddress>>) {
            for (const auto& uwbMacAddress : arg) {
                ss << uwbMacAddress << ' ';
            }
        } else if constexpr (std::is_same_v<T, std::unordered_set<ResultReportConfiguration>>) {
            ss << ToString(arg);
        } else if constexpr (std::is_same_v<T, std::array<uint8_t, StaticStsInitializationVectorLength>>) {
            ss << std::showbase << std::hex << +arg[0] << +arg[1] << +arg[2] << +arg[3] << +arg[4] << +arg[5];
        } else {
            ss << arg;
        }
    },
        uwbApplicationConfigurationParameterValue);

    return ss.str();
}
