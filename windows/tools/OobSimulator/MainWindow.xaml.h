// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"
#include "uwb/protocols/fira/UwbSessionData.hxx"

namespace winrt::OobSimulator::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();

    winrt::Windows::Foundation::IAsyncAction
    SetUwbSessionData(uwb::protocol::fira::UwbSessionData const& uwbSessionData);
    winrt::Windows::Foundation::IAsyncAction
    SetUwbSessionData_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::OobSimulator::factory_implementation
