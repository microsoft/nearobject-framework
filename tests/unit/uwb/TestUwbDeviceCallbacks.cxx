
#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <uwb/UwbDeviceEventCallbacks.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>

TEST_CASE("uwb device callbacks can be created", "[basic]")
{
    using namespace uwb;

    UwbDeviceEventCallbacks cb{
        .OnStatusChanged = [](auto&& status) {},
        .OnDeviceStatusChanged = [](auto&& status) {},
        .OnSessionStatusChanged = [](auto&& status) {},
        .OnSessionMulticastListStatus = [](auto&& status) {},
        .OnSessionRangingData = [](auto&& rangingData) {}
    };
    auto callbacks = std::make_unique<UwbDeviceEventCallbacks>(
        [](auto&& status) {},
        [](auto&& deviceStatus) {},
        [](auto&& sessionStatus) {},
        [](auto&& sessionMulticastListStatus) {},
        [](auto&& rangingData) {}
    );
}
