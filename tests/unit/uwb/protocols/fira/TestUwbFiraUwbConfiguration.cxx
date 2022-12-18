
#include <catch2/catch_test_macros.hpp>

#include <initializer_list>
#include <unordered_set>
#include <unordered_map>

#include <uwb/protocols/fira/UwbConfiguration.hxx>

TEST_CASE("uwb configuration objects can be created", "[basic]")
{
    using namespace uwb::protocol::fira;

    SECTION("creation doesn't cause a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<UwbConfiguration>());
    }

    SECTION("destruction doesn't cause a crash")
    {
        auto uwbConfiguration = std::make_unique<UwbConfiguration>();
        REQUIRE_NOTHROW([&]{ 
            uwbConfiguration.reset();
        });
    }
}

TEST_CASE("UwbConfiguration can be used in unordered_containers", "[basic][container]")
{
    using namespace uwb::protocol::fira;

    UwbConfiguration uwbConfigurationDeviceRoleInitiator{};
    uwbConfigurationDeviceRoleInitiator._deviceRole = DeviceRole::Initiator;
    UwbConfiguration uwbConfigurationDeviceRoleResponder{};
    uwbConfigurationDeviceRoleResponder._deviceRole = DeviceRole::Responder;
    UwbConfiguration uwbConfigurationHoppingMode{};
    uwbConfigurationHoppingMode._hoppingMode = true;

    const std::initializer_list<UwbConfiguration> UwbConfigurations = {
        uwbConfigurationDeviceRoleInitiator,
        uwbConfigurationDeviceRoleResponder,
        uwbConfigurationHoppingMode
    };

    SECTION("can be used in std::unordered_set")
    {
        std::unordered_set<UwbConfiguration> uwbConfigurations{};

        const auto insertConfiguration = [&](const UwbConfiguration& uwbConfiguration) -> bool {
            auto [_, inserted] = uwbConfigurations.insert(uwbConfiguration);
            return inserted;
        };

        for (const auto& uwbConfiguration : UwbConfigurations) {
            REQUIRE(insertConfiguration(uwbConfiguration));
            REQUIRE(!insertConfiguration(uwbConfiguration));
        }
    }

    SECTION("can be used as std::unordered_map key")
    {
        unsigned value = 0;
        std::unordered_map<UwbConfiguration, unsigned> uwbConfigurations{};

        // Populate map with initial objects, ensure all were inserted, implying no existing element.
        for (const auto& uwbConfiguration : UwbConfigurations) {
            auto [_, inserted] = uwbConfigurations.insert({ uwbConfiguration, value++ });
            REQUIRE(inserted);
        }

        // Reset value such that insertion sequence matches the one above.
        value = 0;

        // Populate map with same objects, ensure none were inserted, implying elements already exist.
        for (const auto& uwbConfiguration : UwbConfigurations) {
            auto [_, inserted] = uwbConfigurations.insert({ uwbConfiguration, value++ });
            REQUIRE(!inserted);
        }
    }
}
