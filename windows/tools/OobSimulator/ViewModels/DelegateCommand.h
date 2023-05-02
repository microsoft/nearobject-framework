// Copyright (c) Microsoft Corporation and Contributors.

#pragma once

#include <functional>
#include <winrt/Windows.UI.Xaml.Input.h>

namespace winrt::OobSimulator::ViewModels::implementation
{
struct DelegateCommand : public winrt::implements<DelegateCommand, winrt::Windows::UI::Xaml::Input::ICommand>
{
public:
    // Constructor
    DelegateCommand(std::function<void(winrt::Windows::Foundation::IInspectable)> const& execute, std::function<bool(winrt::Windows::Foundation::IInspectable)> const& canExecute = nullptr);

    // Functions
    void
    Execute(winrt::Windows::Foundation::IInspectable const& parameter);

    bool
    CanExecute(winrt::Windows::Foundation::IInspectable const& parameter);

    // Event Handlers
    winrt::event_token
    CanExecuteChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler);

    void
    CanExecuteChanged(winrt::event_token const& token);

private:
    std::function<void(winrt::Windows::Foundation::IInspectable)> m_execute;
    std::function<bool(winrt::Windows::Foundation::IInspectable)> m_canExecute;
    bool m_lastCanExecute = true;

    winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> m_canExecuteChanged;

    void
    RaiseCanExecuteChanged();
};
} // namespace winrt::OobSimulator::ViewModels::implementation
