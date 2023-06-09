
#include <stdexcept>

#include <uwb/UwbPeer.hxx>
#include <uwb/UwbPeerJsonSerializer.hxx>

using namespace uwb;

void
uwb::to_json(nlohmann::json& json, [[maybe_unused]] const UwbPeer& uwbPeer)
{
    json = nlohmann::json {

    };
}


void
uwb::from_json([[maybe_unused]] const nlohmann::json& json, [[maybe_unused]] UwbPeer& uwbPeer)
{

}

void
uwb::to_json(nlohmann::json& json, [[maybe_unused]] const UwbPeerSpatialProperties& uwbPeerSpatialProperties)
{
    json = nlohmann::json {

    };
}


void
uwb::from_json([[maybe_unused]] const nlohmann::json& json, [[maybe_unused]] UwbPeerSpatialProperties& uwbPeerSpatialProperties)
{
}

// void
// uwb::to_json(nlohmann::json& json, const UwbMacAddress& uwbMacAddress)
// {
//     json = nlohmann::json {
//         { "Type", uwbMacAddress.GetType() },
//         { "Value", uwbMacAddress.GetValue() },
//     };
// }

// void
// uwb::from_json(const nlohmann::json& json, UwbMacAddress& uwbMacAddress)
// {
//     UwbMacAddressType uwbMacAddressType{ UwbMacAddressType::Short }; 
//     json.at("Type").get_to(uwbMacAddressType);

//     switch (uwbMacAddressType) {
//     case UwbMacAddressType::Short: {
//         UwbMacAddress::ShortType value{};
//         json.at("Value").get_to(value);
//         uwbMacAddress = UwbMacAddress{ value };
//         break;
//     }
//     case UwbMacAddressType::Extended: {
//         UwbMacAddress::ExtendedType value{};
//         json.at("Value").get_to(value);
//         uwbMacAddress = UwbMacAddress{ value };
//         break;
//     }
//     default: {
//         throw std::invalid_argument("invalid UwbMacAddressType specified");
//     }
//     }
// }

