// Copyright (c) Microsoft Corporation and Contributors.

// clang-format off
#include "pch.h"
#include "MainViewModel.h"
// clang-format on
#include "DelegateCommand.h"
#include "Utils.hxx"

#include <nearobject/serialization/UwbSessionDataJsonSerializer.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace winrt::OobSimulator::ViewModels::implementation
{
winrt::hstring
MainViewModel::UwbSessionId()
{
    return m_uwbSessionId;
}

void
MainViewModel::UwbSessionId(winrt::hstring const& uwbSessionId)
{
    if (m_uwbSessionId != uwbSessionId) {
        m_uwbSessionId = uwbSessionId;
        RaisePropertyChanged(L"UWB Session Id");
    }
}

DeviceRoleWrapper
MainViewModel::DeviceRole()
{
    return m_deviceRole;
}

void
MainViewModel::DeviceRole(DeviceRoleWrapper const& deviceRole)
{
    if (m_deviceRole != deviceRole) {
        m_deviceRole = deviceRole;
        RaisePropertyChanged(L"Device Role");
    }
}

MultiNodeModeWrapper
MainViewModel::MultiNodeMode()
{
    return m_multiNodeMode;
}

void
MainViewModel::MultiNodeMode(MultiNodeModeWrapper const& multiNodeMode)
{
    if (m_multiNodeMode != multiNodeMode) {
        m_multiNodeMode = multiNodeMode;
        RaisePropertyChanged(L"Multi Node Mode");
    }
}

winrt::hstring
MainViewModel::ControllerMacAddressString()
{
    return m_controllerMacAddressString;
}

void
MainViewModel::ControllerMacAddressString(winrt::hstring const& controllerMacAddressString)
{
    if (m_controllerMacAddressString != controllerMacAddressString) {
        m_controllerMacAddressString = controllerMacAddressString;
        RaisePropertyChanged(L"Controller MAC Address String");
    }
}

winrt::hstring
MainViewModel::ControleeShortMacAddressString()
{
    return m_controleeShortMacAddressString;
}

void
MainViewModel::ControleeShortMacAddressString(winrt::hstring const& controleeShortMacAddressString)
{
    if (m_controleeShortMacAddressString != controleeShortMacAddressString) {
        m_controleeShortMacAddressString = controleeShortMacAddressString;
        RaisePropertyChanged(L"Controlee Short MAC Address String");
    }
}

winrt::event_token
MainViewModel::PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
{
    return m_propertyChanged.add(handler);
}

void
MainViewModel::PropertyChanged(winrt::event_token const& token) noexcept
{
    m_propertyChanged.remove(token);
}

void
MainViewModel::RaisePropertyChanged(winrt::hstring const& propertyName)
{
    m_propertyChanged(*this, winrt::Windows::UI::Xaml::Data::PropertyChangedEventArgs(propertyName));
}

winrt::Windows::UI::Xaml::Input::ICommand
MainViewModel::SetUwbSessionDataCommand()
{
    return winrt::make<DelegateCommand>(std::bind(&MainViewModel::SetUwbSessionData, this));
}

winrt::Windows::Foundation::IAsyncAction
MainViewModel::SetUwbSessionData()
{
    uwb::protocol::fira::UwbSessionData uwbSessionData;
    uwb::protocol::fira::UwbConfiguration::Builder builder{};

    // UWB Session ID
    uwbSessionData.sessionId = HstringToUint32(m_uwbSessionId);

    // Device Role
    builder.SetDeviceRole(m_deviceRole.Value);

    // Multi-Node Mode
    builder.SetMultiNodeMode(m_multiNodeMode.Value);

    // Controller MAC Address
    builder.SetMacAddressController(uwb::UwbMacAddress::FromString(winrt::to_string(m_controllerMacAddressString), uwb::UwbMacAddressType::Short).value());

    // Controlee Short MAC Address
    builder.SetMacAddressControleeShort(uwb::UwbMacAddress::FromString(winrt::to_string(m_controleeShortMacAddressString), uwb::UwbMacAddressType::Short).value());

    // Set UWB Configuration of UWB Session Data
    uwbSessionData.uwbConfiguration = std::move(builder);

    // Create FileSavePicker
    winrt::Windows::Storage::Pickers::FileSavePicker fileSavePicker;

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
} // namespace winrt::OobSimulator::ViewModels::implementation
