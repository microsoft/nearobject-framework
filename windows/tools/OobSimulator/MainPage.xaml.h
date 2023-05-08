// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#ifndef MAIN_PAGE_XAML_H
#define MAIN_PAGE_XAML_H

#include <string>
#include <utility>
#include <vector>

#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"
#include "winrt/Windows.UI.Input.h"
#include "winrt/Windows.UI.Xaml.Input.h"
#include "winrt/Windows.UI.Xaml.Media.Animation.h"
#include <winrt/Windows.UI.Core.h>

#include "MainPage.g.h"

namespace muxc
{
using namespace winrt::Microsoft::UI::Xaml::Controls;
};

namespace winrt::OobSimulator::implementation
{
struct MainPage : MainPageT<MainPage>
{
    MainPage();

    double
    NavViewCompactModeThresholdWidth();

    void
    NavView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

    void
    NavView_ItemInvoked(Windows::Foundation::IInspectable const& /* sender */, muxc::NavigationViewItemInvokedEventArgs const& args);

    void
    NavView_BackRequested(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& sender, winrt::Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const& args);

    void
    NavView_Navigate(std::wstring navItemTag, Microsoft::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo);

    void
    ContentFrame_NavigationFailed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const& args);

    void
    ContentFrame_Navigated(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& e);

    bool
    TryGoBack();

private:
    // Vector of std::pair holding the Navigation Tag and the relative Navigation Page.
    std::vector<std::pair<std::wstring, Windows::UI::Xaml::Interop::TypeName>> m_pages;
};
} // namespace winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct MainPage : MainPageT<MainPage, implementation::MainPage>
{
};
} // namespace winrt::OobSimulator::factory_implementation

#endif // MAIN_PAGE_XAML_H
