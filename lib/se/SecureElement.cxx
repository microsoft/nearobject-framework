
#include <se/SecureElement.hxx>

#include <stdexcept>

using namespace se;

std::shared_ptr<SecureElementChannel>
SecureElement::OpenChannel()
{
    throw std::logic_error("not implemented");
}
