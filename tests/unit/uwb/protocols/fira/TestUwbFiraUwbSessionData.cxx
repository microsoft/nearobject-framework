
#include <catch2/catch_test_macros.hpp>

#include <notstd/utility.hxx>

#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include <notstd/unique_ptr_out.hxx>

#include <tlv/TlvBer.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

TEST_CASE("UwbSessionData encoding")
{
    SECTION("UwbSessionData convert to TlvBer and back again works")
    {
        using namespace uwb::protocol::fira;
        using namespace encoding;

        uwb::protocol::fira::UwbSessionData sessionData;

        sessionData.uwbConfiguration = UwbConfiguration::Builder()
                                           .SetMacAddressController(uwb::UwbMacAddress::FromString("67:89", uwb::UwbMacAddressType::Short).value())
                                           .SetMacAddressControleeShort(uwb::UwbMacAddress::FromString("12:34", uwb::UwbMacAddressType::Short).value())
                                           .SetMultiNodeMode(MultiNodeMode::Unicast)
                                           .SetDeviceRole(DeviceRole::Initiator);

        sessionData.sessionDataVersion = 1;
        sessionData.sessionId = 1234;

        auto tlvber = sessionData.ToDataObject();
        REQUIRE(tlvber);
        auto data = tlvber->ToBytes();

        std::unique_ptr<TlvBer> tlvParsed;
        auto parseResult = TlvBer::Parse(notstd::unique_ptr_out(tlvParsed), data);
        REQUIRE(parseResult == TlvBer::ParseResult::Succeeded);
        REQUIRE(tlvParsed);
        REQUIRE(*tlvParsed == *tlvber);

        uwb::protocol::fira::UwbSessionData sessionDataParsed;
        REQUIRE_NOTHROW(sessionDataParsed = UwbSessionData::FromDataObject(*tlvParsed));
        std::hash<uwb::protocol::fira::UwbSessionData> USBhash;
        REQUIRE(USBhash(sessionDataParsed) == USBhash(sessionData));
    }
}