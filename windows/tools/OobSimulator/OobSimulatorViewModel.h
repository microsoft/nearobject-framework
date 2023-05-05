#ifndef OOB_SIMULATOR_VIEW_MODEL_H
#define OOB_SIMULATOR_VIEW_MODEL_H

// clang-format off
#include "OobSimulatorViewModel.g.h"
#include "BindableBase.h"
#include "DelegateCommand.h"
#include "UwbSessionData.h"
// clang-format on
#include <uwb/protocols/fira/FiraDevice.hxx>

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

    uwb::protocol::fira::DeviceRole
    ConvertDeviceRole();

    uwb::protocol::fira::MultiNodeMode
    ConvertMultiNodeMode();
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct OobSimulatorViewModel : OobSimulatorViewModelT<OobSimulatorViewModel, implementation::OobSimulatorViewModel>
{
};
} // namespace winrt::OobSimulator::factory_implementation

#endif // OOB_SIMULATOR_VIEW_MODEL_H
