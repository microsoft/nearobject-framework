#pragma once
#include "NearObject.g.h"

namespace winrt::Windows::Devices::NearObject::implementation
{
    struct NearObject : NearObjectT<NearObject>
    {
        NearObject() = default;

    };
}
namespace winrt::Windows::Devices::NearObject::factory_implementation
{
    struct NearObject : NearObjectT<NearObject, implementation::NearObject>
    {
    };
}
