
#include <memory>

#include <catch2/catch.hpp>
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
} // namespace test
} // namespace service
} // namespace nearobject

TEST_CASE("near object device discovery agent can be created", "[basic][service]")
{
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
}
