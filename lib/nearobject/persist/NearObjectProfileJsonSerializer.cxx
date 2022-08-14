
#include <nearobject/persist/NearObjectProfileJsonSerializer.hxx>

using namespace nearobject;
using namespace nearobject::persistence;

namespace nearobject
{
// Implementations of required functions for use with nlohmann json conversion.
void
to_json(nlohmann::json& json, const NearObjectConnectionProfileSecurity& security) 
{
    json = nlohmann::json{};
}

void
from_json(const nlohmann::json& json, NearObjectConnectionProfileSecurity& security) 
{
}

void
to_json(nlohmann::json& json, const NearObjectProfile& profile) 
{
    json = nlohmann::json {
        { "Scope", profile.Scope },
        { "Security", profile.Security.value_or(NearObjectConnectionProfileSecurity{}) },
    };
}

void
from_json(const nlohmann::json& json, NearObjectProfile& profile) 
{
    profile.Security.emplace();
    json.at("Scope").get_to(profile.Scope);
    json.at("Security").get_to(profile.Security.value());
}
} // namespace nearobject
