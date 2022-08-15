
#include <nearobject/persist/NearObjectProfileJsonSerializer.hxx>
#include <nearobject/NearObjectProfile.hxx>

using namespace nearobject;

void
nearobject::to_json(nlohmann::json& json, const NearObjectConnectionProfileSecurity& security) 
{
    json = nlohmann::json{};
}

void
nearobject::from_json(const nlohmann::json& json, NearObjectConnectionProfileSecurity& security) 
{
}

void
nearobject::to_json(nlohmann::json& json, const NearObjectProfile& profile) 
{
    json = nlohmann::json {
        { "Scope", profile.Scope },
        { "Security", profile.Security.value_or(NearObjectConnectionProfileSecurity{}) },
    };
}

void
nearobject::from_json(const nlohmann::json& json, NearObjectProfile& profile) 
{
    profile.Security.emplace();
    json.at("Scope").get_to(profile.Scope);
    json.at("Security").get_to(profile.Security.value());
}
