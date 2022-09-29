
#include <stdexcept>

#include <nearobject/serialization/UwbMacAddressJsonSerializer.hxx>
#include <uwb/UwbMacAddress.hxx>

using namespace uwb;

namespace uwb 
{
/**
 * @brief Specialize enum serialization to avoid any potential issues with
 * changes or re-ordering of the underlying values.
 */
NLOHMANN_JSON_SERIALIZE_ENUM(UwbMacAddressType, {
    { UwbMacAddressType::Short, "Short" },  
    { UwbMacAddressType::Extended, "Extended" },
})
} // namespace uwb

void
uwb::to_json(nlohmann::json& json, const UwbMacAddress& uwbMacAddress)
{
    json = nlohmann::json {
        { "Type", uwbMacAddress.GetType() },
        { "Value", uwbMacAddress.GetValue() },
    };
}

void
uwb::from_json(const nlohmann::json& json, UwbMacAddress& uwbMacAddress)
{
    UwbMacAddressType uwbMacAddressType{ UwbMacAddressType::Short }; 
    json.at("Type").get_to(uwbMacAddressType);

    switch (uwbMacAddressType) {
    case UwbMacAddressType::Short: {
        UwbMacAddress::ShortType value{};
        json.at("Value").get_to(value);
        uwbMacAddress = UwbMacAddress{ value };
        break;
    }
    case UwbMacAddressType::Extended: {
        UwbMacAddress::ExtendedType value{};
        json.at("Value").get_to(value);
        uwbMacAddress = UwbMacAddress{ value };
        break;
    }
    default: {
        throw std::invalid_argument("invalid UwbMacAddressType specified");
    }
    }
}
