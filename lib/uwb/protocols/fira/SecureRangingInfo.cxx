
#include <uwb/protocols/fira/SecureRangingInfo.hxx>

using namespace uwb::protocol::fira;

std::unique_ptr<encoding::TlvBer>
SecureRangingInfo::ToDataObject() const
{
    return nullptr;
}

/* static */
SecureRangingInfo
SecureRangingInfo::FromDataObject(const encoding::TlvBer& tlv)
{
    return {};
}