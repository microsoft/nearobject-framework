
#include <stdexcept>

#include <nearobject/serialization/UwbSessionDataJsonSerializer.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

void
uwb::protocol::fira::to_json(nlohmann::json& json, const UwbSessionData& uwbSessionData)
{
    json = nlohmann::json{
        { "SessionDataVersion", uwbSessionData.sessionDataVersion },
        { "SessionId", uwbSessionData.sessionId },
        { "SubSessionId", uwbSessionData.subSessionId },
        // TODO: below types need their own [to|from]_json() impls
        // { "UwbConfiguration", uwbSessionData.uwbConfiguration },
        // { "StaticRangingInfo", uwbSessionData.staticRangingInfo },
        // { "SecureRangingInfo", uwbSessionData.secureRangingInfo },
    };
}

void
uwb::protocol::fira::from_json(const nlohmann::json& json, UwbSessionData& uwbSessionData)
{
    json.at("SessionDataVersion").get_to(uwbSessionData.sessionDataVersion);
    json.at("SessionId").get_to(uwbSessionData.sessionId);
    json.at("SubSessionId").get_to(uwbSessionData.subSessionId);
    // TODO: below types need their own [to|from]_json() impls
    // json.at("UwbConfiguration").get_to(uwbSessionData.uwbConfiguration);
    // json.at("StaticRangingInfo").get_to(uwbSessionData.staticRangingInfo);
    // json.at("SecureRangingInfo").get_to(uwbSessionData.secureRangingInfo);
}
