
#include <stdexcept>

#include <uwb/protocols/fira/UwbSessionData.hxx>

using namespace uwb::protocol::fira;

std::unique_ptr<encoding::TlvBer>
UwbSessionData::ToDataObject() const
{
    throw std::logic_error("not implemented");
}

/* static */
UwbSessionData
UwbSessionData::FromDataObject(const encoding::TlvBer& /* tlv */)
{
    throw std::logic_error("not implemented");
}
