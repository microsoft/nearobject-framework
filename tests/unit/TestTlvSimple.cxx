#include <algorithm>
#include <catch2/catch.hpp>
#include <shared/tlv/TlvSimple.hxx>
#include <string>
#include <vector>

TEST_CASE("TlvSimple object can be created properly from the Parse function", "[basic][infra]")
{
    SECTION("creating a TlvSimple from no data works as expected")
    {
        std::string mystr("");
        std::vector<std::byte> mydataSmall(mystr.size());
        for (std::size_t i = 0; i < mystr.size(); i++) {
            mydataSmall[i] = std::byte{ (unsigned char)mystr[i] };
        }

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::byte length{ 0 };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), length);
        packet.insert(std::cend(packet), std::cbegin(mydataSmall), std::cend(mydataSmall));

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
            REQUIRE((tlvparsed->Value[i] == mydataSmall[i]));
    }

    SECTION("creating a TlvSimple from a small amount of data works as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<std::byte> mydataSmall(mystr.size());
        for (std::size_t i = 0; i < mystr.size(); i++) {
            mydataSmall[i] = std::byte{ (unsigned char)mystr[i] };
        }

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::byte length{ uint8_t(mydataSmall.size()) };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), length);
        packet.insert(std::cend(packet), std::cbegin(mydataSmall), std::cend(mydataSmall));

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
            REQUIRE((tlvparsed->Value[i] == mydataSmall[i]));
    }

    SECTION("creating a TlvSimple from a incorrect small amount of data fails as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<std::byte> mydataSmall(mystr.size());
        for (std::size_t i = 0; i < mystr.size(); i++) {
            mydataSmall[i] = std::byte{ (unsigned char)mystr[i] };
        }

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::byte length{ uint8_t(mydataSmall.size() + 1) };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), length);
        packet.insert(std::cend(packet), std::cbegin(mydataSmall), std::cend(mydataSmall));

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Failed);
    }

    SECTION("creating a TlvSimple from a large amount of data succeeds as expected")
    {
        std::size_t length(0x100);
        std::vector<std::byte> mydata(length);

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), std::byte{ encoding::TlvSimple::ThreeByteLengthIndicatorValue });
        packet.insert(packet.end(), std::byte{ 0x01 });
        packet.insert(packet.end(), std::byte{ 0x00 });
        packet.insert(packet.end(), mydata.begin(), mydata.end());

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Succeeded);

        // check that it parsed correctly
        REQUIRE(tlvparsed != nullptr);
        REQUIRE(tlvparsed->Tag.size() == 1);
        REQUIRE(tlvparsed->Tag[0] == tag);
        REQUIRE(tlvparsed->Value.size() == length);
        for (std::size_t i = 0; i < length; i++)
            REQUIRE((tlvparsed->Value[i] == mydata[i]));
    }

    SECTION("creating a TlvSimple from a incorrect large amount of data fails as expected")
    {
        std::size_t length(0x100);
        std::vector<std::byte> mydata(length);

        // create the vector for the packet
        std::byte tag{ 0x3 };
        std::vector<std::byte> packet;

        packet.insert(packet.end(), tag);
        packet.insert(packet.end(), std::byte{ encoding::TlvSimple::ThreeByteLengthIndicatorValue });
        packet.insert(packet.end(), std::byte{ 0x01 });
        packet.insert(packet.end(), mydata.begin(), mydata.end());

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Failed);
    }
}
