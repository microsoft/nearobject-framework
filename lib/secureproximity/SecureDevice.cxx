
#include "SecureDevice.hxx"

#include <stdexcept>

using namespace secureproximity;

ISecureDeviceChannel*
SecureDevice::CreateChannel(SecureDeviceChannelParameters)
{
    throw std::logic_error("not implemented");
}
