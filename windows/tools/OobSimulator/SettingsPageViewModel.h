
#ifndef SETTINGS_PAGE_VIEW_MODEL_H
#define SETTINGS_PAGE_VIEW_MODEL_H

#include "SettingsPageViewModel.g.h"
#include "BindableBase.h"

namespace winrt::OobSimulator::implementation
{
struct SettingsPageViewModel : SettingsPageViewModelT<SettingsPageViewModel, OobSimulator::implementation::BindableBase>
{
    SettingsPageViewModel() = default;
};
} // winrt::OobSimulator::implementation

namespace winrt::OobSimulator::factory_implementation
{
struct SettingsPageViewModel : SettingsPageViewModelT<SettingsPageViewModel, implementation::SettingsPageViewModel>
{
};
} // winrt::OobSimulator::factory_implementation

#endif // SETTINGS_PAGE_VIEW_MODEL_H
