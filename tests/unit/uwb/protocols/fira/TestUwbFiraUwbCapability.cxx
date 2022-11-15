
#include <catch2/catch_test_macros.hpp>

#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;

bool tagIsPresent(UwbCapability::ParameterTag tag, const encoding::TlvBer& tlv){
    for (auto subtlv : tlv.GetValues()){
        if(subtlv.Tag.size()==1 and subtlv.Tag[0] == uint8_t(tag)) return true;
    }
    return false;
}

TEST_CASE("uwb capability", "[basic][protocol]")
{
    SECTION("FromOobDataObject works with a default UwbCapability TlvBer"){
        UwbCapability defaultCapability;
        auto tlv = defaultCapability.ToOobDataObject();
        REQUIRE(tlv);
        // check that all of the fields are represented in the tlv
        for (auto tag : UwbCapability::ParameterTags){
            auto found = tagIsPresent(tag,*tlv);
            if(not found) {
                printf("couldn't find %x",std::uint8_t(tag));
                REQUIRE(false);
            }
        }

        UwbCapability newCapability;
        REQUIRE_NOTHROW(newCapability = UwbCapability::FromOobDataObject(*tlv));
        REQUIRE(newCapability.check(defaultCapability));
    }

    SECTION("FromOobDataObject correctly throws error with an invalid TlvBer tag"){
        UwbCapability defaultCapability;
        auto tlv = defaultCapability.ToOobDataObject();

        encoding::TlvBer::Builder builder;
        builder.SetAsCopyOfTlv(*tlv)
                .SetTag(0xFF);
        


    }

}
