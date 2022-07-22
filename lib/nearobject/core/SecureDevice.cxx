
#include "SecureDevice.hxx"

#include <stdexcept>

using namespace nearobject;

ISecureDeviceChannel*
SecureDevice::CreateChannel(SecureDeviceChannelParameters)
{
    throw std::logic_error("not implemented");
}
