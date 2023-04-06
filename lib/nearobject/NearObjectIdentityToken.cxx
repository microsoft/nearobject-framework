
#include <algorithm>
#include <ios>
#include <ranges>
#include <sstream>
#include <typeinfo>

#include <nearobject/NearObjectIdentityToken.hxx>

using namespace nearobject;

bool
NearObjectIdentityToken::IsEqual(const NearObjectIdentityToken& rhs) const noexcept
{
    return std::ranges::equal(this->GetToken(), rhs.GetToken());
}

std::string
NearObjectIdentityToken::ToString() const
{
    std::ostringstream tokenStr{};
    const auto& token = GetToken();

    tokenStr << std::hex;
    for (const auto& b : token.first(token.size() - 1)) {
        tokenStr << +b << ':';
    }
    tokenStr << +token[token.size() - 1];

    return tokenStr.str();
}

bool
nearobject::operator==(const NearObjectIdentityToken& lhs, const NearObjectIdentityToken& rhs) noexcept
{
    return (typeid(lhs) == typeid(rhs)) && lhs.IsEqual(rhs);
}

bool
nearobject::operator!=(const NearObjectIdentityToken& lhs, const NearObjectIdentityToken& rhs) noexcept
{
    return !(lhs == rhs);
}
