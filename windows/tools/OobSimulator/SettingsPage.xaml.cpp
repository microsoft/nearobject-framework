// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "SettingsPage.xaml.h"
#if __has_include("SettingsPage.g.cpp")
#include "SettingsPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::OobSimulator::implementation
{
SettingsPage::SettingsPage()
{
    InitializeComponent();
}

winrt::OobSimulator::SettingsPageViewModel
SettingsPage::ViewModel()
{
    return m_viewModel;
}
} // namespace winrt::OobSimulator::implementation
