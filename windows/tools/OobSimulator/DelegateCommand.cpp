// clang-format off
#include "pch.h"
#include "DelegateCommand.h"
// clang-format on

using namespace winrt::Windows::Foundation;

namespace winrt::OobSimulator::implementation
{
DelegateCommand::DelegateCommand(std::function<void(IInspectable)> const& execute, std::function<bool(IInspectable)> const& canExecute)
{
    if (execute == nullptr) {
        throw winrt::hresult_invalid_argument(L"execute");
    }

    m_execute = execute;
    m_canExecute = canExecute;
}

void
DelegateCommand::Execute(IInspectable const& parameter)
{
    if (m_execute != nullptr) {
        m_execute(parameter);
    }
}

bool
DelegateCommand::CanExecute(IInspectable const& parameter)
{
    if (m_canExecute == nullptr) {
        return true;
    }

    bool canExecute = m_canExecute(parameter);
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
