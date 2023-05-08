// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "UwbSessionDataPage.g.h"

namespace winrt::OobSimulator::implementation
{
struct UwbSessionDataPage : UwbSessionDataPageT<UwbSessionDataPage>
{
    UwbSessionDataPage();

    winrt::OobSimulator::UwbSessionDataPageViewModel
    ViewModel();

private:
    winrt::OobSimulator::UwbSessionDataPageViewModel m_viewModel;
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct UwbSessionDataPage : UwbSessionDataPageT<UwbSessionDataPage, implementation::UwbSessionDataPage>
{
};
} // namespace winrt::OobSimulator::factory_implementation
