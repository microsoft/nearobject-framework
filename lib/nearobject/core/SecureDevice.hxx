
#ifndef __SECURE_DEVICE_HXX__
#define __SECURE_DEVICE_HXX__

#include "SecureDeviceChannel.hxx"

namespace nearobject
{

class SecureDevice
{
public:
    SecureDeviceChannel* 
    CreateChannel(SecureDeviceChannelParameters);

private:
    virtual SecureDeviceChannel* 
    CreateChannelImpl(SecureDeviceChannelParameters) = 0;
};

} // namespace nearobject

#endif //__SECURE_DEVICE_HXX__
