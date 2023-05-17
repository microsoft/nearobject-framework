
#include <stdexcept>

#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>
#include <uwb/protocols/fira/UwbConfigurationJsonSerializer.hxx>
#include <uwb/UwbMacAddressJsonSerializer.hxx>

void
uwb::protocol::fira::to_json(nlohmann::json& json, const UwbConfiguration& uwbConfiguration)
{
    if (uwbConfiguration.GetDeviceRole().has_value()) {
        json["DeviceRole"] = uwbConfiguration.GetDeviceRole().value();
    }
    if (uwbConfiguration.GetMultiNodeMode().has_value()) {
        json["MultiNodeMode"] = uwbConfiguration.GetMultiNodeMode().value();
    }
    if (uwbConfiguration.GetControllerMacAddress().has_value()) {
        json["ControllerMacAddress"] = uwbConfiguration.GetControllerMacAddress().value();
    }
    if (uwbConfiguration.GetControleeShortMacAddress().has_value()) {
        json["ControleeShortMacAddress"] = uwbConfiguration.GetControleeShortMacAddress().value();
    }
}

void
uwb::protocol::fira::from_json(const nlohmann::json& json, UwbConfiguration& uwbConfiguration)
{
    UwbConfiguration::Builder builder{};

    uwb::protocol::fira::DeviceRole deviceRole;
    uwb::protocol::fira::MultiNodeMode multiNodeMode;
    uwb::UwbMacAddress controllerMacAddress;
    uwb::UwbMacAddress controleeShortMacAddress;
    if (json.contains("DeviceRole")) {
        json.at("DeviceRole").get_to(deviceRole);
        builder.SetDeviceRole(deviceRole);
    }
    if (json.contains("MultiNodeMode")) {
        json.at("MultiNodeMode").get_to(multiNodeMode);
        builder.SetMultiNodeMode(multiNodeMode);
    }
    if (json.contains("ControllerMacAddress")) {
        json.at("ControllerMacAddress").get_to(controllerMacAddress);
        builder.SetMacAddressController(controllerMacAddress);
    }
    if (json.contains("ControleeShortMacAddress")) {
        json.at("ControleeShortMacAddress").get_to(controleeShortMacAddress);
        builder.SetMacAddressControleeShort(controleeShortMacAddress);
    }

    uwbConfiguration = std::move(builder);
}