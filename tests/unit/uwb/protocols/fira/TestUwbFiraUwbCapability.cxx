
#include <catch2/catch_test_macros.hpp>

#include <notstd/utility.hxx>

#include <array>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <memory>

#include <uwb/protocols/fira/UwbCapability.hxx>
#include <TlvBer.hxx>

using namespace uwb::protocol::fira;
using encoding::TlvBer;

bool tagIsPresent(UwbCapability::ParameterTag tag, const encoding::TlvBer& tlv){
    const auto tagValue = notstd::to_underlying(tag); 
    std::cout << std::endl << "looking for tag value: " << std::hex << static_cast<int>(tagValue) << " -> ";
    for (auto subtlv : tlv.GetValuesRef()){
        const auto subTlvTag = subtlv.Tag[0];
        CHECK(!subtlv.Tag.empty());
        std::cout << std::hex << static_cast<int>(subTlvTag) << ", ";
        if (subTlvTag == tagValue) return true;
        // if(subtlv.Tag.size()==1 and subtlv.Tag[0] == notstd::to_underlying(tag)) return true;
    }
    return false;
}

TEST_CASE("sanity", "[basic]")
{
    UwbCapability uwbCapability;
    std::unique_ptr<TlvBer> tlv = uwbCapability.ToOobDataObject();
    const auto tag = tlv->GetTag(); 
    REQUIRE(tag.size() == 1);
    REQUIRE(tag[0] == UwbCapability::Tag);
    auto values = tlv->GetValues();
    REQUIRE(values.size() == 1);
    auto tlvPhyRange = values[0];
    auto phyRangeTag = tlvPhyRange.GetTag();
    REQUIRE(phyRangeTag.size() == 1);
    REQUIRE(phyRangeTag[0] == notstd::to_underlying(UwbCapability::ParameterTag::FiraPhyVersionRange));
    REQUIRE(tlvPhyRange.Tag.size() == 1);
    auto phyRangeTagRaw = tlvPhyRange.Tag;
    std::cout << "raw tag expected " << std::hex << static_cast<int>(UwbCapability::ParameterTag::FiraPhyVersionRange) << ", actual " << std::hex << static_cast<int>(phyRangeTagRaw[0]) << std::endl;
    CHECK(tlvPhyRange.Tag[0] == notstd::to_underlying(UwbCapability::ParameterTag::FiraPhyVersionRange));
    // auto& phyRangeValue = tlvPhyRange.Value;
    auto phyRangeValue = tlvPhyRange.GetValue();
    CHECK(phyRangeValue.size() == 4);
    constexpr std::array<uint8_t, 4> phyRangeExpected{ 0xAA, 0xBB, 0xCC, 0xDD };
    int i = 0;
    for (const auto b : phyRangeValue) {
        std::cout << "actual: " << std::hex << static_cast<int>(b) << ", expected " << std::hex << static_cast<int>(phyRangeExpected[i]) << std::endl;
        i++;
        CHECK(b == phyRangeExpected[i]);
    }
    // REQUIRE(std::equal(std::cbegin(phyRangeExpected), std::cend(phyRangeExpected), std::cbegin(phyRangeValue), std::cend(phyRangeValue)));
}

// TEST_CASE("uwb capability", "[basic][protocol]")
// {
//     SECTION("FromOobDataObject works with a default UwbCapability TlvBer"){
//         UwbCapability defaultCapability;
//         auto tlv = defaultCapability.ToOobDataObject();
//         std::cout << "tlv first cvalue tag: " << std::hex << static_cast<int>(tlv->GetValues()[0].Tag[0]) << std::endl;
//         REQUIRE(tlv);
//         for (const auto& value : tlv->GetValuesRef()) {
//             std::cout << "got subTlv tag " << std::hex << static_cast<int>(value.Tag[0]) << std::endl;
//         }
//         // check that all of the fields are represented in the tlv
//         for (const auto& tag : UwbCapability::ParameterTags){
//             auto found = tagIsPresent(tag,*tlv);
//             if(not found) {
//                 printf("couldn't find %x",std::uint8_t(tag));
//                 REQUIRE(false);
//             }
//         }

//         UwbCapability newCapability;
//         REQUIRE_NOTHROW(newCapability = UwbCapability::FromOobDataObject(*tlv));
//         REQUIRE(newCapability.check(defaultCapability));
//     }

//     SECTION("FromOobDataObject correctly throws error with an invalid TlvBer tag"){
//         UwbCapability defaultCapability;
//         auto tlv = defaultCapability.ToOobDataObject();

//         encoding::TlvBer::Builder builder;
//         builder.SetAsCopyOfTlv(*tlv)
//                 .SetTag(0xFF);
        


//     }

// }
