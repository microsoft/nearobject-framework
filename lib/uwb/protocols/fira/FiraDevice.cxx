
#include <uwb/protocols/fira/FiraDevice.hxx>

#include <magic_enum.hpp>
#include <numeric>
#include <sstream>

using namespace uwb::protocol::fira;

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

std::string
uwb::protocol::fira::ResultReportConfigurationToString(const std::unordered_set<ResultReportConfiguration>& input)
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
uwb::protocol::fira::StringToResultReportConfiguration(const std::string& input, std::unordered_map<std::string, ResultReportConfiguration> map)
{
    auto tokens = tokenize(input, ',');
    std::unordered_set<ResultReportConfiguration> output{};

    for (const auto& s : tokens) {
        if (not map.contains(s)) {
            return std::nullopt;
        }
        output.insert(map.at(s));
    }

    return output;
}
