
#pragma once
#include "NearObjectSession.g.h"

namespace winrt::Windows::Devices::NearObject::implementation
{
    struct NearObjectSession : NearObjectSessionT<NearObjectSession>
    {
        NearObjectSession() = default;

    };
}
namespace winrt::Windows::Devices::NearObject::factory_implementation
{
    struct NearObjectSession : NearObjectSessionT<NearObjectSession, implementation::NearObjectSession>
    {
    };
}
