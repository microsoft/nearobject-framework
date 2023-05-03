// Copyright (c) Microsoft Corporation and Contributors.

// clang-format off
#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
// clang-format on

#include <Microsoft.UI.Xaml.Window.h>
#include <shobjidl.h>

namespace winrt::OobSimulator::implementation
{
MainWindow::MainWindow()
{
    InitializeComponent();

    // Retrieve the window handle (HWND) of the current WinUI 3 window and store globally
    auto windowNative{ this->try_as<::IWindowNative>() };
    winrt::check_bool(windowNative);
    windowNative->get_WindowHandle(&g_hwnd);
}

OobSimulator::OobSimulatorViewModel
MainWindow::MainViewModel()
{
    return m_mainViewModel;
}
} // namespace winrt::OobSimulator::implementation
