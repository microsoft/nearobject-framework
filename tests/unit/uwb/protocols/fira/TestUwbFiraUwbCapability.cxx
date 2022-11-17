
#include <catch2/catch_test_macros.hpp>

#include <notstd/utility.hxx>

#include <algorithm>
#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>

#include <TlvBer.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;
using encoding::TlvBer;

bool
tagIsPresent(UwbCapability::ParameterTag tag, const encoding::TlvBer& tlv)
{
    return std::ranges::any_of(tlv.GetValuesRef(), [&tag](const auto& subTlv) {
        const auto subTlvTag = subTlv.GetTag();
        return (subTlvTag.size() == 1) && (subTlvTag[0] == notstd::to_underlying(tag));
    });
}

TEST_CASE("sanity", "[basic]")
{
    // Convert a default-constructed UwbCapability object to an OOB data object (tlv).
    auto tlv = UwbCapability{}.ToOobDataObject();

    // Validate tag is as expected.
    const auto tag = tlv->GetTag();
    REQUIRE(tag.size() == 1);
    REQUIRE(tag[0] == UwbCapability::Tag);

    // Validate type is constructed.
    REQUIRE(tlv->IsConstructed());

    // Validate values are as expected.
    auto values = tlv->GetValues();
    REQUIRE(values.size() == 1);

    // Validate FiraPhyVersionRange parameter child tlv.
    {
        REQUIRE(tagIsPresent(UwbCapability::ParameterTag::FiraPhyVersionRange, *tlv));
        auto tlvPhyRange = values[0];

        // Validate tag.
        auto phyRangeTag = tlvPhyRange.GetTag();
        REQUIRE(tlvPhyRange.IsPrimitive());
        REQUIRE(phyRangeTag.size() == 1);
        REQUIRE(phyRangeTag[0] == notstd::to_underlying(UwbCapability::ParameterTag::FiraPhyVersionRange));

        // Validate type is primitive.
        REQUIRE(tlvPhyRange.IsPrimitive());

        // Validate value.
        auto phyRangeValue = tlvPhyRange.GetValue();
        constexpr std::array<uint8_t, 4> phyRangeExpected{ 0xAA, 0xBB, 0xCC, 0xDD };
        REQUIRE(std::equal(std::cbegin(phyRangeExpected), std::cend(phyRangeExpected), std::cbegin(phyRangeValue), std::cend(phyRangeValue)));
    }
}

// TEST_CASE("uwb capability", "[basic][protocol]")
// {
//     SECTION("FromOobDataObject works with a default UwbCapability TlvBer")
//     {
//         UwbCapability defaultCapability;
//         auto tlv = defaultCapability.ToOobDataObject();
//         REQUIRE(tlv);
//         for (const auto& value : tlv->GetValuesRef()) {
//         }
//         // check that all of the fields are represented in the tlv
//         for (const auto& tag : UwbCapability::ParameterTags) {
//             auto found = tagIsPresent(tag, *tlv);
//             if (not found) {
//                 REQUIRE(false);
//             }
//         }

//         UwbCapability newCapability;
//         REQUIRE_NOTHROW(newCapability = UwbCapability::FromOobDataObject(*tlv));
//         REQUIRE(newCapability.check(defaultCapability));
//     }

//     SECTION("FromOobDataObject correctly throws error with an invalid TlvBer tag")
//     {
//         UwbCapability defaultCapability;
//         auto tlv = defaultCapability.ToOobDataObject();

//         encoding::TlvBer::Builder builder;
//         builder.SetAsCopyOfTlv(*tlv)
//             .SetTag(0xFF);
//     }
// }
