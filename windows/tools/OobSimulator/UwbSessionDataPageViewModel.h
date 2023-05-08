
#ifndef UWB_SESSION_DATA_PAGE_VIEW_MODEL_H
#define UWB_SESSION_DATA_PAGE_VIEW_MODEL_H

// clang-format off
#include "UwbSessionDataPageViewModel.g.h"
#include "BindableBase.h"

#include "DelegateCommand.h"
#include "UwbSessionData.h"
// clang-format on
#include <uwb/protocols/fira/FiraDevice.hxx>

namespace winrt::OobSimulator::implementation
{
struct UwbSessionDataPageViewModel : UwbSessionDataPageViewModelT<UwbSessionDataPageViewModel, OobSimulator::implementation::BindableBase>
{
    UwbSessionDataPageViewModel();

    winrt::OobSimulator::UwbSessionData
    UwbSessionData();
    
    winrt::Microsoft::UI::Xaml::Input::ICommand
    SetUwbSessionDataCommand();

private:
    winrt::Windows::Foundation::IAsyncAction
    SetUwbSessionData();

    uwb::protocol::fira::DeviceRole
    ConvertDeviceRole();

    uwb::protocol::fira::MultiNodeMode
    ConvertMultiNodeMode();

private:
    winrt::OobSimulator::UwbSessionData m_uwbSessionData{ nullptr };
    OobSimulator::DelegateCommand m_setUwbSessionData{ nullptr };
};
} // winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct UwbSessionDataPageViewModel : UwbSessionDataPageViewModelT<UwbSessionDataPageViewModel, implementation::UwbSessionDataPageViewModel>
{
};
} // winrt::OobSimulator::factory_implementation

#endif // UWB_SESSION_DATA_PAGE_VIEW_MODEL_H
