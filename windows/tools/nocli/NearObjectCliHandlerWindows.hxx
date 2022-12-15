
#ifndef NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX
#define NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX

#include <memory>

#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <nearobject/cli/NearObjectCliData.hxx>
#include <uwb/UwbDevice.hxx>

namespace nearobject::cli
{
struct NearObjectCliHandlerWindows :
    public NearObjectCliHandler
{
    std::shared_ptr<::uwb::UwbDevice>
    ResolveUwbDevice(const nearobject::cli::NearObjectCliData& cliData) noexcept override;
};
} // namespace nearobject::cli

#endif // NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX
