#ifndef UWB_SESSION_DATA_H
#define UWB_SESSION_DATA_H

#include "UwbSessionData.g.h"

namespace winrt::OobSimulator::implementation
{
struct UwbSessionData : UwbSessionDataT<UwbSessionData>
{
    UwbSessionData() = default;

    hstring
    SessionId();
    void
    SessionId(hstring const& value);
    winrt::OobSimulator::DeviceRole
    DeviceRole();
    void
    DeviceRole(winrt::OobSimulator::DeviceRole const& value);
    winrt::OobSimulator::MultiNodeMode
    MultiNodeMode();
    void
    MultiNodeMode(winrt::OobSimulator::MultiNodeMode const& value);
    hstring
    ControllerMacAddress();
    void
    ControllerMacAddress(hstring const& value);
    hstring
    ControleeShortMacAddress();
    void
    ControleeShortMacAddress(hstring const& value);
    winrt::OobSimulator::SessionDataEncoding
    SessionDataEncoding();
    void
    SessionDataEncoding(winrt::OobSimulator::SessionDataEncoding const& value);
    winrt::event_token
    PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
    void
    PropertyChanged(winrt::event_token const& token) noexcept;

private:
    winrt::hstring m_sessionId;
    winrt::OobSimulator::DeviceRole m_deviceRole;
    winrt::OobSimulator::MultiNodeMode m_multiNodeMode;
    winrt::hstring m_controllerMacAddress;
    winrt::hstring m_controleeShortMacAddress;
    winrt::OobSimulator::SessionDataEncoding m_sessionDataEncoding;

    winrt::event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
};
} // namespace winrt::OobSimulator::implementation
namespace winrt::OobSimulator::factory_implementation
{
struct UwbSessionData : UwbSessionDataT<UwbSessionData, implementation::UwbSessionData>
{
};
} // namespace winrt::OobSimulator::factory_implementation

#endif // UWB_SESSION_DATA_H
