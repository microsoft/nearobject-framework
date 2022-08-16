
#include <sstream>
#include <tuple>

#include <magic_enum.hpp>
#include <notstd/tostring.hxx>

#include <nearobject/NearObjectProfile.hxx>

using namespace nearobject;
using namespace strings::ostream_operators;

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
    profileString << "Scope: " << magic_enum::enum_name(Scope);
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
