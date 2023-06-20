
#ifndef NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX
#define NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <nearobject/cli/NearObjectCliData.hxx>
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>

#include <winrt/windows.devices.nearobject.h>

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
    winrt::Windows::Devices::NearObject::INearObjectSessionClient m_sessionClient;

    // Event handler tokens.
    winrt::Windows::Devices::NearObject::INearObjectSessionEventProducer::SessionEnded_revoker m_sessionEndedEventToken;
    winrt::Windows::Devices::NearObject::INearObjectSessionEventProducer::RangingStarted_revoker m_rangingStartedEventToken;
    winrt::Windows::Devices::NearObject::INearObjectSessionEventProducer::RangingStopped_revoker m_rangingStoppedEventToken;
    winrt::Windows::Devices::NearObject::INearObjectSessionEventProducer::PeerPropertiesChanged_revoker m_peerPropertiesChangedEventToken;
    winrt::Windows::Devices::NearObject::INearObjectSessionEventProducer::SessionMembershipChanged_revoker m_sessionMemembershipChangedEventToken;
};
} // namespace nearobject::cli

#endif // NEAR_OBEJCT_CLI_HANDLER_WINDOWS_HXX
