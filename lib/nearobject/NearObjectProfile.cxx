
#include <sstream>
#include <tuple>

#include <magic_enum.hpp>
#include <notstd/tostring.hxx>

#include <nearobject/NearObjectProfile.hxx>

using namespace nearobject;
using namespace strings::ostream_operators;

NearObjectProfile::NearObjectProfile(NearObjectConnectionScope scope) :
    m_scope(scope)
{}

NearObjectProfile::NearObjectProfile(NearObjectConnectionScope scope, NearObjectProfileSecurity security) :
    m_scope(scope),
    m_security(std::move(security))
{}

NearObjectConnectionScope
NearObjectProfile::GetScope() const noexcept
{
    return m_scope;
}

const std::optional<NearObjectProfileSecurity>
NearObjectProfile::GetSecurity() const noexcept
{
    return m_security;
}

bool
NearObjectProfile::IsSame(const NearObjectProfile& other) const noexcept
{
    return std::tie(this->m_scope, this->m_security) == std::tie(other.m_scope, other.m_security);
}

std::string
NearObjectProfile::ToString() const noexcept
{
    std::ostringstream profileString;
    profileString << "Scope: " << magic_enum::enum_name(m_scope);
    profileString << ", Security: " << m_security;

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
