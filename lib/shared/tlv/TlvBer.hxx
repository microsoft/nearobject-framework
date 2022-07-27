
#ifndef __TLV_BER_HXX__
#define __TLV_BER_HXX__

#include "Tlv.hxx"

#include <cstddef>
#include <vector>

namespace smartcard
{
namespace encoding
{
class TlvBer : public Tlv
{
public:
    std::vector<std::byte>
    ToVector() const override;
};

} // namespace encoding
} // namespace smartcard

#endif // __TLV_BER_HXX__
