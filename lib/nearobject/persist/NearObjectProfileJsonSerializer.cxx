
#include <nlohmann/json.hpp>
#include <nearobject/persist/NearObjectProfileJsonSerializer.hxx>

using namespace nearobject;
using namespace nearobject::persistence;

std::string
NearObjectProfilePersister::ToJson(const NearObjectProfile& profile) const
{
    return {};
}

NearObjectProfile
NearObjectProfilePersister::FromJson(const std::string& json) const
{
    return {};
}

using json = nlohmann::json;

namespace nearobject
{
void to_json(json& j, const NearObjectConnectionProfileSecurity& security) 
{
    j = json{};
}

void from_json(const json& j, NearObjectConnectionProfileSecurity& security) 
{
}

void to_json(json& j, const NearObjectProfile& profile) 
{
    j = json {
        { "Scope", profile.Scope },
        { "Security", profile.Security.value_or(NearObjectConnectionProfileSecurity{}) },
    };
}

void from_json(const json& j, NearObjectProfile& profile) 
{
    profile.Security.emplace();
    j.at("Scope").get_to(profile.Scope);
    j.at("Security").get_to(profile.Security.value());
}
} // namespace nearobject
