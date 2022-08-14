
#include <nearobject/persist/NearObjectProfileJsonSerializer.hxx>

using namespace nearobject;
using namespace nearobject::persistence;
using json = nlohmann::json;

namespace nearobject
{
// Implementations of required functions for use with nlohmann json conversion.
void
to_json(json& asJson, const NearObjectConnectionProfileSecurity& security) 
{
    asJson = json{};
}

void
from_json(const json& asJson, NearObjectConnectionProfileSecurity& security) 
{
}

void
to_json(json& asJson, const NearObjectProfile& profile) 
{
    asJson = json {
        { "Scope", profile.Scope },
        { "Security", profile.Security.value_or(NearObjectConnectionProfileSecurity{}) },
    };
}

void
from_json(const json& asJson, NearObjectProfile& profile) 
{
    profile.Security.emplace();
    asJson.at("Scope").get_to(profile.Scope);
    asJson.at("Security").get_to(profile.Security.value());
}
} // namespace nearobject
