
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
nearobject::NearObjectConnectionScope_FromString(std::string s)
{
    if (s == std::string("Unicast"))
        return NearObjectConnectionScope::Unicast;
    return NearObjectConnectionScope::Multicast; // TODO this means that the default is multicast
}

rapidjson::Value
nearobject::NearObjectProfile::to_json(rapidjson::Document::AllocatorType& allocator) const
{
    Value v(rapidjson::kObjectType);
    auto ScopeString = nearobject::NearObjectConnectionScope_ToString(Scope);
    v.AddMember("Scope", rapidjson::Value().SetString(ScopeString.c_str(), ScopeString.size(), allocator), allocator);
    if (Security) {
        auto v_security = (*Security).to_json(allocator);
        v.AddMember("Security", v_security, allocator);
    }
    return std::move(v);
}

persist::ParseResult
nearobject::NearObjectProfile::parse_and_set(const rapidjson::Value& value)
{
    {
        rapidjson::Value::ConstMemberIterator itr = value.FindMember("Scope");
        if (itr == value.MemberEnd())
            return persist::ParseResult::Failed;
        NearObjectProfile::Scope = nearobject::NearObjectConnectionScope_FromString(itr->value.GetString());
    }
    {
        rapidjson::Value::ConstMemberIterator itr = value.FindMember("Security");
        if (itr == value.MemberEnd())
            return persist::ParseResult::Succeeded;
        nearobject::NearObjectConnectionProfileSecurity sec;
        auto result = sec.parse_and_set(itr->value);
        if (result != persist::ParseResult::Succeeded)
            return result;
        *NearObjectProfile::Security = std::move(sec);
    }
    return persist::ParseResult::Succeeded;
}

bool
nearobject::NearObjectProfile::profiles_match(const NearObjectProfile& p1, const NearObjectProfile& p2)
{
    if (p1.Scope != p2.Scope)
        return false;
    if ((p1.Security == std::nullopt) != (p2.Security == std::nullopt))
        return false;
    if (p1.Security) {
        return nearobject::NearObjectConnectionProfileSecurity::profiles_match(*p1.Security, *p2.Security);
    }
    return true;
}
