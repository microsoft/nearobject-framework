// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "MainPage.xaml.h"
#if __has_include("MainPage.g.cpp")
#include "MainPage.g.cpp"
#endif

#include "UwbSessionDataPage.xaml.h"
#include "winrt/Microsoft.UI.Xaml.Media.Animation.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::OobSimulator::implementation
{
MainPage::MainPage()
{
    InitializeComponent();

    // Map of page string tags to their corresponding xaml type.
    m_pages.push_back(std::make_pair<std::wstring, Windows::UI::Xaml::Interop::TypeName>(L"uwbsessiondata", winrt::xaml_typename<OobSimulator::UwbSessionDataPage>()));
}

double
MainPage::NavViewCompactModeThresholdWidth()
{
    return NavView().CompactModeThresholdWidth();
}

void
winrt::OobSimulator::implementation::MainPage::NavView_Loaded(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*e*/)
{
    // Add handler for ContentFrame navigation.
    ContentFrame().Navigated({ this, &MainPage::ContentFrame_Navigated });

    // NavView doesn't load any page by default, so load home page.
    NavView().SelectedItem(NavView().MenuItems().GetAt(0));

    // If navigation occurs on SelectionChanged, then this isn't needed.
    // Because we use ItemInvoked to navigate, we need to call Navigate
    // here to load the home page.
    // Load the first page.
    NavView_Navigate(m_pages.front().first, Microsoft::UI::Xaml::Media::Animation::EntranceNavigationTransitionInfo());
}

void
MainPage::NavView_ItemInvoked(Windows::Foundation::IInspectable const& /* sender */, muxc::NavigationViewItemInvokedEventArgs const& args)
{
    if (args.IsSettingsInvoked()) {
        // TODO: settings page
        // NavView_Navigate(L"settings", args.RecommendedNavigationTransitionInfo());
    } else if (args.InvokedItemContainer()) {
        NavView_Navigate(winrt::unbox_value_or<winrt::hstring>(args.InvokedItemContainer().Tag(), L"").c_str(), args.RecommendedNavigationTransitionInfo());
    }
}

void
MainPage::NavView_Navigate(std::wstring navItemTag, Microsoft::UI::Xaml::Media::Animation::NavigationTransitionInfo const& transitionInfo)
{
    Windows::UI::Xaml::Interop::TypeName pageTypeName;

    // TOOD: settings page
    // if (navItemTag == L"settings")
    //{
    //    pageTypeName = winrt::xaml_typename<NavigationViewCppWinRT::SettingsPage>();
    //}
    // else
    {
        for (auto&& eachPage : m_pages) {
            if (eachPage.first == navItemTag) {
                pageTypeName = eachPage.second;
                break;
            }
        }
    }
    // Get the page type before navigation so you can prevent duplicate
    // entries in the backstack.
    Windows::UI::Xaml::Interop::TypeName preNavPageType = ContentFrame().CurrentSourcePageType();

    // Navigate only if the selected page isn't currently loaded.
    if (pageTypeName.Name != L"" && preNavPageType.Name != pageTypeName.Name) {
        ContentFrame().Navigate(pageTypeName, nullptr, transitionInfo);
    }
}

void
winrt::OobSimulator::implementation::MainPage::NavView_BackRequested(winrt::Microsoft::UI::Xaml::Controls::NavigationView const& /*sender*/, winrt::Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs const& /*args*/)
{
    TryGoBack();
}

void
winrt::OobSimulator::implementation::MainPage::ContentFrame_NavigationFailed(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Microsoft::UI::Xaml::Navigation::NavigationFailedEventArgs const& args)
{
    throw winrt::hresult_error(E_FAIL, winrt::hstring(L"Failed to load Page ") + args.SourcePageType().Name);
}

void
winrt::OobSimulator::implementation::MainPage::ContentFrame_Navigated(winrt::Windows::Foundation::IInspectable const& /*sender*/, winrt::Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& args)
{
    NavView().IsBackEnabled(ContentFrame().CanGoBack());

    // TODO: settings page
    // if (ContentFrame().SourcePageType().Name ==
    //    winrt::xaml_typename<NavigationViewCppWinRT::SettingsPage>().Name)
    //{
    //    // SettingsItem is not part of NavView.MenuItems, and doesn't have a Tag.
    //    NavView().SelectedItem(NavView().SettingsItem().as<muxc::NavigationViewItem>());
    //    NavView().Header(winrt::box_value(L"Settings"));
    //}
    // else if (ContentFrame().SourcePageType().Name != L"")
    if (ContentFrame().SourcePageType().Name != L"") {
        for (auto&& eachPage : m_pages) {
            if (eachPage.second.Name == args.SourcePageType().Name) {
                for (auto&& eachMenuItem : NavView().MenuItems()) {
                    auto navigationViewItem = eachMenuItem.try_as<muxc::NavigationViewItem>();
                    {
                        if (navigationViewItem) {
                            winrt::hstring hstringValue = winrt::unbox_value_or<winrt::hstring>(navigationViewItem.Tag(), L"");
                            if (hstringValue == eachPage.first) {
                                NavView().SelectedItem(navigationViewItem);
                                NavView().Header(navigationViewItem.Content());
                            }
                        }
                    }
                }
                break;
            }
        }
    }
}

bool
MainPage::TryGoBack()
{
    if (!ContentFrame().CanGoBack())
        return false;

    // Don't go back if the nav pane is overlayed.
    if (NavView().IsPaneOpen() &&
        (NavView().DisplayMode() == muxc::NavigationViewDisplayMode::Compact || NavView().DisplayMode() == muxc::NavigationViewDisplayMode::Minimal))
        return false;

    ContentFrame().GoBack();
    return true;
}

} // namespace winrt::OobSimulator::implementation
