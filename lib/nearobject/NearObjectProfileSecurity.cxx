#include "NearObjectProfileSecurity.hxx"

rapidjson::Value
nearobject::NearObjectConnectionProfileSecurity::to_json(rapidjson::Document::AllocatorType& allocator) const
{
    rapidjson::Value v(rapidjson::kObjectType);
    return v;
}

persist::ParseResult
nearobject::NearObjectConnectionProfileSecurity::ParseAndSet(const rapidjson::Value& value)
{
    return persist::ParseResult::Succeeded;
}

bool
nearobject::NearObjectConnectionProfileSecurity::IsSame(const NearObjectConnectionProfileSecurity& other) const noexcept
{
    return true;
}

bool
nearobject::operator==(const NearObjectConnectionProfileSecurity& lhs, const NearObjectConnectionProfileSecurity& rhs) noexcept
{
    return lhs.IsSame(rhs);
}

bool
nearobject::operator!=(const NearObjectConnectionProfileSecurity& lhs, const NearObjectConnectionProfileSecurity& rhs) noexcept
{
    return !(lhs == rhs);
}