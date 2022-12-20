
#include <initializer_list>
#include <unordered_set>

#include <catch2/catch_test_macros.hpp>

#include <uwb/protocols/fira/RangingConfiguration.hxx>

using namespace uwb::protocol::fira;

TEST_CASE("RangingMethod can be used in unordered_containers", "[basic]")
{
    // Set with all possible combinations of object properties.
    const std::initializer_list<RangingMethod> RangingMethodsAll = {
        RangingMethod{ RangingDirection::OneWay, MeasurementReportMode::None },
        RangingMethod{ RangingDirection::SingleSidedTwoWay, MeasurementReportMode::None },
        RangingMethod{ RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::None },
        RangingMethod{ RangingDirection::OneWay, MeasurementReportMode::Deferred },
        RangingMethod{ RangingDirection::SingleSidedTwoWay, MeasurementReportMode::Deferred },
        RangingMethod{ RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::Deferred },
        RangingMethod{ RangingDirection::OneWay, MeasurementReportMode::NonDeferred },
        RangingMethod{ RangingDirection::SingleSidedTwoWay, MeasurementReportMode::NonDeferred },
        RangingMethod{ RangingDirection::DoubleSidedTwoWay, MeasurementReportMode::NonDeferred },
    };

    SECTION("can be used in std::unordered_set")
    {
        std::unordered_set<RangingMethod> rangingMethods{};

        // Populate set with initial objects, ensure all were inserted, implying no existing element.
        for (const auto& rangingMethod : RangingMethodsAll) {
            auto [_, inserted] = rangingMethods.insert(rangingMethod);
            REQUIRE(inserted);
        }

        // Populate set with same objects, ensure none were inserted, implying elements already exist.
        for (const auto& rangingMethod : RangingMethodsAll) {
            auto [_, inserted] = rangingMethods.insert(rangingMethod);
            REQUIRE(!inserted);
        }
    }

    SECTION("can be used as std::unordered_map key")
    {
        unsigned value = 0;
        std::unordered_map<RangingMethod, unsigned> rangingMethods{};

        // Populate map with initial objects, ensure all were inserted, implying no existing element.
        for (const auto& rangingMethod : RangingMethodsAll) {
            auto [_, inserted] = rangingMethods.insert({ rangingMethod, value++ });
            REQUIRE(inserted);
        }

        // Reset value such that insertion sequence matches the one above.
        value = 0;

        // Populate map with same objects, ensure none were inserted, implying elements already exist.
        for (const auto& rangingMethod : RangingMethodsAll) {
            auto [_, inserted] = rangingMethods.insert({ rangingMethod, value++ });
            REQUIRE(!inserted);
        }
    }
}
