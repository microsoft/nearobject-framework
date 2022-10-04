
#include <algorithm>
#include <chrono>
#include <memory>

#include <catch2/catch.hpp>
#include <nearobject/service/NearObjectDeviceController.hxx>
#include <nearobject/service/NearObjectDeviceControllerDiscoveryAgent.hxx>

// NOLINTBEGIN(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)

namespace nearobject
{
namespace service
{
namespace test
{
struct NearObjectDeviceDiscoveryAgentTest final :
    public NearObjectDeviceControllerDiscoveryAgent
{
    ~NearObjectDeviceDiscoveryAgentTest() final = default;

    std::promise<std::vector<std::shared_ptr<NearObjectDeviceController>>> ProbePromise;

    void
    SignalDiscoveryEvent(NearObjectDevicePresence presence, std::shared_ptr<NearObjectDeviceController> deviceChanged)
    {
        DevicePresenceChanged(presence, std::move(deviceChanged));
    }

protected:
    void
    StartImpl() override
    {}

    void
    StopImpl() override
    {}

    std::future<std::vector<std::shared_ptr<NearObjectDeviceController>>>
    ProbeAsyncImpl() override
    {
        return ProbePromise.get_future();
    }
};

struct NearObjectDeviceTest :
    public NearObjectDeviceController
{
    explicit NearObjectDeviceTest(uint64_t deviceId) :
        NearObjectDeviceController(deviceId)
    {}

    StartSessionResult
    StartSessionImpl(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) override
    {
        return { std::nullopt };
    }
};
} // namespace test
} // namespace service
} // namespace nearobject

TEST_CASE("near object device discovery agent can be created", "[basic][service]")
{
    using namespace std::chrono_literals;
    using namespace nearobject::service;

    test::NearObjectDeviceDiscoveryAgentTest discoveryAgentTest{};

    SECTION("creating an agent doesn't cause a crash when created")
    {
    }

    SECTION("running status is stopped upon creation")
    {
        REQUIRE(!discoveryAgentTest.IsStarted());
    }

    SECTION("running status is correctly reflected")
    {
        discoveryAgentTest.Start();
        REQUIRE(discoveryAgentTest.IsStarted());
        discoveryAgentTest.Stop();
        REQUIRE(!discoveryAgentTest.IsStarted());
    }

    SECTION("running status doesn't change when target state already met")
    {
        discoveryAgentTest.Stop();
        REQUIRE(!discoveryAgentTest.IsStarted());
        discoveryAgentTest.Stop();
        REQUIRE(!discoveryAgentTest.IsStarted());
        discoveryAgentTest.Start();
        REQUIRE(discoveryAgentTest.IsStarted());
        discoveryAgentTest.Start();
        REQUIRE(discoveryAgentTest.IsStarted());
    }

    SECTION("discovery event callback can be registered without causing a crash")
    {
        discoveryAgentTest.RegisterDiscoveryEventCallback([](auto&&, auto&&) {});
    }

    SECTION("discovery event callback can be registered in started state")
    {
        discoveryAgentTest.Start();
        discoveryAgentTest.RegisterDiscoveryEventCallback([](auto&&, auto&&) {});
    }

    SECTION("discovery event callback can be registered in stopped state")
    {
        discoveryAgentTest.Stop();
        discoveryAgentTest.RegisterDiscoveryEventCallback([](auto&&, auto&&) {});
    }

    SECTION("discovery event callback provides correct presence and device for added device")
    {
        const auto deviceToAdd = std::make_shared<test::NearObjectDeviceTest>(0x1);
        discoveryAgentTest.RegisterDiscoveryEventCallback([&](auto&& presence, auto&& deviceAdded) {
            REQUIRE(*deviceToAdd == *deviceAdded);
            REQUIRE(presence == NearObjectDevicePresence::Arrived);
        });
        discoveryAgentTest.SignalDiscoveryEvent(NearObjectDevicePresence::Arrived, deviceToAdd);
    }

    SECTION("discovery event callback provides correct presence and device for removed device")
    {
        const auto deviceToRemove = std::make_shared<test::NearObjectDeviceTest>(0x1);
        discoveryAgentTest.RegisterDiscoveryEventCallback([&](auto&& presence, auto&& deviceRemoved) {
            REQUIRE(*deviceToRemove == *deviceRemoved);
            REQUIRE(presence == NearObjectDevicePresence::Departed);
        });
        discoveryAgentTest.SignalDiscoveryEvent(NearObjectDevicePresence::Departed, deviceToRemove);
    }

    SECTION("discovery probe promise is eventually completed")
    {
        static constexpr auto probeTimeout = 1ms;

        const std::vector<std::shared_ptr<NearObjectDeviceController>> probeDevices{
            std::make_shared<test::NearObjectDeviceTest>(0x1),
            std::make_shared<test::NearObjectDeviceTest>(0x2),
            std::make_shared<test::NearObjectDeviceTest>(0x3),
        };

        // Kick of an async probe.
        auto probeFuture = discoveryAgentTest.ProbeAsync();
        REQUIRE(probeFuture.valid());

        // [test] Set the value of the promise, which will update the future shared state.
        discoveryAgentTest.ProbePromise.set_value({
            probeDevices[0],
            probeDevices[1],
            probeDevices[2],
        });

        // Wait for the future to complete and get the result.
        const auto probeCompletion = probeFuture.wait_for(probeTimeout);
        REQUIRE(probeCompletion == std::future_status::ready);
        const auto probeResults = probeFuture.get();
        REQUIRE(!probeFuture.valid());
        REQUIRE(probeResults.size() == probeDevices.size());
    }
}

// NOLINTEND(cppcoreguidelines-special-member-functions, hicpp-special-member-functions)
