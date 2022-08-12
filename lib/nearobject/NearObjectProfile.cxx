
#include "NearObjectProfile.hxx"

std::string
nearobject::NearObjectConnectionScope_ToString(NearObjectConnectionScope s)
{
    switch (s) {
        case NearObjectConnectionScope::Unicast:
            return "Unicast";
        case NearObjectConnectionScope::Multicast:
            return "Multicast";
        default:
            return "Unknown";
    }
}

nearobject::NearObjectConnectionScope
nearobject::NearObjectConnectionScope_FromString(const std::string& s)
{
    if (s == "Unicast") {
        return NearObjectConnectionScope::Unicast;
    } else if (s == "Multicast") {
        return NearObjectConnectionScope::Multicast;
    } else {
        return NearObjectConnectionScope::Unknown;
    }
}

rapidjson::Value
nearobject::NearObjectProfile::ToJson(rapidjson::Document::AllocatorType& allocator) const
{
    Value v(rapidjson::kObjectType);
    auto ScopeString = nearobject::NearObjectConnectionScope_ToString(Scope);
    v.AddMember("Scope", rapidjson::Value().SetString(ScopeString.c_str(), ScopeString.size(), allocator), allocator);
    if (Security) {
        auto v_security = Security->ToJson(allocator);
        v.AddMember("Security", v_security, allocator);
    }
    return std::move(v);
}

persist::ParseResult
nearobject::NearObjectProfile::ParseAndSet(const rapidjson::Value& value)
{
    {
        rapidjson::Value::ConstMemberIterator itr = value.FindMember("Scope");
        if (itr == value.MemberEnd()) {
            return persist::ParseResult::Failed;
        }
        NearObjectProfile::Scope = nearobject::NearObjectConnectionScope_FromString(itr->value.GetString());
        if (NearObjectProfile::Scope == NearObjectConnectionScope::Unknown) {
            return persist::ParseResult::Failed;
        }
    }
    {
        rapidjson::Value::ConstMemberIterator itr = value.FindMember("Security");
        if (itr == value.MemberEnd()) {
            return persist::ParseResult::Succeeded;
        }
        nearobject::NearObjectConnectionProfileSecurity sec;
        auto result = sec.ParseAndSet(itr->value);
        if (result != persist::ParseResult::Succeeded) {
            return result;
        }
        *NearObjectProfile::Security = std::move(sec);
    }
    return persist::ParseResult::Succeeded;
}

bool
nearobject::NearObjectProfile::IsSame(const NearObjectProfile& other) const noexcept
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