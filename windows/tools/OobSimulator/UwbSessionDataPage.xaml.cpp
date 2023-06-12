// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "UwbSessionDataPage.xaml.h"
#if __has_include("UwbSessionDataPage.g.cpp")
#include "UwbSessionDataPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::OobSimulator::implementation
{
UwbSessionDataPage::UwbSessionDataPage()
{
    InitializeComponent();
}

winrt::OobSimulator::UwbSessionDataPageViewModel
UwbSessionDataPage::ViewModel()
{
    return m_viewModel;
}

// TODO: Move to IValueConverter for radio buttons to conform to MVVM principles
void
UwbSessionDataPage::OnSelectSessionDataEncoding(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
{
    auto radioButton = sender.as<Controls::RadioButton>();
    if (radioButton == TlvBerRadioButton()) {
        ViewModel().UwbSessionData().SessionDataEncoding(winrt::OobSimulator::SessionDataEncoding::TlvBer);
    } else if (radioButton == MsgpackRadioButton()) {
        ViewModel().UwbSessionData().SessionDataEncoding(winrt::OobSimulator::SessionDataEncoding::MessagePack);
    }
}
} // namespace winrt::OobSimulator::implementation
