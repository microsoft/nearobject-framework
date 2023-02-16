
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
    OnStatusChanged(::uwb::protocol::fira::UwbStatus /* status */) override
    {
    }

    void 
    OnDeviceStatusChanged(::uwb::protocol::fira::UwbStatusDevice /* statusDevice */) override
    {
    }

    void
    OnSessionStatusChanged(::uwb::protocol::fira::UwbSessionStatus /* statusSession */) override
    {
    }

    void
    OnSessionMulticastListStatus(::uwb::protocol::fira::UwbSessionUpdateMulicastListStatus /* statusMulticastList */) override
    {
    }

    void
    OnSessionRangingData(::uwb::protocol::fira::UwbRangingData /* rangingData */) override
    {
    }
};
} // namespace uwb::test

TEST_CASE("uwb device callbacks can be created", "[basic]")
{
    using namespace uwb;

    auto callbacks = std::make_unique<test::UwbDeviceCallbacksTest>();
}
