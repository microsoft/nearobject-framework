#include <TlvBer.hxx>
#include <catch2/catch_test_macros.hpp>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

using namespace encoding;

template<class A, class B>
bool
byte_array_matches(const A& a, const B& b)
{
    std::vector<uint8_t> avec(std::cbegin(a), std::cend(a));
    std::vector<uint8_t> bvec(std::cbegin(b), std::cend(b));
    if (avec.size() != bvec.size())
        return false;
    for (auto ait = std::begin(avec), bit = std::begin(bvec); ait != std::end(avec);) {
        if (uint8_t(*ait) != uint8_t(*bit))
            return false;
        ait++;
        bit++;
    }
    return true;
}

TEST_CASE("test TlvBer", "[basic][infra]")
{
    const std::array<uint8_t,2> tagTwoBytes { 0x93, 0x94 };
    const std::array<uint8_t,2> valueTwoBytes { 0x91, 0x92 };

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
                          .SetTag(tagTwoBytes)
                          .SetValue(0x43)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagTwoBytes)));

        REQUIRE(tlvBer.Value.size() == 1);
        REQUIRE(tlvBer.Value[0] == 0x43);
    }

    SECTION("creating a TlvBer with a long tag and value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                          .SetTag(tagTwoBytes)
                          .SetValue(valueTwoBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(tlvBer.Tag),std::cend(tlvBer.Tag),std::cbegin(tagTwoBytes)));
        REQUIRE(std::equal(std::cbegin(tlvBer.Value),std::cend(tlvBer.Value),std::cbegin(valueTwoBytes)));

        auto bytes = tlvBer.ToBytes();
        std::vector<uint8_t> desired{ 0x93, 0x94, 0x02, 0x91, 0x92 };
        REQUIRE(std::equal(std::cbegin(bytes),std::cend(bytes),std::cbegin(desired)));
    }

    SECTION("creating a TlvBer with a nested TLV value works as expected")
    {
        TlvBer::Builder builder{};
        auto child = builder
                         .SetTag(tagTwoBytes)
                         .SetValue(valueTwoBytes)
                         .Build();

        const uint8_t ptag[] = { 0xB3, 0x94 };
        auto parent = builder
                          .Reset()
                          .SetTag(ptag)
                          .AddTlv(child)
                          .Build();
        REQUIRE(std::equal(std::cbegin(parent.Tag),std::cend(parent.Tag),std::cbegin(ptag)));
        REQUIRE(std::equal(std::cbegin(parent.Value),std::cend(parent.Value),std::cbegin(child.ToBytes())));

        auto parent2 = builder
                          .Reset()
                          .SetTag(ptag)
                          .AddTlv(tagTwoBytes,valueTwoBytes)
                          .Build();
        REQUIRE(std::equal(std::cbegin(parent2.Tag),std::cend(parent2.Tag),std::cbegin(parent.Tag)));
        REQUIRE(std::equal(std::cbegin(parent2.Value),std::cend(parent2.Value),std::cbegin(parent.Value)));
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
