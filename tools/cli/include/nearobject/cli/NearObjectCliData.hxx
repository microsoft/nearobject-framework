
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <map>
#include <magic_enum.hpp>

namespace nearobject::cli
{
/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    std::string defaultFile {"default.json"};
    bool hostIsController {false};
    uwb::protocol::fira::UwbConfiguration defaultConfiguration {};
};

// std::map<std::string,uwb::protocol::fira::DeviceRole> deviceRoleMap;

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
