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
#include "uwb/protocols/fira/UwbConfigurationBuilder.hxx"
#include <fstream>

using namespace winrt;
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

void
MainWindow::SetUwbSessionData(UwbSessionData const& uwbSessionData, std::string const& outputDirectory)
{
    // Convert the input data into JSON
    nlohmann::json json{};
    json["sessionId"] = uwbSessionData.sessionId;
    json["deviceRole"] = static_cast<int>(uwbSessionData.uwbConfiguration.GetDeviceRole().value());
    json["multiNodeMode"] = static_cast<int>(uwbSessionData.uwbConfiguration.GetMultiNodeMode().value());
    json["controllerMacAddress"] = uwbSessionData.uwbConfiguration.GetControllerMacAddress().value().ToString();         // Is ToString() the best way to store this?
    json["controleeShortMacAddress"] = uwbSessionData.uwbConfiguration.GetControleeShortMacAddress().value().ToString(); // Is ToString() the best way to store this?

    // Serialize JSON to byte array (MessagePack)
    std::vector<uint8_t> sessionDataBytes = nlohmann::json::to_msgpack(json);

    // Write to file
    std::filesystem::path outfilePath = outputDirectory + "/uwbsessiondata.msgpack";
    std::ofstream outfile(outfilePath, std::ios::out | std::ios::binary);
    if (outfile.fail() || outfile.bad()) {
        std::cerr << "Error opening file" << std::endl;
    }

    outfile.write(reinterpret_cast<const char*>(std::data(sessionDataBytes)), std::size(sessionDataBytes));
    if (outfile.fail() || outfile.bad()) {
        std::cerr << "Error writing to file" << std::endl;
    }

    outfile.close();
}

void
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

    // Output directory
    std::string outputDirectory = winrt::to_string(OutputDirectoryTextBox().Text());

    // Set UWB Configuration of UWB Session Data
    uwbSessionData.uwbConfiguration = std::move(builder);

    // Set the UWB Session Data
    SetUwbSessionData(uwbSessionData, outputDirectory);
}
} // namespace winrt::OobSimulator::implementation
