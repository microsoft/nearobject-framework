
#include <stdexcept>

#include <nearobject/serialization/UwbConfigurationJsonSerializer.hxx>
#include <nearobject/serialization/UwbMacAddressJsonSerializer.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

void
uwb::protocol::fira::to_json(nlohmann::json& json, const UwbConfiguration& uwbConfiguration)
{
    json = nlohmann::json{
        { "DeviceRole", uwbConfiguration.GetDeviceRole().value() },
        { "MultiNodeMode", uwbConfiguration.GetMultiNodeMode().value() },
        { "ControllerMacAddress", uwbConfiguration.GetControllerMacAddress().value() },
        { "ControleeShortMacAddress", uwbConfiguration.GetControleeShortMacAddress().value() },
    };
}

void
uwb::protocol::fira::from_json(const nlohmann::json& json, UwbConfiguration& uwbConfiguration)
{
    UwbConfiguration::Builder builder{};

    uwb::protocol::fira::DeviceRole deviceRole;
    uwb::protocol::fira::MultiNodeMode multiNodeMode;
    uwb::UwbMacAddress controllerMacAddress;
    uwb::UwbMacAddress controleeShortMacAddress;
    json.at("DeviceRole").get_to(deviceRole);
    json.at("MultiNodeMode").get_to(multiNodeMode);
    json.at("ControllerMacAddress").get_to(controllerMacAddress);
    json.at("ControleeShortMacAddress").get_to(controleeShortMacAddress);

    builder.SetDeviceRole(deviceRole);
    builder.SetMultiNodeMode(multiNodeMode);
    builder.SetMacAddressController(controllerMacAddress);
    builder.SetMacAddressControleeShort(controleeShortMacAddress);

    uwbConfiguration = std::move(builder);
}