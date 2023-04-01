
#include <memory>

#include <catch2/catch_test_macros.hpp>

#include <uwb/UwbDeviceEventCallbacks.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb::test
{
struct UwbDeviceCallbacksTest : public UwbDeviceEventCallbacks
{
    ~UwbDeviceCallbacksTest() = default;

    void
    OnStatusChanged([[maybe_unused]] ::uwb::protocol::fira::UwbStatus status) override
    {
    }

    void
    OnDeviceStatusChanged([[maybe_unused]] ::uwb::protocol::fira::UwbStatusDevice statusDevice) override
    {
    }

    void
    OnSessionStatusChanged([[maybe_unused]] ::uwb::protocol::fira::UwbSessionStatus statusSession) override
    {
    }
};
} // namespace uwb::test

TEST_CASE("uwb device callbacks can be created", "[basic]")
{
    using namespace uwb;

    auto callbacks = std::make_unique<test::UwbDeviceCallbacksTest>();
}
