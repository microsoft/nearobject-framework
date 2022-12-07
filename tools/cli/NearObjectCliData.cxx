#include "include/nearobject/cli/NearObjectCliData.hxx"

#include <magic_enum.hpp>
#include <uwb/protocols/fira/UwbConfiguration.hxx>


using namespace nearobject::cli;

std::map<std::string, uwb::protocol::fira::DeviceRole> NearObjectCliData::deviceRoleMap{ 
    { std::string{ magic_enum::enum_name(uwb::protocol::fira::DeviceRole::Initiator) }, uwb::protocol::fira::DeviceRole::Initiator },
    { std::string{ magic_enum::enum_name(uwb::protocol::fira::DeviceRole::Responder) }, uwb::protocol::fira::DeviceRole::Responder } 
};
