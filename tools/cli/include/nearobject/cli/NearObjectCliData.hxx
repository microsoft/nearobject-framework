
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <map>

namespace nearobject::cli
{
/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    static std::map<std::string,uwb::protocol::fira::DeviceRole> deviceRoleMap;
    
    std::string defaultFile {"default.json"};
    bool hostIsController {false};
    uwb::protocol::fira::UwbConfiguration defaultConfiguration {};

};



} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
