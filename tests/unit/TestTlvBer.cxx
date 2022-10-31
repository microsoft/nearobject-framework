#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <TlvBer.hxx>
#include <string>
#include <vector>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

using namespace encoding;

template <class A,class B>
bool byte_array_matches(const A& a,const B& b){
    std::vector<uint8_t> avec(std::cbegin(a),std::cend(a));
    std::vector<uint8_t> bvec(std::cbegin(b),std::cend(b));
    if(avec.size()!= bvec.size()) return false;
    for(auto ait = std::begin(avec), bit = std::begin(bvec); ait!=std::end(avec); ){
        if(uint8_t(*ait)!=uint8_t(*bit)) return false;
        ait++;
        bit++;
    }
    return true;
}

TEST_CASE("test TlvBer", "[basic][infra]")
{
    SECTION("creating a TlvBer with no value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                    .SetTag('\x93')
                    .Build();
        REQUIRE(tlvBer.Tag.size() == 1);
        REQUIRE(tlvBer.Tag[0]==0x93);
        REQUIRE(tlvBer.Value.size() == 0);
    }

    SECTION("creating a TlvBer with a primitive value works as expected")
    {
        TlvBer::Builder builder{};
        auto tlvBer = builder
                    .SetTag('\x93')
                    .SetValue('\x43')
                    .Build();
        REQUIRE(tlvBer.Tag.size() == 1);
        REQUIRE(tlvBer.Tag[0]==0x93);
        REQUIRE(tlvBer.Value.size() == 1);
        REQUIRE(tlvBer.Value[0] == 0x43);
    }

    SECTION("creating a TlvBer with a long tag works as expected")
    {
        TlvBer::Builder builder{};
        const uint8_t tag[] = {0x93,0x94};
        auto tlvBer = builder
                    .SetTag(tag)
                    .SetValue('\x43')
                    .Build();
        REQUIRE(tlvBer.Tag.size() == 2);
        REQUIRE(tlvBer.Tag[0]==tag[0]);
        REQUIRE(tlvBer.Tag[1]==tag[1]);
        REQUIRE(tlvBer.Value.size() == 1);
        REQUIRE(tlvBer.Value[0] == 0x43);
    }

    SECTION("creating a TlvBer with a long tag and value works as expected")
    {
        TlvBer::Builder builder{};
        const uint8_t tag[] = {0x93,0x94};
        const uint8_t value[] = {0x91,0x92};
        auto tlvBer = builder
                    .SetTag(tag)
                    .SetValue(value)
                    .Build();
        REQUIRE(tlvBer.Tag.size() == 2);
        REQUIRE(tlvBer.Tag[0]==tag[0]);
        REQUIRE(tlvBer.Tag[1]==tag[1]);
        REQUIRE(tlvBer.Value.size() == 2);
        REQUIRE(tlvBer.Value[0] == value[0]);
        REQUIRE(tlvBer.Value[1] == value[1]);

        auto bytes = tlvBer.ToBytes();
        REQUIRE(byte_array_matches(bytes,std::vector<uint8_t>{0x93,0x94,0x02,0x91,0x92}));
    }

    SECTION("creating a TlvBer with a nested TLV value works as expected")
    {
        TlvBer::Builder builder{};
        const uint8_t tag[] = {0x93,0x94};
        const uint8_t value[] = {0x91,0x92};
        auto child = builder
                    .SetTag(tag)
                    .SetValue(value)
                    .Build();

        const uint8_t ptag[] = {0xB3,0x94};
        auto parent = builder
                    .Reset()
                    .SetTag(ptag)
                    .AddTlv(child)
                    .Build();
        REQUIRE(byte_array_matches(parent.Tag,ptag));
        REQUIRE(byte_array_matches(parent.Value,child.ToBytes()));
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
