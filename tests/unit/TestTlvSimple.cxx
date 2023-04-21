
#include <algorithm>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <tlv/TlvSimple.hxx>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

/**
* @brief Convert a string to a byte vector
*
* @param str 
* @return std::vector<uint8_t>
*/
std::vector<uint8_t>
StringToByteVector(const std::string &str) noexcept
{
    std::vector<uint8_t> vec(str.size());
    std::transform(std::cbegin(str), std::cend(str), std::begin(vec), [](const auto &element) {
        return uint8_t(element);
    });

    return vec;
}

TEST_CASE("TlvSimple object can be created properly from the Parse function", "[basic][infra]")
{
    SECTION("creating a TlvSimple from no data works as expected")
    {
        std::string mystr("");
        std::vector<uint8_t> mydataSmall = StringToByteVector(mystr);

        // create the vector for the packet
        uint8_t tag{ 0x3 };
        uint8_t length{ 0 };
        std::vector<uint8_t> packet;

        packet.insert(std::cend(packet), tag);
        packet.insert(std::cend(packet), length);
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
        REQUIRE(std::equal(std::cbegin(tlvparsed->Value), std::cend(tlvparsed->Value), std::cbegin(mydataSmall)));
    }

    SECTION("creating a TlvSimple from a small amount of data works as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<uint8_t> mydataSmall = StringToByteVector(mystr);

        // create the vector for the packet
        uint8_t tag{ 0x3 };
        uint8_t length{ uint8_t(mydataSmall.size()) };
        std::vector<uint8_t> packet;

        packet.insert(std::cend(packet), tag);
        packet.insert(std::cend(packet), length);
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
        REQUIRE(std::equal(std::cbegin(tlvparsed->Value), std::cend(tlvparsed->Value), std::cbegin(mydataSmall)));
    }

    SECTION("creating a TlvSimple from a incorrect small amount of data fails as expected")
    {
        std::string mystr("hellothisisbob");
        std::vector<uint8_t> mydataSmall = StringToByteVector(mystr);

        // create the vector for the packet
        uint8_t tag{ 0x3 };
        uint8_t length{ uint8_t(mydataSmall.size() + 1) };
        std::vector<uint8_t> packet;

        packet.insert(std::cend(packet), tag);
        packet.insert(std::cend(packet), length);
        packet.insert(std::cend(packet), std::cbegin(mydataSmall), std::cend(mydataSmall));

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Failed);
    }

    SECTION("creating a TlvSimple from a large amount of data succeeds as expected")
    {
        std::size_t length(0x100);
        std::vector<uint8_t> mydata(length);

        // create the vector for the packet
        uint8_t tag{ 0x3 };
        std::vector<uint8_t> packet;

        packet.insert(std::cend(packet), tag);
        packet.insert(std::cend(packet), uint8_t{ encoding::TlvSimple::ThreeByteLengthIndicatorValue });
        packet.insert(std::cend(packet), uint8_t{ 0x01 });
        packet.insert(std::cend(packet), uint8_t{ 0x00 });
        packet.insert(std::cend(packet), std::cbegin(mydata), std::cend(mydata));

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Succeeded);

        // check that it parsed correctly
        REQUIRE(tlvparsed != nullptr);
        REQUIRE(tlvparsed->Tag.size() == 1);
        REQUIRE(tlvparsed->Tag[0] == tag);
        REQUIRE(tlvparsed->Value.size() == length);
        REQUIRE(std::equal(std::cbegin(tlvparsed->Value), std::cend(tlvparsed->Value), std::cbegin(mydata)));
    }

    SECTION("creating a TlvSimple from a incorrect large amount of data fails as expected")
    {
        std::size_t length(0x100);
        std::vector<uint8_t> mydata(length);

        // create the vector for the packet
        uint8_t tag{ 0x3 };
        std::vector<uint8_t> packet;

        packet.insert(std::cend(packet), tag);
        packet.insert(std::cend(packet), uint8_t{ encoding::TlvSimple::ThreeByteLengthIndicatorValue });
        packet.insert(std::cend(packet), uint8_t{ 0x01 });
        packet.insert(std::cend(packet), std::cbegin(mydata), std::cend(mydata));

        // try to parse
        encoding::TlvSimple *tlvparsed = nullptr;
        auto result = encoding::TlvSimple::Parse(&tlvparsed, packet);
        REQUIRE(result == encoding::Tlv::ParseResult::Failed);
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
