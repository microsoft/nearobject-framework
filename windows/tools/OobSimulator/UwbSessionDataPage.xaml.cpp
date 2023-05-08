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
} // namespace winrt::OobSimulator::implementation
