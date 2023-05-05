#ifndef DELEGATE_COMMAND_H
#define DELEGATE_COMMAND_H

#include "DelegateCommand.g.h"
#include <functional>

namespace winrt::OobSimulator::implementation
{
struct DelegateCommand : DelegateCommandT<DelegateCommand>
{
    DelegateCommand(std::function<void(winrt::Windows::Foundation::IInspectable)> const& execute, std::function<bool(winrt::Windows::Foundation::IInspectable)> const& canExecute = nullptr);

    void
    Execute(winrt::Windows::Foundation::IInspectable const& parameter);

    bool
    CanExecute(winrt::Windows::Foundation::IInspectable const& parameter);

    winrt::event_token
    CanExecuteChanged(winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable> const& handler);

    void
    CanExecuteChanged(winrt::event_token const& token);

    void
    RaiseCanExecuteChanged();

private:
    std::function<void(winrt::Windows::Foundation::IInspectable)> m_executeDelegate;
    std::function<bool(winrt::Windows::Foundation::IInspectable)> m_canExecuteDelegate;
    bool m_lastCanExecute = true;

    winrt::event<winrt::Windows::Foundation::EventHandler<winrt::Windows::Foundation::IInspectable>> m_canExecuteChanged;
};
} // namespace winrt::OobSimulator::implementation

#endif // DELEGATE_COMMAND_H
