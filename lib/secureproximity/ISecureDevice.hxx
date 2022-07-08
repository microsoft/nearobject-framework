
#ifndef __I_SECURE_DEVICE_HXX__
#define __I_SECURE_DEVICE_HXX__

#include "ISecureDeviceChannel.hxx"

namespace secureproximity
{

class ISecureDevice
{
    virtual ISecureDeviceChannel* 
    CreateChannel(SecureDeviceChannelParameters) = 0;
};

} // namespace secureproximity

#endif //__I_SECURE_DEVICE_HXX__
