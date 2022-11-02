#include <TlvBer.hxx>
#include <catch2/catch_test_macros.hpp>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

using namespace encoding;

TEST_CASE("test TlvBer", "[basic][infra]")
{
    static constexpr std::array<uint8_t,2> tagTwoBytesPrimitive { 0x93, 0x14 };
    static constexpr std::array<uint8_t,2> tagTwoBytesConstructed { 0xB3, 0x14 };
    static constexpr std::array<uint8_t,3> tagThreeBytesPrimitive { 0x93, 0x94, 0x17 };
    static constexpr std::array<uint8_t,2> valueTwoBytes { 0x91, 0x92 };
    static constexpr std::array<uint8_t,3> valueThreeBytes { 0x91, 0x92 , 0x93 };
    static constexpr std::array<uint8_t,4> valueFourBytes { 0x91, 0x92 , 0x93, 0x94 };
    static constexpr std::array<uint8_t,5> valueFiveBytes { 0x91, 0x92 , 0x93, 0x94, 0x95};

    SECTION("creating a TlvBer from an empty Builder holds no data")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .Build();
        REQUIRE(tlvBer.Tag.size() == 0);
        REQUIRE(tlvBer.Value.size() == 0);
    }
    
    SECTION("creating a TlvBer with no value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(0x93)
                          .Build();
        REQUIRE(tlvBer.Tag.size() == 1);
        REQUIRE(tlvBer.Tag[0] == 0x93);
        REQUIRE(tlvBer.Value.size() == 0);
    }

    SECTION("creating a TlvBer with a primitive value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(0x93)
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
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagTwoBytesPrimitive)));

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
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagTwoBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value),std::cend(tlvBer.Value),std::cbegin(valueTwoBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0x93, 0x14, 0x02, 0x91, 0x92 };
        REQUIRE(std::equal(std::cbegin(bytes),std::cend(bytes),std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a 3 byte tag and 2 byte value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueTwoBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagThreeBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value),std::cend(tlvBer.Value),std::cbegin(valueTwoBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0x93, 0x94, 0x17, 0x02, 0x91, 0x92 };
        REQUIRE(std::equal(std::cbegin(bytes),std::cend(bytes),std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a 3 byte tag and 3 byte value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueThreeBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagThreeBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value),std::cend(tlvBer.Value),std::cbegin(valueThreeBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0x93, 0x94, 0x17, 0x03, 0x91, 0x92, 0x93 };
        REQUIRE(std::equal(std::cbegin(bytes),std::cend(bytes),std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a 3 byte tag and 4 byte value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueFourBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagThreeBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value),std::cend(tlvBer.Value),std::cbegin(valueFourBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0x93, 0x94, 0x17, 0x04, 0x91, 0x92, 0x93, 0x94 };
        REQUIRE(std::equal(std::cbegin(bytes),std::cend(bytes),std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a 3 byte tag and 5 byte value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagThreeBytesPrimitive)
                          .SetValue(valueFiveBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagThreeBytesPrimitive)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value),std::cend(tlvBer.Value),std::cbegin(valueFiveBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0x93, 0x94, 0x17, 0x05, 0x91, 0x92, 0x93, 0x94, 0x95 };
        REQUIRE(std::equal(std::cbegin(bytes),std::cend(bytes),std::cbegin(desired)));
    }

    SECTION("resetting TlvBer::Builder works as expected")
    {
        TlvBer::Builder builder{};
        auto asdf =  builder
                     .SetTag(tagThreeBytesPrimitive)
                     .SetValue(valueFiveBytes)
                     .Reset()
                     .Build();
        REQUIRE(asdf.Tag.size() == 0);
        REQUIRE(asdf.Value.size() == 0);
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
        REQUIRE(std::equal(std::cbegin(parent.Tag),std::cend(parent.Tag),std::cbegin(tagTwoBytesConstructed)));
        REQUIRE(std::equal(std::cbegin(parent.Value),std::cend(parent.Value),std::cbegin(child.ToBytes())));

        auto parent2 = builder
                          .Reset()
                          .SetTag(tagTwoBytesConstructed)
                          .AddTlv(tagTwoBytesPrimitive,valueTwoBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(parent2.Tag),std::cend(parent2.Tag),std::cbegin(parent.Tag)));
        REQUIRE(std::equal(std::cbegin(parent2.Value),std::cend(parent2.Value),std::cbegin(parent.Value)));
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
        REQUIRE(std::equal(std::cbegin(parentparent.Tag),std::cend(parentparent.Tag),std::cbegin(tagTwoBytesConstructed)));
        REQUIRE(std::equal(std::cbegin(parentparent.Value),std::cend(parentparent.Value),std::cbegin(parent.ToBytes())));
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
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
