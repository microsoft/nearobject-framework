
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdint>
#include <iterator>
#include <random>
#include <vector>

#include <notstd/unique_ptr_out.hxx>
#include <catch2/catch_test_macros.hpp>

#include <tlv/TlvBer.hxx>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

#include <uwb/protocols/fira/UwbSessionData.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>
#include <uwb/protocols/fira/UwbConfigurationBuilder.hxx>

using namespace encoding;

namespace encoding::test 
{
std::vector<uint8_t>
getOctets(size_t length)
{
    std::independent_bits_engine<std::mt19937, CHAR_BIT * sizeof(uint8_t), uint16_t> engine(
        static_cast<uint16_t>(std::chrono::system_clock::now().time_since_epoch().count()));

    std::vector<uint8_t> holder(length);
    std::generate(std::begin(holder), std::end(holder), [&engine] {
        return static_cast<uint8_t>(engine());
    });

    return holder;
}
} // namespace encoding::test 

TEST_CASE("test TlvBer", "[basic][infra]")
{
    static constexpr std::array<uint8_t, 2> tagTwoBytesPrimitive{ 0b11011111, 0x24 };
    static constexpr std::array<uint8_t, 2> tagTwoBytesConstructed{ 0xFF, 0x24 };
    static constexpr std::array<uint8_t, 3> tagThreeBytesPrimitive{ 0b11011111, 0x94, 0x17 };
    static constexpr std::array<uint8_t, 2> valueTwoBytes{ 0x91, 0x92 };
    static constexpr std::array<uint8_t, 3> valueThreeBytes{ 0x91, 0x92, 0x93 };
    static constexpr std::array<uint8_t, 5> valueFiveBytes{ 0x91, 0x92, 0x93, 0x94, 0x95 };

    static constexpr std::size_t minSizeForTwoLengthOctets = 128;
    static constexpr std::size_t minSizeForThreeLengthOctets = 256;
    static constexpr std::size_t minSizeForFourLengthOctets = 65'536;
    static constexpr std::size_t minSizeForFiveLengthOctets = 16'777'216;

    constexpr std::array<std::tuple<size_t, std::size_t>, 4> minSizesForLengthOctets = {
        std::make_tuple(2, minSizeForTwoLengthOctets),
        std::make_tuple(3, minSizeForThreeLengthOctets),
        std::make_tuple(4, minSizeForFourLengthOctets),
        std::make_tuple(5, minSizeForFiveLengthOctets)
    };

    SECTION("ParseTag fails if the tag is more than 3 bytes")
    {
        std::array<uint8_t, 4> invalidTag{ 0xFF, 0x84, 0x85, 0x16 };
        TlvBer::Class tlvClass = TlvBer::Class::Invalid;
        TlvBer::Type tlvType = TlvBer::Type::Primitive;
        uint32_t tagNumber = 0;
        std::vector<uint8_t> tagComplete;
        std::size_t bytesParsed = 0;
        REQUIRE(Tlv::ParseResult::Failed == TlvBer::ParseTag(tlvClass, tlvType, tagNumber, tagComplete, invalidTag, bytesParsed));
    }

    SECTION("ParseTag fails if the tag is 2 bytes long and the second byte is not valued from 0x1F to 0x7F")
    {
        for (uint8_t invalidSecondByte = 0x0; invalidSecondByte < 0x1F; invalidSecondByte++) {
            std::array<uint8_t, 2> invalidTag{ 0xFF, invalidSecondByte };
            TlvBer::Class tlvClass = TlvBer::Class::Invalid;
            TlvBer::TlvBer::Type tlvType = TlvBer::Type::Primitive;
            uint32_t tagNumber = 0;
            std::vector<uint8_t> tagComplete;
            std::size_t bytesParsed = 0;
            REQUIRE(Tlv::ParseResult::Failed == TlvBer::ParseTag(tlvClass, tlvType, tagNumber, tagComplete, invalidTag, bytesParsed));
        }

        for (uint8_t invalidSecondByte = 0x80; invalidSecondByte != 0; invalidSecondByte++) {
            std::array<uint8_t, 2> invalidTag{ 0xFF, invalidSecondByte };
            TlvBer::Class tlvClass = TlvBer::Class::Invalid;
            TlvBer::TlvBer::Type tlvType = TlvBer::Type::Primitive;
            uint32_t tagNumber = 0;
            std::vector<uint8_t> tagComplete;
            std::size_t bytesParsed = 0;
            REQUIRE(Tlv::ParseResult::Failed == TlvBer::ParseTag(tlvClass, tlvType, tagNumber, tagComplete, invalidTag, bytesParsed));
        }
    }

    SECTION("ParseLength fails if the indicated length is longer than 5 bytes")
    {
        for (uint8_t invalidNumberOfOctets = 0x85; invalidNumberOfOctets != 0; invalidNumberOfOctets++) {
            std::array<uint8_t, 1> invalidLengthEncoding{ invalidNumberOfOctets };
            std::size_t bytesParsed = 0;
            std::size_t length = 0;
            REQUIRE(Tlv::ParseResult::Failed == TlvBer::ParseLength(length, invalidLengthEncoding, bytesParsed));
        }
    }

    SECTION("ParsePrimitiveValue fails if the provided span doesn't have enough bytes")
    {
        std::size_t bytesParsed = 0;
        std::vector<uint8_t> valueOutput;
        REQUIRE(TlvBer::ParsePrimitiveValue(valueOutput, 4, valueThreeBytes, bytesParsed) == Tlv::ParseResult::Failed);
    }

    SECTION("creating a TlvBer from an empty Builder holds no data")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .Build();
        REQUIRE(tlvBer.Tag.empty());
        REQUIRE(tlvBer.Value.empty());
    }

    SECTION("creating a TlvBer with no value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(uint8_t(0x93))
                          .Build();
        REQUIRE(tlvBer.Tag.size() == 1);
        REQUIRE(tlvBer.Tag[0] == 0x93);
        REQUIRE(tlvBer.Value.empty());
    }

    SECTION("creating a TlvBer with a primitive value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(uint8_t(0x93))
                          .SetValue(0x94)
                          .Build();
        REQUIRE(tlvBer.Tag.size() == 1);
        REQUIRE(tlvBer.Tag[0] == 0x93);
        REQUIRE(tlvBer.Value.size() == 1);
        REQUIRE(tlvBer.Value[0] == 0x94);
    }

    SECTION("creating a TlvBer with a long tag works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagTwoBytesPrimitive)
                          .SetValue(0x43)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag), std::cend(tlvBer.Tag), std::cbegin(tagTwoBytesPrimitive)));

        REQUIRE(tlvBer.Value.size() == 1);
        REQUIRE(tlvBer.Value[0] == 0x43);
    }

    SECTION("creating a TlvBer with a long tag and value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagTwoBytesPrimitive)
                          .SetValue(valueTwoBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag), std::cend(tlvBer.Tag), std::cbegin(tagTwoBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value), std::cend(tlvBer.Value), std::cbegin(valueTwoBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0b11011111, 0x24, 0x02, 0x91, 0x92 };
        REQUIRE(std::equal(std::cbegin(bytes), std::cend(bytes), std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a 3 byte tag and 2 byte value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueTwoBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag), std::cend(tlvBer.Tag), std::cbegin(tagThreeBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value), std::cend(tlvBer.Value), std::cbegin(valueTwoBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0b11011111, 0x94, 0x17, 0x02, 0x91, 0x92 };
        REQUIRE(std::equal(std::cbegin(bytes), std::cend(bytes), std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a 3 byte tag and 3 byte value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueThreeBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag), std::cend(tlvBer.Tag), std::cbegin(tagThreeBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value), std::cend(tlvBer.Value), std::cbegin(valueThreeBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0b11011111, 0x94, 0x17, 0x03, 0x91, 0x92, 0x93 };
        REQUIRE(std::equal(std::cbegin(bytes), std::cend(bytes), std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a value that requires 2,3,4,5 length octets works")
    {
        TlvBer::Builder builder{};
        for (const auto &[numMinBytes, minSize] : minSizesForLengthOctets) {
            const auto valueOctets = test::getOctets(minSize);
            const auto tlvBer = builder
                              .Reset()
                              .SetTag(tagThreeBytesPrimitive)
                              .SetValue(valueOctets)
                              .Build();
            REQUIRE(std::equal(std::cbegin(tlvBer.Tag), std::cend(tlvBer.Tag), std::cbegin(tagThreeBytesPrimitive)));
            REQUIRE(std::equal(std::cbegin(tlvBer.Value), std::cend(tlvBer.Value), std::cbegin(valueOctets)));

            auto lengthEncoding = TlvBer::GetLengthEncoding(tlvBer.Value.size());
            REQUIRE(lengthEncoding.size() >= numMinBytes);
            std::size_t length = 0;
            std::size_t bytesParsed = 0;
            TlvBer::ParseLength(length, lengthEncoding, bytesParsed);
            REQUIRE(length == minSize);
        }
    }

    SECTION("resetting TlvBer::Builder works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                        .SetTag(tagThreeBytesPrimitive)
                        .SetValue(valueFiveBytes)
                        .Reset()
                        .Build();
        REQUIRE(tlvBer.Tag.empty());
        REQUIRE(tlvBer.Value.empty());
    }

    SECTION("creating a TlvBer with a nested TLV value works as expected")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();

        auto parent = builder
                          .Reset()
                          .SetTag(tagTwoBytesConstructed)
                          .AddTlv(child)
                          .Build();
        REQUIRE(std::equal(std::cbegin(parent.Tag), std::cend(parent.Tag), std::cbegin(tagTwoBytesConstructed)));
        REQUIRE(std::equal(std::cbegin(parent.Value), std::cend(parent.Value), std::cbegin(child.ToBytes())));
    }

    SECTION("creating a TlvBer with a nested TLV value that itself has a nested TLV value works as expected")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();

        auto parent = builder
                          .Reset()
                          .SetTag(tagTwoBytesConstructed)
                          .AddTlv(child)
                          .Build();

        auto parentparent = builder
                                .Reset()
                                .SetTag(tagTwoBytesConstructed)
                                .AddTlv(parent)
                                .Build();
        REQUIRE(std::equal(std::cbegin(parentparent.Tag), std::cend(parentparent.Tag), std::cbegin(tagTwoBytesConstructed)));
        REQUIRE(std::equal(std::cbegin(parentparent.Value), std::cend(parentparent.Value), std::cbegin(parent.ToBytes())));
    }

    SECTION("adding a tlv to a tag that is primitive throws an error")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();
        auto parent = builder
                          .Reset()
                          .SetTag(tagTwoBytesPrimitive)
                          .AddTlv(child);
        REQUIRE_THROWS(builder.Build());
    }

    SECTION("setting the value to a primitive while the tag is Constructed throws an error")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesConstructed)
                         .SetValue(valueTwoBytes);
        REQUIRE_THROWS(builder.Build());
    }

    SECTION("An empty primitive value works")
    {
        TlvBer::Builder builder;
        builder.SetTag(tagTwoBytesPrimitive);
        REQUIRE_NOTHROW(builder.Build());
        auto value = builder.Build();
        REQUIRE(value.Value.empty());
    }

    SECTION("An empty constructed value works")
    {
        TlvBer::Builder builder;
        builder.SetTag(tagTwoBytesConstructed);
        REQUIRE_NOTHROW(builder.Build());
        auto value = builder.Build();
        REQUIRE(value.Value.empty());
    }

    SECTION("Parsing a primitive tlv works")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();
        auto childBytes = child.ToBytes();
        std::unique_ptr<TlvBer> tlvParsed;
        auto presult = TlvBer::Parse(notstd::unique_ptr_out(tlvParsed), childBytes);
        REQUIRE(presult == Tlv::ParseResult::Succeeded);

        REQUIRE(tlvParsed != nullptr);
        REQUIRE(std::equal(std::cbegin(child.Tag), std::cend(child.Tag), std::cbegin(tlvParsed->Tag)));
        REQUIRE(std::equal(std::cbegin(child.Value), std::cend(child.Value), std::cbegin(tlvParsed->Value)));
    }

    SECTION("Parsing a a one level constructed tlv works")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();
        auto childBytes = child.ToBytes();
        auto child2 = builder
                          .Reset()
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueTwoBytes)
                          .Build();
        auto childBytes2 = child2.ToBytes();

        auto parent = builder
                          .Reset()
                          .SetTag(tagTwoBytesConstructed)
                          .AddTlv(child)
                          .AddTlv(child2)
                          .Build();
        auto parentBytes = parent.ToBytes();

        std::unique_ptr<TlvBer> tlvParsed;
        auto presult = TlvBer::Parse(notstd::unique_ptr_out(tlvParsed), parentBytes);
        REQUIRE(presult == Tlv::ParseResult::Succeeded);

        REQUIRE(tlvParsed != nullptr);
        REQUIRE(std::equal(std::cbegin(parent.Tag), std::cend(parent.Tag), std::cbegin(tlvParsed->Tag)));
        REQUIRE(std::equal(std::cbegin(parent.Value), std::cend(parent.Value), std::cbegin(tlvParsed->Value)));
        auto pValuesConstructed = parent.GetValues();
        auto pValuesConstructedParsed = tlvParsed->GetValues();
        REQUIRE(std::equal(std::cbegin(pValuesConstructed), std::cend(pValuesConstructed), std::cbegin(pValuesConstructedParsed)));
    }

    SECTION("Parsing a a two level constructed tlv works")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();
        auto childBytes = child.ToBytes();
        auto child2 = builder
                          .Reset()
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueTwoBytes)
                          .Build();
        auto childBytes2 = child2.ToBytes();

        auto parent = builder
                          .Reset()
                          .SetTag(tagTwoBytesConstructed)
                          .AddTlv(child)
                          .AddTlv(child2)
                          .Build();
        auto parentBytes = parent.ToBytes();

        auto parentparent = builder
                                .Reset()
                                .SetTag(tagTwoBytesConstructed)
                                .AddTlv(child)
                                .AddTlv(child2)
                                .AddTlv(parent)
                                .Build();

        auto parentparentBytes = parentparent.ToBytes();

        std::unique_ptr<TlvBer> tlvParsed;
        auto presult = TlvBer::Parse(notstd::unique_ptr_out(tlvParsed), parentparentBytes);
        REQUIRE(presult == Tlv::ParseResult::Succeeded);

        REQUIRE(tlvParsed != nullptr);
        REQUIRE(std::equal(std::cbegin(parentparent.Tag), std::cend(parentparent.Tag), std::cbegin(tlvParsed->Tag)));
        REQUIRE(std::equal(std::cbegin(parentparent.Value), std::cend(parentparent.Value), std::cbegin(tlvParsed->Value)));
        auto pValuesConstructed = parentparent.GetValues();
        auto pValuesConstructedParsed = tlvParsed->GetValues();
        REQUIRE(std::equal(std::cbegin(pValuesConstructed), std::cend(pValuesConstructed), std::cbegin(pValuesConstructedParsed)));
    }

    SECTION("SetAsCopyOfTlv works for primitives"){
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagTwoBytesPrimitive)
                          .SetValue(0x43)
                          .Build();
        auto tlvBerCopy = builder
                          .Reset()
                          .SetAsCopyOfTlv(tlvBer)
                          .Build();

        REQUIRE(std::equal(std::cbegin(tlvBer.Tag), std::cend(tlvBer.Tag), std::cbegin(tlvBerCopy.Tag)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value), std::cend(tlvBer.Value), std::cbegin(tlvBerCopy.Value)));
        auto pValuesConstructed = tlvBer.GetValues();
        auto pValuesConstructedCopy = tlvBerCopy.GetValues();
        REQUIRE(std::equal(std::cbegin(pValuesConstructed), std::cend(pValuesConstructed), std::cbegin(pValuesConstructedCopy)));
    }

    SECTION("SetAsCopyOfTlv works for two layer constructed"){
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytesPrimitive)
                         .SetValue(valueTwoBytes)
                         .Build();
        auto child2 = builder
                          .Reset()
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueTwoBytes)
                          .Build();

        auto parent = builder
                          .Reset()
                          .SetTag(tagTwoBytesConstructed)
                          .AddTlv(child)
                          .AddTlv(child2)
                          .Build();

        auto parentparent = builder
                                .Reset()
                                .SetTag(tagTwoBytesConstructed)
                                .AddTlv(child)
                                .AddTlv(child2)
                                .AddTlv(parent)
                                .Build();

        auto tlvBerCopy = builder
                          .Reset()
                          .SetAsCopyOfTlv(parentparent)
                          .Build();

        REQUIRE(std::equal(std::cbegin(parentparent.Tag), std::cend(parentparent.Tag), std::cbegin(tlvBerCopy.Tag)));
        REQUIRE(std::equal(std::cbegin(parentparent.Value), std::cend(parentparent.Value), std::cbegin(tlvBerCopy.Value)));
        auto pValuesConstructed = parentparent.GetValues();
        auto pValuesConstructedCopy = tlvBerCopy.GetValues();
        REQUIRE(std::equal(std::cbegin(pValuesConstructed), std::cend(pValuesConstructed), std::cbegin(pValuesConstructedCopy)));
    }

    SECTION("UwbSessionData convert to TlvBer and back again works"){
        using namespace uwb::protocol::fira;
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

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
