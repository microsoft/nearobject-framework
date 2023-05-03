// Copyright (c) Microsoft Corporation and Contributors.

// clang-format off
#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
// clang-format on

namespace winrt::OobSimulator::implementation
{
MainWindow::MainWindow()
{
    InitializeComponent();
}

void
MainWindow::ClickHandler(Windows::Foundation::IInspectable const& /* sender */, Microsoft::UI::Xaml::RoutedEventArgs const& /* args */)
{
    MainViewModel().UwbSessionData().SessionId(L"1234"); // TODO: I don't think we need this ClickHandler
}

OobSimulator::OobSimulatorViewModel
MainWindow::MainViewModel()
{
    return m_mainViewModel;
}
} // namespace winrt::OobSimulator::implementation
