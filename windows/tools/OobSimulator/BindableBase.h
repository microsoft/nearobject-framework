#pragma once

#include "BindableBase.g.h"

namespace winrt::OobSimulator::implementation
{
struct BindableBase : BindableBaseT<BindableBase>
{
    void
    RaisePropertyChanged(hstring const& propertyName);
    winrt::event_token
    PropertyChanged(winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
    void
    PropertyChanged(winrt::event_token const& token) noexcept;

private:
    event<winrt::Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct BindableBase : BindableBaseT<BindableBase, implementation::BindableBase>
{
};
} // namespace winrt::OobSimulator::factory_implementation
