
#ifndef SECURE_ELEMENT_HXX
#define SECURE_ELEMENT_HXX

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

#endif // SECURE_ELEMENT_HXX
