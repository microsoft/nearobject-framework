
#include <sstream>
#include <tuple>

#include <strings/tostring.hxx>

#include <nearobject/NearObjectProfile.hxx>

using namespace nearobject;

auto
nearobject::NearObjectConnectionScope_ToString(NearObjectConnectionScope scope)
{
    switch (scope) {
    case NearObjectConnectionScope::Unicast:
        return "Unicast";
    case NearObjectConnectionScope::Multicast:
        return "Multicast";
    default:
        return "Unknown";
    }
}

nearobject::NearObjectConnectionScope
nearobject::NearObjectConnectionScope_FromString(const std::string& scope)
{
    if (scope == "Unicast") {
        return NearObjectConnectionScope::Unicast;
    } else if (scope == "Multicast") {
        return NearObjectConnectionScope::Multicast;
    } else {
        return NearObjectConnectionScope::Unknown;
    }
}

NearObjectProfile::NearObjectProfile(NearObjectConnectionScope scope) :
    Scope(scope)
{}

NearObjectProfile::NearObjectProfile(NearObjectConnectionScope scope, NearObjectProfileSecurity security) :
    Scope(scope),
    Security(std::move(security))
{}

bool
NearObjectProfile::IsSame(const NearObjectProfile& other) const noexcept
{
    return std::tie(this->Scope, this->Security)
        == std::tie(other.Scope, other.Security);
}

std::string
NearObjectProfile::ToString() const noexcept
{
    std::ostringstream profileString;
    profileString << "Scope: " << NearObjectConnectionScope_ToString(Scope);
    profileString << ", Security: " << Security;

    return profileString.str();
}

bool
nearobject::operator==(const NearObjectProfile& lhs, const NearObjectProfile& rhs) noexcept
{
    return lhs.IsSame(rhs);
}

bool
nearobject::operator!=(const NearObjectProfile& lhs, const NearObjectProfile& rhs) noexcept
{
    return !(lhs == rhs);
}
