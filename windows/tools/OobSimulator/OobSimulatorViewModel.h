#pragma once

// clang-format off
#include "OobSimulatorViewModel.g.h"
#include "BindableBase.h"
#include "DelegateCommand.h"
#include "UwbSessionData.h"
// clang-format on

namespace winrt::OobSimulator::implementation
{
struct OobSimulatorViewModel : OobSimulatorViewModelT<OobSimulatorViewModel, BindableBase>
{
    OobSimulatorViewModel();

    winrt::OobSimulator::UwbSessionData
    UwbSessionData();
    winrt::Microsoft::UI::Xaml::Input::ICommand
    SetUwbSessionDataCommand();

private:
    winrt::OobSimulator::UwbSessionData m_uwbSessionData{ nullptr };
    OobSimulator::DelegateCommand m_setUwbSessionData{ nullptr };

    winrt::Windows::Foundation::IAsyncAction
    SetUwbSessionData();
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct OobSimulatorViewModel : OobSimulatorViewModelT<OobSimulatorViewModel, implementation::OobSimulatorViewModel>
{
};
} // namespace winrt::OobSimulator::factory_implementation
