
#ifndef __SECURE_ELEMENT_HXX__
#define __SECURE_ELEMENT_HXX__

#include "SecureElementChannel.hxx"

#include <memory>

namespace se
{
class SecureElement
{
    std::shared_ptr<SecureElementChannel>
    OpenChannel();
};

} // namespace se

#endif // __SECURE_ELEMENT_HXX__
