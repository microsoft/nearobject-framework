
#ifndef NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX
#define NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <nearobject/cli/NearObjectCliData.hxx>
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/UwbDevice.hxx>

namespace windows::devices::uwb
{
class UwbDevice;
} // namespace windows::devices::uwb

namespace nearobject::cli
{
struct NearObjectCliHandlerWindows :
    public NearObjectCliHandler
{
    std::shared_ptr<::uwb::UwbDevice>
    ResolveUwbDevice(const nearobject::cli::NearObjectCliData& cliData) noexcept override;

    void
    HandleStartRanging(uwb::protocol::fira::DeviceType deviceType, std::filesystem::path sessionDataFilePath) noexcept override;

    void
    HandleMonitorMode() noexcept override;

private:
    void
    OnDeviceArrived(const std::string& deviceName);

    void
    OnDeviceDeparted(const std::string& deviceName);

private:
    std::vector<std::shared_ptr<windows::devices::uwb::UwbDevice>> m_uwbDevices;
};
} // namespace nearobject::cli

#endif // NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX
