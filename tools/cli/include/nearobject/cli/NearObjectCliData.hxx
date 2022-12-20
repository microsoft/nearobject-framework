
#ifndef NEAR_OBJECT_CLI_DATA_HXX
#define NEAR_OBJECT_CLI_DATA_HXX

#include <string>
#include <unordered_map>

#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace nearobject::cli
{
/**
 * @brief Base class for data parsed by NearObjectCli.
 */
struct NearObjectCliData
{
    virtual ~NearObjectCliData() = default;

    std::string MacVersionString;
    std::string PhyVersionString;
    std::string ResultReportConfigurationString;

    uwb::protocol::fira::StaticRangingInfo StaticRanging{};

    bool HostIsController{ false };
    uwb::protocol::fira::UwbSessionData SessionData{};
};

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
