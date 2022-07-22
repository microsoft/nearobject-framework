
#ifndef __I_SECURE_DEVICE_HXX__
#define __I_SECURE_DEVICE_HXX__

#include "ISecureDeviceChannel.hxx"

namespace nearobject
{

class ISecureDevice
{
    virtual ISecureDeviceChannel* 
    CreateChannel(SecureDeviceChannelParameters) = 0;
};

} // namespace nearobject

#endif //__I_SECURE_DEVICE_HXX__
