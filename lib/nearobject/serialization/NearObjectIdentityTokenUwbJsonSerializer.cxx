
#include <nearobject/NearObjectIdentityTokenUwb.hxx>
#include <nearobject/serialization/NearObjectIdentityTokenUwbJsonSerializer.hxx>
#include <nearobject/serialization/UwbMacAddressJsonSerializer.hxx>

using namespace nearobject;

void
nearobject::to_json(nlohmann::json& json, const NearObjectIdentityTokenUwb& identityTokenUwb)
{
    json = nlohmann::json {
        { "UwbMacAddress", identityTokenUwb.GetMacAddress() },
    };
}

void
nearobject::from_json(const nlohmann::json& json, NearObjectIdentityTokenUwb& identityTokenUwb)
{
    uwb::UwbMacAddress uwbMacAddress{};
    json.at("UwbMacAddress").get_to(uwbMacAddress);
    identityTokenUwb = NearObjectIdentityTokenUwb{ uwbMacAddress };
}
