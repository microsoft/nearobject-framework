// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "SettingsPage.g.h"

namespace winrt::OobSimulator::implementation
{
struct SettingsPage : SettingsPageT<SettingsPage>
{
    SettingsPage();

    winrt::OobSimulator::SettingsPageViewModel
    ViewModel();

private:
    winrt::OobSimulator::SettingsPageViewModel m_viewModel;
};
} // winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage>
{
};
} // winrt::OobSimulator::factory_implementation
