// Copyright (c) Microsoft Corporation and Contributors.

#pragma once

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <winrt/Windows.UI.Xaml.Data.h>

namespace winrt::OobSimulator::ViewModels::implementation
{
struct DeviceRoleWrapper : winrt::Windows::Foundation::IInspectable
{
    uwb::protocol::fira::DeviceRole Value;
};

struct MultiNodeModeWrapper : winrt::Windows::Foundation::IInspectable
{
    uwb::protocol::fira::MultiNodeMode Value;
};

struct MainViewModel : public winrt::implements<MainViewModel, winrt::Windows::UI::Xaml::Data::INotifyPropertyChanged>
{
public:
    // Constructor
    MainViewModel() = default;

    // Properties
    winrt::hstring
    UwbSessionId();
    void
    UwbSessionId(winrt::hstring const& uwbSessionId);

    DeviceRoleWrapper
    DeviceRole();
    void
    DeviceRole(DeviceRoleWrapper const& deviceRole);

    MultiNodeModeWrapper
    MultiNodeMode();
    void
    MultiNodeMode(MultiNodeModeWrapper const& multiNodeMode);

    winrt::hstring
    ControllerMacAddressString();
    void
    ControllerMacAddressString(winrt::hstring const& controllerMacAddressString);

    winrt::hstring
    ControleeShortMacAddressString();
    void
    ControleeShortMacAddressString(winrt::hstring const& controleeShortMacAddressString);

    // Event Handlers
    winrt::event_token
    PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
    void
    PropertyChanged(winrt::event_token const& token) noexcept;

    winrt::Windows::UI::Xaml::Input::ICommand
    SetUwbSessionDataCommand();

    winrt::Windows::Foundation::IAsyncAction
    SetUwbSessionData();

private:
    winrt::hstring m_uwbSessionId;
    DeviceRoleWrapper m_deviceRole;
    MultiNodeModeWrapper m_multiNodeMode;
    winrt::hstring m_controllerMacAddressString;
    winrt::hstring m_controleeShortMacAddressString;

    winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

    void
    RaisePropertyChanged(winrt::hstring const& propertyName);
};
} // namespace winrt::OobSimulator::ViewModels::implementation
