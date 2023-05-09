﻿// clang-format off
#include "pch.h"
#include "UwbSessionDataPageViewModel.h"
#if __has_include("UwbSessionDataPageViewModel.g.cpp")
#include "UwbSessionDataPageViewModel.g.cpp"
#endif
// clang-format on

#include "Utils.hxx"

#include <nearobject/serialization/UwbSessionDataJsonSerializer.hxx>
#include <shobjidl.h>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace winrt::OobSimulator::implementation
{
UwbSessionDataPageViewModel::UwbSessionDataPageViewModel()
{
    m_uwbSessionData = winrt::make<OobSimulator::implementation::UwbSessionData>();
    m_setUwbSessionData = winrt::make<OobSimulator::implementation::DelegateCommand>(std::bind(&UwbSessionDataPageViewModel::SetUwbSessionData, this));

    // Set default values
    m_uwbSessionData.SessionId(L"1234");
    m_uwbSessionData.DeviceRole(OobSimulator::DeviceRole::Responder);
    m_uwbSessionData.MultiNodeMode(OobSimulator::MultiNodeMode::Unicast);
    m_uwbSessionData.ControllerMacAddress(L"12:34");
    m_uwbSessionData.ControleeShortMacAddress(L"67:89");
}

winrt::OobSimulator::UwbSessionData
UwbSessionDataPageViewModel::UwbSessionData()
{
    return m_uwbSessionData;
}

winrt::Microsoft::UI::Xaml::Input::ICommand
UwbSessionDataPageViewModel::SetUwbSessionDataCommand()
{
    return m_setUwbSessionData;
}

winrt::Windows::Foundation::IAsyncAction
UwbSessionDataPageViewModel::SetUwbSessionData()
{
    uwb::protocol::fira::UwbSessionData uwbSessionData{};
    uwb::protocol::fira::UwbConfiguration::Builder builder{};

    // UWB Session ID
    uwbSessionData.sessionId = HstringToUint32(m_uwbSessionData.SessionId());

    // Device Role
    builder.SetDeviceRole(ConvertDeviceRole());

    // Multi-Node Mode
    builder.SetMultiNodeMode(ConvertMultiNodeMode());

    // Controller MAC Address
    builder.SetMacAddressController(uwb::UwbMacAddress::FromString(winrt::to_string(m_uwbSessionData.ControllerMacAddress()), uwb::UwbMacAddressType::Short).value());

    // Controlee Short MAC Address
    builder.SetMacAddressControleeShort(uwb::UwbMacAddress::FromString(winrt::to_string(m_uwbSessionData.ControleeShortMacAddress()), uwb::UwbMacAddressType::Short).value());

    // Set UWB Configuration of UWB Session Data
    uwbSessionData.uwbConfiguration = std::move(builder);

    // Create FileSavePicker
    winrt::Windows::Storage::Pickers::FileSavePicker fileSavePicker;
    auto initializeWithWindow{ fileSavePicker.as<::IInitializeWithWindow>() };
    initializeWithWindow->Initialize(g_hwnd);

    // Convert the input data to JSON
    nlohmann::json json = uwbSessionData;

    // Serialize JSON to byte array (MessagePack)
    std::vector<uint8_t> sessionDataBytes = nlohmann::json::to_msgpack(json);

    // Write to file
    fileSavePicker.FileTypeChoices().Insert(L"Message Pack", winrt::single_threaded_vector<winrt::hstring>({ L".msgpack" }));
    fileSavePicker.SuggestedStartLocation(winrt::Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary);
    fileSavePicker.SuggestedFileName(L"uwbsessiondata.msgpack");
    auto file = co_await fileSavePicker.PickSaveFileAsync();

    if (file != nullptr) {
        auto storageFile = file.as<winrt::Windows::Storage::IStorageFile>();
        co_await winrt::Windows::Storage::FileIO::WriteBytesAsync(storageFile, sessionDataBytes);
    }
}

uwb::protocol::fira::DeviceRole
UwbSessionDataPageViewModel::ConvertDeviceRole()
{
    auto oobSimulatorDeviceRole = m_uwbSessionData.DeviceRole();
    auto deviceRole = uwb::protocol::fira::DeviceRole::Responder;
    if (oobSimulatorDeviceRole == OobSimulator::DeviceRole::Initiator) {
        deviceRole = uwb::protocol::fira::DeviceRole::Initiator;
    }

    return std::move(deviceRole);
}

uwb::protocol::fira::MultiNodeMode
UwbSessionDataPageViewModel::ConvertMultiNodeMode()
{
    auto oobSimulatorMultiNodeMode = m_uwbSessionData.MultiNodeMode();
    auto multiNodeMode = uwb::protocol::fira::MultiNodeMode::Unicast;
    if (oobSimulatorMultiNodeMode == OobSimulator::MultiNodeMode::OneToMany) {
        multiNodeMode = uwb::protocol::fira::MultiNodeMode::OneToMany;
    } else if (oobSimulatorMultiNodeMode == OobSimulator::MultiNodeMode::ManyToMany) {
        multiNodeMode = uwb::protocol::fira::MultiNodeMode::ManyToMany;
    }

    return std::move(multiNodeMode);
}
} // namespace winrt::OobSimulator::implementation