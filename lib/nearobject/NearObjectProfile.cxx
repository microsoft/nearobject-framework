
#include "NearObjectProfile.hxx"

std::string nearobject::NearObjectConnectionScope_ToString(NearObjectConnectionScope s){
    switch(s){
        case NearObjectConnectionScope::Unicast: return "Unicast";
        case NearObjectConnectionScope::Multicast: return "Multicast";
        default: return "Unknown";
    }
}

rapidjson::Value
nearobject::NearObjectProfile::to_serial(rapidjson::Document::AllocatorType& allocator) {
    Value v(rapidjson::kObjectType);
    auto ScopeString = nearobject::NearObjectConnectionScope_ToString(Scope);
    v.AddMember("Scope",rapidjson::Value().SetString(ScopeString.c_str(),ScopeString.size(),allocator),allocator);
    if(Security){
        auto v_security = (*Security).to_serial(allocator);
        v.AddMember("Security",v_security,allocator);
    }
    return std::move(v);
}

