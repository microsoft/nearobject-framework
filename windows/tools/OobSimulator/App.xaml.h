// Copyright (c) Microsoft Corporation and Contributors.

#ifndef APP_XAML_H
#define APP_XAML_H

#include "App.xaml.g.h"

namespace winrt::OobSimulator::implementation
{
struct App : AppT<App>
{
    App();

    void
    OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const &);

private:
    winrt::Microsoft::UI::Xaml::Window window{ nullptr };
};
} // namespace winrt::OobSimulator::implementation

#endif // APP_XAML_H
