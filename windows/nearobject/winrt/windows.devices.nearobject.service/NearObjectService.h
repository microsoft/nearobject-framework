#pragma once
#include "NearObjectService.g.h"

namespace winrt::Windows::Devices::NearObject::Service::implementation
{
    struct NearObjectService : NearObjectServiceT<NearObjectService>
    {
        NearObjectService() = default;

    };
}
namespace winrt::Windows::Devices::NearObject::Service::factory_implementation
{
    struct NearObjectService : NearObjectServiceT<NearObjectService, implementation::NearObjectService>
    {
    };
}
