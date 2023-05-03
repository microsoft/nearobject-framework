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
