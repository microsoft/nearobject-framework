
#include <stdexcept>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbMacAddressJsonSerializer.hxx>
#include <uwb/UwbPeer.hxx>
#include <uwb/UwbPeerJsonSerializer.hxx>

using namespace uwb;

void
uwb::to_json(nlohmann::json& json, const UwbPeer& uwbPeer)
{
    json = nlohmann::json{
        { "MacAddress", uwbPeer.GetAddress() },
        { "SpatialProperties", uwbPeer.GetSpatialProperties() },
    };
}

void
uwb::from_json(const nlohmann::json& json, UwbPeer& uwbPeer)
{
    UwbMacAddress uwbMacAddress{};
    UwbPeerSpatialProperties uwbPeerSpatialProperties{};
    json.at("MacAddress").get_to(uwbMacAddress);
    json.at("SpatialProperties").get_to(uwbPeerSpatialProperties);

    uwbPeer = UwbPeer{
        std::move(uwbMacAddress),
        std::move(uwbPeerSpatialProperties)
    };
}
