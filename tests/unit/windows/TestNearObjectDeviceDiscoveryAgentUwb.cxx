
#include <algorithm>
#include <chrono>
#include <memory>

#include <catch2/catch.hpp>

#include "NearObjectDeviceDiscoveryAgentUwb.hxx"

namespace windows
{
namespace nearobject
{
namespace service
{
namespace test
{
struct NearObjectDeviceDiscoveryAgentUwbTest :
    public NearObjectDeviceDiscoveryAgentUwb
{
    ~NearObjectDeviceDiscoveryAgentUwbTest() final = default;
};
} // namespace test
} // namespace service
} // namespace nearobject
} // namespace windows

TEST_CASE("near object windows uwb device discovery agent can be created", "[basic][service][windows]")
{
    using namespace windows::nearobject::service;

    test::NearObjectDeviceDiscoveryAgentUwbTest discoveryAgentTest{};

    SECTION("creating an agent doesn't cause a crash when created")
    {
    }
}
