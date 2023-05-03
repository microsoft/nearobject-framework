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

OobSimulator::OobSimulatorViewModel
MainWindow::MainViewModel()
{
    return m_mainViewModel;
}

void
winrt::OobSimulator::implementation::MainWindow::OnSetUwbSessionDataButtonClick(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    MainViewModel().SetUwbSessionDataCommand();
}
} // namespace winrt::OobSimulator::implementation
