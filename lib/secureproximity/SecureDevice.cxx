
#include "SecureDevice.hxx"

#include <stdexcept>

ISecureDeviceChannel*
SecureDevice::CreateChannel(SecureDeviceChannelParameters)
{
    throw std::logic_error("not implemented");
}
