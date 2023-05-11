// clang-format off
#include "pch.h"
#include "DelegateCommand.h"
// clang-format on

using namespace winrt::Windows::Foundation;

namespace winrt::OobSimulator::implementation
{
DelegateCommand::DelegateCommand(std::function<void(IInspectable)> const& execute, std::function<bool(IInspectable)> const& canExecute)
{
    if (!execute) {
        throw winrt::hresult_invalid_argument(L"execute");
    }

    m_executeDelegate = execute;
    m_canExecuteDelegate = canExecute;
}

void
DelegateCommand::Execute(IInspectable const& parameter)
{
    if (m_executeDelegate) {
        m_executeDelegate(parameter);
    }
}

bool
DelegateCommand::CanExecute(IInspectable const& parameter)
{
    if (!m_canExecuteDelegate) {
        return true;
    }

    bool canExecute = m_canExecuteDelegate(parameter);
    if (m_lastCanExecute != canExecute) {
        m_lastCanExecute = canExecute;
        RaiseCanExecuteChanged();
    }

    return m_lastCanExecute;
}

winrt::event_token
DelegateCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
{
    return m_canExecuteChanged.add(handler);
}

void
DelegateCommand::CanExecuteChanged(winrt::event_token const& token)
{
    m_canExecuteChanged.remove(token);
}

void
DelegateCommand::RaiseCanExecuteChanged()
{
    m_canExecuteChanged(*this, IInspectable());
}
} // namespace winrt::OobSimulator::implementation
