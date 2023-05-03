// Copyright (c) Microsoft Corporation and Contributors.

#pragma once

#include "MainWindow.g.h"
#include "OobSimulatorViewModel.h"

namespace winrt::OobSimulator::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();

    OobSimulator::OobSimulatorViewModel
    MainViewModel();
    void
    OnSetUwbSessionDataButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

private:
    OobSimulator::OobSimulatorViewModel m_mainViewModel;
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::OobSimulator::factory_implementation
