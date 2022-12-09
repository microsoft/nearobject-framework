
#include <uwb/protocols/fira/FiraDevice.hxx>

#include <magic_enum.hpp>
#include <numeric>
#include <sstream>

using namespace uwb::protocol::fira;

std::string
uwb::protocol::fira::VersionToString(uint32_t input)
{
    auto first_byte = input & 0xFFU;
    auto second_byte = (input >> 8) & 0xFFU;
    std::stringstream ss;
    ss << first_byte << "." << second_byte;
    return ss.str();
}

uint32_t
uwb::protocol::fira::StringToVersion(std::string input)
{
    auto pos = input.find(".");
    if (pos == std::string::npos || (pos + 1) >= input.size()) {
        throw std::runtime_error("version string incorrect format");
    }
    auto first_token = input.substr(0, pos);
    if (first_token.size() == 0) {
        throw std::runtime_error("version string incorrect format");
    }
    uint32_t first_byte;
    try {
        std::stringstream ss{ first_token };
        ss >> first_byte;
    } catch (std::exception e) {
        throw std::runtime_error("version string incorrect format");
    }
    if (first_byte > 0xFFU) {
        throw std::runtime_error("version string incorrect format");
    }
    first_byte <<= 8;
    auto second_token = input.substr(pos + 1);
    uint32_t second_byte;
    if (second_token.size() == 0) {
        throw std::runtime_error("version string incorrect format");
    }
    try {
        std::stringstream ss{ second_token };
        ss >> second_byte;
    } catch (std::exception e) {
        throw std::runtime_error("version string incorrect format");
    }
    if (second_byte > 0xFFU) {
        throw std::runtime_error("version string incorrect format");
    }
    return first_byte | second_byte;
}

std::string
uwb::protocol::fira::ResultReportConfigurationToString(const std::unordered_set<ResultReportConfiguration>& input)
{
    return std::transform_reduce(
        std::begin(input),
        std::end(input),
        std::string{ "" },
        [](const std::string& a, const std::string& b) {
            if (a.size() and b.size()) {
                return a + "," + b;
            } else {
                return a + b;
            }
        },
        [](const ResultReportConfiguration& a) {
            return std::string{ magic_enum::enum_name(a) };
        });
}

std::vector<std::string>
tokenize(const std::string& input, char deliminator)
{
    std::vector<std::string> output;
    std::stringstream ss{ input };
    while (ss.good()) {
        std::string a;
        getline(ss, a, deliminator);
        output.push_back(a);
    }
    return output;
}

std::unordered_set<ResultReportConfiguration>
uwb::protocol::fira::StringToResultReportConfiguration(const std::string& input, std::unordered_map<std::string, ResultReportConfiguration> map)
{
    auto tokens = tokenize(input, ',');
    std::unordered_set<ResultReportConfiguration> output{ };

    for(const auto& s : tokens){
        if (not map.contains(s)) {
                std::string error_msg{ "not a valid ResultReportConfiguration: " };
                error_msg = error_msg + s;
                throw std::runtime_error(error_msg);
            }
        output.insert(map.at(s));
    }

    return output;
}