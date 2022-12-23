
#include <catch2/catch_test_macros.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbAppConfiguration.hxx>

TEST_CASE("UwbAppConfiguration performs allocation for contained value correctly", "[basic]")
{
    using namespace uwb::protocols::fira;

    SECTION("random")
    {
        DeviceRole role{ DeviceRole::Initiator };  
    }
}

