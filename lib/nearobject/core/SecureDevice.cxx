
#include "SecureDevice.hxx"

using namespace nearobject;

SecureDeviceChannel* 
SecureDevice::CreateChannel(SecureDeviceChannelParameters params)
{
    return CreateChannelImpl(params);
}

SecureDeviceChannel*
SecureDevice::CreateChannelImpl(SecureDeviceChannelParameters /* params */)
{
    return nullptr;
}
