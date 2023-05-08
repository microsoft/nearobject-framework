// Copyright (c) Microsoft Corporation and Contributors.

#ifndef MAIN_WINDOW_XAML_H
#define MAIN_WINDOW_XAML_H

#include "MainWindow.g.h"
#include "OobSimulatorViewModel.h"

namespace winrt::OobSimulator::implementation
{
struct MainWindow : MainWindowT<MainWindow>
{
    MainWindow();
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
{
};
} // namespace winrt::OobSimulator::factory_implementation

#endif // MAIN_WINDOW_XAML_H
