
#include "SecureDevice.hxx"

using namespace nearobject::service;

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
