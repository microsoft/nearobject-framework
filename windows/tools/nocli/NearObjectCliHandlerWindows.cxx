
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

#include <magic_enum.hpp>
#include <notstd/guid.hxx>
#include <plog/Log.h>

#include <windows/devices/DeviceEnumerator.hxx>
#include <windows/devices/DevicePresenceMonitor.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbDeviceDriver.hxx>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/windows.devices.nearobject.h>

#include "NearObjectCliDataWindows.hxx"
#include "NearObjectCliHandlerWindows.hxx"

using namespace nearobject::cli;
using namespace windows::devices;
using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Foundation;

namespace detail
{
/**
 * @brief Get the default UWB device on the system.
 *
 * This enumerates all the UWB devices on the system and returns the name of the
 * first one.
 *
 * @return std::string
 */
std::string
GetUwbDeviceNameDefault() noexcept
{
    const auto uwbDeviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(windows::devices::uwb::InterfaceClassUwb);
    return !uwbDeviceNames.empty() ? uwbDeviceNames.front() : "";
}

/**
 * @brief Probes for uwb devices and prompts the user to select one.
 *
 * @param deviceClassGuid
 * @return std::string
 */
std::string
GetUwbDeviceNameFromProbe(std::string_view deviceClassGuid = windows::devices::uwb::InterfaceClassUwbString)
{
    const auto uwbDeviceNames = DeviceEnumerator::GetDeviceInterfaceClassInstanceNames(deviceClassGuid);
    if (uwbDeviceNames.empty()) {
        return "";
    }

    int32_t index = 0;
    for (const auto& uwbDeviceName : uwbDeviceNames) {
        std::cout << "[" << index++ << "] " << uwbDeviceName << std::endl;
    }

    for (index = -1;;) {
        std::cout << "select the uwb device to use from the list above [0-" << uwbDeviceNames.size() - 1 << "]: ";
        std::cin >> index;
        if (index >= 0 && index < uwbDeviceNames.size()) {
            break;
        }
        if (std::cin.fail()) {
            std::cin.clear();
        }
        std::cout << "invalid device index specified; please enter an index between 0 and " << uwbDeviceNames.size() - 1 << std::endl;
    }

    return uwbDeviceNames[static_cast<std::size_t>(index)];
}

/**
 * @brief Tries to resolve the UwbDevice to use for ranging.
 *
 * @param cliData
 * @return std::shared_ptr<windows::devices::UwbDevice>
 */
std::shared_ptr<windows::devices::uwb::UwbDevice>
ResolveUwbDevice(const nearobject::cli::NearObjectCliDataWindows& cliData)
{
    std::string deviceName = cliData.DeviceNameProbe
        ? GetUwbDeviceNameFromProbe(cliData.DeviceClassGuid)
        : cliData.DeviceName.value_or(GetUwbDeviceNameDefault());

    // Ensure a device name was set.
    if (deviceName.empty()) {
        return nullptr;
    }

    return windows::devices::uwb::UwbDevice::Create(deviceName);
}
} // namespace detail

std::shared_ptr<::uwb::UwbDevice>
NearObjectCliHandlerWindows::ResolveUwbDevice(const nearobject::cli::NearObjectCliData& cliData) noexcept
{
    const auto* cliDataWindows = dynamic_cast<const NearObjectCliDataWindows*>(&cliData);
    if (!cliDataWindows) {
        return nullptr;
    }

    return detail::ResolveUwbDevice(*cliDataWindows);
}

void
NearObjectCliHandlerWindows::HandleStartRanging(::uwb::protocol::fira::DeviceType deviceType, std::filesystem::path sessionDataFilePath) noexcept
{
    std::ifstream inputFileStream(sessionDataFilePath, std::ios::binary);
    if (!inputFileStream.is_open()) {
        // TODO: Log error
        return;
    }

    // Read file and store data into byte buffer
    std::vector<uint8_t> uwbSessionDataSerialized;
    std::string line;
    // File only contains one line of data
    if (std::getline(inputFileStream, line)) {
        uwbSessionDataSerialized = std::vector<uint8_t>(std::cbegin(line), std::cend(line));
    }

    inputFileStream.close();

    // Create a DataWriter to write the serialized data to an InMemoryRandomAccessStream
    winrt::Windows::Storage::Streams::InMemoryRandomAccessStream stream;
    winrt::Windows::Storage::Streams::DataWriter dataWriter(stream);
    dataWriter.WriteBytes(winrt::array_view<const uint8_t>(uwbSessionDataSerialized));
    auto uwbSessionDataBuffer = dataWriter.DetachBuffer();

    // Create the NearObjectIdentityToken from the byte buffer
    winrt::Windows::Devices::NearObject::INearObjectIdentityToken identityTokenUwb = NearObject::NearObjectIdentityToken::TryParse(uwbSessionDataBuffer);

    // Create the NearObjectSessionConfiguration
    NearObject::NearObjectSessionConfiguration sessionConfiguration;
    sessionConfiguration.IdentityToken(identityTokenUwb);

    // Create the NearObjectSession
    IAsyncOperation<NearObject::NearObjectSessionCreateResult> sessionCreateResultAsyncOperation = NearObject::NearObjectSession::CreateAsync(sessionConfiguration);

    // Get the result from the async operation
    NearObject::NearObjectSessionCreateResult sessionCreateResult = sessionCreateResultAsyncOperation.get();

    // Start the ranging session
    sessionCreateResult.HostClient().Start();
}

void
NearObjectCliHandlerWindows::OnDeviceArrived(const std::string& deviceName)
{
    auto uwbDevice = windows::devices::uwb::UwbDevice::Create(deviceName);
    if (!uwbDevice) {
        PLOG_ERROR << "Failed to instantiate UWB device with name " << deviceName;
        return;
    }

    uwbDevice->Initialize();
    m_uwbDevices.push_back(std::move(uwbDevice));
}

void
NearObjectCliHandlerWindows::OnDeviceDeparted(const std::string& deviceName)
{
    auto numErased = std::erase_if(m_uwbDevices, [&](const auto& uwbDevice) {
        return uwbDevice->DeviceName() == deviceName;
    });
    if (numErased == 0) {
        PLOG_WARNING << "UWB device with name " << deviceName << " not found; ignoring removal event";
    }
}

void
NearObjectCliHandlerWindows::HandleMonitorMode() noexcept
try {
    DevicePresenceMonitor presenceMonitor(
        windows::devices::uwb::InterfaceClassUwb, [&](auto&& deviceGuid, auto&& presenceEvent, auto&& deviceName) {
            const auto presenceEventName = magic_enum::enum_name(presenceEvent);
            PLOG_INFO << deviceName << " " << presenceEventName << std::endl;

            switch (presenceEvent) {
            case DevicePresenceEvent::Arrived:
                OnDeviceArrived(deviceName);
                break;
            case DevicePresenceEvent::Departed:
                OnDeviceDeparted(deviceName);
                break;
            default:
                PLOG_ERROR << "Ignoring unknown presence event";
                break;
            }
        },
        /* enumerateInitialDevicesOnStart = */ true);

    presenceMonitor.Start();

    // Wait for input before stopping.
    PLOG_INFO << "UWB monitor mode started. Press <enter> to stop monitoring.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    presenceMonitor.Stop();
    PLOG_INFO << "UWB monitor mode stopped";

} catch (...) {
    // TODO: handle this properly
}
