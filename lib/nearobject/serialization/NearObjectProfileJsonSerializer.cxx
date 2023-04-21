
#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/serialization/NearObjectProfileJsonSerializer.hxx>

#include <jsonify/nlohmann/std_optional_serializer.hxx>

using namespace nearobject;

void
nearobject::to_json(nlohmann::json& json, const NearObjectProfileSecurity& /* security */)
{
    json = nlohmann::json({});
}

void
nearobject::from_json(const nlohmann::json& /* json */, NearObjectProfileSecurity& /* security */)
{
}

void
nearobject::to_json(nlohmann::json& json, const NearObjectProfile& profile)
{
    json = nlohmann::json{
        { "Scope", profile.GetScope() },
        { "Security", profile.GetSecurity() },
    };
}

void
nearobject::from_json(const nlohmann::json& json, NearObjectProfile& profile)
{
    json.at("Scope").get_to(profile.m_scope);
    json.at("Security").get_to(profile.m_security);
}
