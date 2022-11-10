#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include <TlvBer.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/RangingConfiguration.hxx>
#include <uwb/protocols/fira/UwbCapability.hxx>

using namespace uwb::protocol::fira;

UwbCapability
FromOobDataObject(const encoding::TlvBer& tlv)
{
    for (auto subtlv : tlv.GetValues()){
        auto dataObject = DataObject.matchTag(subtlv.Tag);
        switch(dataObject){
            
            default:
                // error case
                throw TagNotFoundError();
        }
    }
}