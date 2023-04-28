// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

// clang-format off
#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
// clang-format on

#include "Utils.hxx"

#include <fstream>
#include <microsoft.ui.xaml.window.h>
#include <nearobject/serialization/UwbSessionDataJsonSerializer.hxx>
#include <shobjidl.h>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Storage::Pickers;
using namespace winrt::Windows::UI::Core;
using namespace Microsoft::UI::Xaml;
using namespace uwb::protocol::fira;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::OobSimulator::implementation
{
MainWindow::MainWindow()
{
    InitializeComponent();
}

IAsyncAction
MainWindow::SetUwbSessionData(UwbSessionData const& uwbSessionData)
{
    // Retrieve the window handle (HWND) of the current WinUI 3 window.
    auto windowNative{ this->try_as<::IWindowNative>() };
    winrt::check_bool(windowNative);
    HWND hwnd{ 0 };
    windowNative->get_WindowHandle(&hwnd);

    FileSavePicker fileSavePicker;
    auto initializeWithWindow{ fileSavePicker.as<::IInitializeWithWindow>() };
    initializeWithWindow->Initialize(hwnd);

    // Convert the input data into JSON
    nlohmann::json json = uwbSessionData;
    // nlohmann::json json{};
    // json["sessionId"] = uwbSessionData.sessionId;
    // json["deviceRole"] = static_cast<int>(uwbSessionData.uwbConfiguration.GetDeviceRole().value());
    // json["multiNodeMode"] = static_cast<int>(uwbSessionData.uwbConfiguration.GetMultiNodeMode().value());
    // json["controllerMacAddress"] = uwbSessionData.uwbConfiguration.GetControllerMacAddress().value().ToString();         // Is ToString() the best way to store this?
    // json["controleeShortMacAddress"] = uwbSessionData.uwbConfiguration.GetControleeShortMacAddress().value().ToString(); // Is ToString() the best way to store this?

    // Serialize JSON to byte array (MessagePack)
    std::vector<uint8_t> sessionDataBytes = nlohmann::json::to_msgpack(json);

    // Write to file
    fileSavePicker.FileTypeChoices().Insert(L"Message Pack", winrt::single_threaded_vector<winrt::hstring>({ L".msgpack" }));
    fileSavePicker.SuggestedStartLocation(PickerLocationId::DocumentsLibrary);
    fileSavePicker.SuggestedFileName(L"uwbsessiondata.msgpack");
    auto file = co_await fileSavePicker.PickSaveFileAsync();

    if (file != nullptr) {
        auto storageFile = file.as<IStorageFile>();
        co_await FileIO::WriteBytesAsync(storageFile, sessionDataBytes);
    }
}

IAsyncAction
MainWindow::SetUwbSessionData_Click(IInspectable const&, RoutedEventArgs const&)
{
    UwbSessionData uwbSessionData;
    UwbConfiguration::Builder builder{};

    // UWB Session ID
    uwbSessionData.sessionId = HstringToUint32(UwbSessionIdTextBox().Text());

    // Device Role
    if (ResponderRadioButton().IsChecked().Value()) {
        builder.SetDeviceRole(DeviceRole::Responder);
    } else {
        builder.SetDeviceRole(DeviceRole::Initiator);
    }

    // Multi-Node Mode
    if (UnicastRadioButton().IsChecked().Value()) {
        builder.SetMultiNodeMode(MultiNodeMode::Unicast);
    } else if (OneToManyRadioButton().IsChecked().Value()) {
        builder.SetMultiNodeMode(MultiNodeMode::OneToMany);
    } else {
        builder.SetMultiNodeMode(MultiNodeMode::ManyToMany);
    }

    // Controller MAC Address
    builder.SetMacAddressController(uwb::UwbMacAddress::FromString(winrt::to_string(ControllerMacAddressTextBox().Text()), uwb::UwbMacAddressType::Short).value());

    // Controlee Short MAC Address
    builder.SetMacAddressControleeShort(uwb::UwbMacAddress::FromString(winrt::to_string(ControleeShortMacAddressTextBox().Text()), uwb::UwbMacAddressType::Short).value());

    // Set UWB Configuration of UWB Session Data
    uwbSessionData.uwbConfiguration = std::move(builder);

    // Set the UWB Session Data
    co_await SetUwbSessionData(uwbSessionData);
}
} // namespace winrt::OobSimulator::implementation
