#pragma once
#include "UwbSessionData.g.h"

namespace winrt::OobSimulator::implementation
{
    struct UwbSessionData : UwbSessionDataT<UwbSessionData>
    {
        UwbSessionData() = default;

        hstring SessionId();
        void SessionId(hstring const& value);
        winrt::OobSimulator::DeviceRole DeviceRole();
        void DeviceRole(winrt::OobSimulator::DeviceRole const& value);
        winrt::OobSimulator::MultiNodeMode MultiNodeMode();
        void MultiNodeMode(winrt::OobSimulator::MultiNodeMode const& value);
        hstring ControllerMacAddress();
        void ControllerMacAddress(hstring const& value);
        hstring ControleeShortMacAddress();
        void ControleeShortMacAddress(hstring const& value);
        winrt::event_token PropertyChanged(winrt::Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

    private:
        winrt::hstring m_sessionId;
        winrt::OobSimulator::DeviceRole m_deviceRole;
        winrt::OobSimulator::MultiNodeMode m_multiNodeMode;
        winrt::hstring m_controllerMacAddress;
        winrt::hstring m_controleeShortMacAddress;

        winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
    };
}
namespace winrt::OobSimulator::factory_implementation
{
    struct UwbSessionData : UwbSessionDataT<UwbSessionData, implementation::UwbSessionData>
    {
    };
}
