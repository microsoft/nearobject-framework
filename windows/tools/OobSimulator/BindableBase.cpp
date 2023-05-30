// clang-format off
#include "pch.h"
#include "BindableBase.h"
#include "BindableBase.g.cpp"
// clang-format on

using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::OobSimulator::implementation
{
void
BindableBase::RaisePropertyChanged(hstring const& propertyName)
{
    m_propertyChanged(*this, PropertyChangedEventArgs(propertyName));
}

winrt::event_token
BindableBase::PropertyChanged(PropertyChangedEventHandler const& handler)
{
    return m_propertyChanged.add(handler);
}

void
BindableBase::PropertyChanged(winrt::event_token const& token) noexcept
{
    m_propertyChanged.remove(token);
}
} // namespace winrt::OobSimulator::implementation
