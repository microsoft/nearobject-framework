
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

rapidjson::Value
NearObjectProfile::ToJson(rapidjson::Document::AllocatorType& allocator) const
{
    Value jsonValue(rapidjson::kObjectType);
    std::string scopeString = NearObjectConnectionScope_ToString(Scope);
    jsonValue.AddMember("Scope", rapidjson::Value().SetString(scopeString.c_str(), static_cast<rapidjson::SizeType>(scopeString.size()), allocator), allocator);
    if (Security) {
        auto jsonValueSecurity = Security->ToJson(allocator);
        jsonValue.AddMember("Security", jsonValueSecurity, allocator);
    }
    return std::move(jsonValue);
}

persist::ParseResult
NearObjectProfile::ParseAndSet(const rapidjson::Value& value)
{
    {
        rapidjson::Value::ConstMemberIterator itr = value.FindMember("Scope");
        if (itr == value.MemberEnd()) {
            return persist::ParseResult::Failed;
        }
        NearObjectProfile::Scope = NearObjectConnectionScope_FromString(itr->value.GetString());
        if (NearObjectProfile::Scope == NearObjectConnectionScope::Unknown) {
            return persist::ParseResult::Failed;
        }
    }
    {
        rapidjson::Value::ConstMemberIterator itr = value.FindMember("Security");
        if (itr == value.MemberEnd()) {
            return persist::ParseResult::Succeeded;
        }
        NearObjectConnectionProfileSecurity security;
        auto result = security.ParseAndSet(itr->value);
        if (result != persist::ParseResult::Succeeded) {
            return result;
        }
        NearObjectProfile::Security.emplace(std::move(security));
    }
    return persist::ParseResult::Succeeded;
}

bool
NearObjectProfile::IsSame(const NearObjectProfile& other) const noexcept
{
    if (Scope != other.Scope) {
        return false;
    }
    if (Security != other.Security) {
        return false;
    }
    return true;
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
