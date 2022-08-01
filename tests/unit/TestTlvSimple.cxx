#include <algorithm>
#include <catch2/catch.hpp>
#include <shared/tlv/TlvSimple.hxx>
#include <string>

TEST_CASE("TlvSimple object can be created properly from the Parse function", "[basic][infra]")
{
    SECTION("creating a TlvSimple from no data works as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<std::byte> mydata_small(mystr.size());
        for (std::size_t i = 0; i < mystr.size(); i++) {
            mydata_small[i] = std::byte{ (unsigned char)mystr[i] };
        }

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::byte length{ 0 };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), length);
        packet.insert(packet.end(), mydata_small.begin(), mydata_small.end());

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Succeeded);

        // check that it parsed correctly
        REQUIRE(tlvparsed != nullptr);
        REQUIRE(tlvparsed->Tag.size() == 1);
        REQUIRE(tlvparsed->Tag[0] == tag);
        REQUIRE(tlvparsed->Value.size() == std::size_t(length));
        for (std::size_t i = 0; i < std::size_t(length); i++)
            REQUIRE((tlvparsed->Value[i] == mydata_small[i]));
    }

    SECTION("creating a TlvSimple from a small amount of data works as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<std::byte> mydata_small(mystr.size());
        for (std::size_t i = 0; i < mystr.size(); i++) {
            mydata_small[i] = std::byte{ (unsigned char)mystr[i] };
        }

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::byte length{ uint8_t(mydata_small.size()) };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), length);
        packet.insert(packet.end(), mydata_small.begin(), mydata_small.end());

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Succeeded);

        // check that it parsed correctly
        REQUIRE(tlvparsed != nullptr);
        REQUIRE(tlvparsed->Tag.size() == 1);
        REQUIRE(tlvparsed->Tag[0] == tag);
        REQUIRE(tlvparsed->Value.size() == std::size_t(length));
        for (std::size_t i = 0; i < std::size_t(length); i++)
            REQUIRE((tlvparsed->Value[i] == mydata_small[i]));
    }

    SECTION("creating a TlvSimple from a incorrect small amount of data fails as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<std::byte> mydata_small(mystr.size());
        for (std::size_t i = 0; i < mystr.size(); i++) {
            mydata_small[i] = std::byte{ (unsigned char)mystr[i] };
        }

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::byte length{ uint8_t(mydata_small.size()+1)};
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), length);
        packet.insert(packet.end(), mydata_small.begin(), mydata_small.end());

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Failed);
    }
}
