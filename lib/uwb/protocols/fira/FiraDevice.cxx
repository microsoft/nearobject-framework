
#include <uwb/protocols/fira/FiraDevice.hxx>

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
    if(first_byte > 0xFFU){
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
    if(second_byte > 0xFFU){
        throw std::runtime_error("version string incorrect format");
    }
    return first_byte | second_byte;
}
