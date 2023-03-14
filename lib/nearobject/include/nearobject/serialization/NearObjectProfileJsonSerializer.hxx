
#ifndef NEAR_OBJECT_PROFILE_JSON_SERIALIZER
#define NEAR_OBJECT_PROFILE_JSON_SERIALIZER

#include <nlohmann/json.hpp>

namespace nearobject
{
struct NearObjectProfile;
struct NearObjectProfileSecurity;

// Implementations of required functions for use with nlohmann json conversion.
void
to_json(nlohmann::json& json, const NearObjectProfileSecurity& security);

void
from_json(const nlohmann::json& json, NearObjectProfileSecurity& security);

void
to_json(nlohmann::json& json, const NearObjectProfile& profile); 

void
from_json(const nlohmann::json& json, NearObjectProfile& profile); 
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_JSON_SERIALIZER
