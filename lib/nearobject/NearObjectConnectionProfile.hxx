
#ifndef __NEAR_OBJECT_CONNECTION_PROFILE_HXX__
#define __NEAR_OBJECT_CONNECTION_PROFILE_HXX__

#include <optional>

#include "NearObjectConnectionProfileSecurity.hxx"

namespace nearobject
{
enum NearObjectConnectionScope
{
    Unicast,
    Multicast,
};

struct NearObjectConnectionProfile
{
    NearObjectConnectionScope Scope { NearObjectConnectionScope::Unicast };
    std::optional<NearObjectConnectionProfileSecurity> Security{ std::nullopt };
};

} // namespace nearobject

#endif // __NEAR_OBJECT_CONNECTION_PROFILE_HXX__
