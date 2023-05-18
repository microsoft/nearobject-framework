// clang-format off
#include "pch.h"
#include "UwbSessionData.h"
#include "UwbSessionData.g.cpp"
// clang-format on

namespace winrt::OobSimulator::implementation
{
winrt::hstring
UwbSessionData::SessionId()
{
    return m_sessionId;
}
void
UwbSessionData::SessionId(winrt::hstring const& value)
{
    if (m_sessionId != value) {
        m_sessionId = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"UWB Session ID" });
    }
}
winrt::OobSimulator::DeviceRole
UwbSessionData::DeviceRole()
{
    return m_deviceRole;
}
void
UwbSessionData::DeviceRole(winrt::OobSimulator::DeviceRole const& value)
{
    if (m_deviceRole != value) {
        m_deviceRole = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"Device Role" });
    }
}
winrt::OobSimulator::MultiNodeMode
UwbSessionData::MultiNodeMode()
{
    return m_multiNodeMode;
}
void
UwbSessionData::MultiNodeMode(winrt::OobSimulator::MultiNodeMode const& value)
{
    if (m_multiNodeMode != value) {
        m_multiNodeMode = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"Multi Node Mode" });
    }
}
hstring
UwbSessionData::ControllerMacAddress()
{
    return m_controllerMacAddress;
}
void
UwbSessionData::ControllerMacAddress(hstring const& value)
{
    if (m_controllerMacAddress != value) {
        m_controllerMacAddress = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"Controller MAC Address" });
    }
}
hstring
UwbSessionData::ControleeShortMacAddress()
{
    return m_controleeShortMacAddress;
}
void
UwbSessionData::ControleeShortMacAddress(hstring const& value)
{
    if (m_controleeShortMacAddress != value) {
        m_controleeShortMacAddress = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"Controlee Short MAC Address" });
    }
}
winrt::OobSimulator::SessionDataEncoding
UwbSessionData::SessionDataEncoding()
{
    return m_sessionDataEncoding;
}
void
UwbSessionData::SessionDataEncoding(winrt::OobSimulator::SessionDataEncoding const& value)
{
    if (m_sessionDataEncoding != value) {
        m_sessionDataEncoding = value;
        m_propertyChanged(*this, winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventArgs{ L"Session Data Encoding" });
    }
}
winrt::event_token
UwbSessionData::PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
{
    return m_propertyChanged.add(handler);
}
void
UwbSessionData::PropertyChanged(winrt::event_token const& token) noexcept
{
    m_propertyChanged.remove(token);
}
} // namespace winrt::OobSimulator::implementation
