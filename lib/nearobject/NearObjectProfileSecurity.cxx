#include "NearObjectProfileSecurity.hxx"

rapidjson::Value
nearobject::NearObjectConnectionProfileSecurity::to_serial(rapidjson::Document::AllocatorType& allocator) const {
    rapidjson::Value v(rapidjson::kObjectType);
    return v;
}

persist::ParseResult 
nearobject::NearObjectConnectionProfileSecurity::parse_and_set(const rapidjson::Value& value) {
    return persist::ParseResult::Succeeded;
}

bool nearobject::NearObjectConnectionProfileSecurity::profiles_match(const NearObjectConnectionProfileSecurity& p1, const NearObjectConnectionProfileSecurity& p2){
    return true;
}