
#include <algorithm>
#include <chrono>
#include <memory>

#include <catch2/catch.hpp>
#include <nearobject/service/NearObjectDevice.hxx>
#include <nearobject/service/NearObjectDeviceDiscoveryAgent.hxx>

namespace nearobject
{
namespace service
{
namespace test
{
struct NearObjectDeviceDiscoveryAgentTest :
    public NearObjectDeviceDiscoveryAgent {
    virtual ~NearObjectDeviceDiscoveryAgentTest() = default;

    std::promise<std::vector<std::weak_ptr<NearObjectDevice>>> ProbePromise;

protected:
    void
    StartImpl() override
    { }

    void
    StopImpl() override
    { }

    std::future<std::vector<std::weak_ptr<NearObjectDevice>>>
    ProbeAsyncImpl() override
    {
        return ProbePromise.get_future();
    }
};

struct NearObjectDeviceTest :
    public NearObjectDevice {

    explicit NearObjectDeviceTest(uint64_t deviceId) :
        NearObjectDevice(deviceId)
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

namespace std
{
template <>
struct equal_to<std::weak_ptr<nearobject::service::test::NearObjectDeviceTest>>
{
    bool operator()(const std::weak_ptr<nearobject::service::test::NearObjectDeviceTest>& lhs, const std::weak_ptr<nearobject::service::test::NearObjectDeviceTest>& rhs) const noexcept
    {
        return *(lhs.lock()) == *(rhs.lock());
    }
};
} // namespace std

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

    SECTION("discovery probe promise is eventually completed")
    {
        static constexpr auto probeTimeout = 1ms;

        std::vector<std::shared_ptr<NearObjectDevice>> probeDevices{
            std::make_shared<test::NearObjectDeviceTest>(0x1),
            std::make_shared<test::NearObjectDeviceTest>(0x2),
            std::make_shared<test::NearObjectDeviceTest>(0x3),
        };

        // Kick of an async probe.
        auto probeFuture = discoveryAgentTest.ProbeAsync();
        REQUIRE(probeFuture.valid());

        // [test] Set the value of the promise, which will update the future shared state.
        discoveryAgentTest.ProbePromise.set_value({
            probeDevices[0], probeDevices[1], probeDevices[2],    
        });

        // Wait for the future to complete and get the result.
        const auto probeCompletion = probeFuture.wait_for(probeTimeout);
        REQUIRE(probeCompletion == std::future_status::ready);
        const auto probeResults = probeFuture.get();
        REQUIRE(!probeFuture.valid());

        // Verify the future provided the expected results.
        std::vector<std::shared_ptr<NearObjectDevice>> probeResultsStrong{};
        std::transform(std::cbegin(probeResults), std::cend(probeResults), std::back_inserter(probeResultsStrong), [](auto& probeDevice)
        {
            return probeDevice.lock();
        });

        REQUIRE(probeResults.size() == probeDevices.size());
        REQUIRE(probeResultsStrong == probeDevices);
    }
}
