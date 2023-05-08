// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#ifndef MAIN_PAGE_H
#define MAIN_PAGE_H

#include <string>
#include <utility>
#include <vector>

#include "winrt/Windows.UI.Xaml.Input.h"
#include "winrt/Windows.UI.Xaml.Media.Animation.h"
#include "winrt/Microsoft.UI.Xaml.Controls.h"
#include "winrt/Microsoft.UI.Xaml.XamlTypeInfo.h"
#include <winrt/Windows.UI.Core.h>
#include "winrt/Windows.UI.Input.h"

#include "MainPage.g.h"

namespace muxc
{
    using namespace winrt::Microsoft::UI::Xaml::Controls;
};

namespace wuxc
{
    using namespace winrt::Windows::UI::Xaml::Controls;
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

    void
    CoreDispatcher_AcceleratorKeyActivated(Windows::UI::Core::CoreDispatcher const& /* sender */, Windows::UI::Core::AcceleratorKeyEventArgs const& args);
    
    void
    CoreWindow_PointerPressed(Windows::UI::Core::CoreWindow const& /* sender */, Windows::UI::Core::PointerEventArgs const& args);
    
    void 
    System_BackRequested(Windows::Foundation::IInspectable const& /* sender */, Windows::UI::Core::BackRequestedEventArgs const& args);
    
    bool
    TryGoBack();

private:
    // Vector of std::pair holding the Navigation Tag and the relative Navigation Page.
    std::vector<std::pair<std::wstring, Windows::UI::Xaml::Interop::TypeName>> m_pages;
};
} // winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct MainPage : MainPageT<MainPage, implementation::MainPage>
{
};
} // winrt::OobSimulator::factory_implementation

#endif // MAIN_PAGE_H
