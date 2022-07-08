
#ifndef __SECURE_DEVICE_HXX__
#define __SECURE_DEVICE_HXX__

#include "ISecureDevice.hxx"

namespace secureproximity
{

class SecureDevice :
    public ISecureDevice
{
    ISecureDeviceChannel* 
    CreateChannel(SecureDeviceChannelParameters) override;
};

} // namespace secureproximity

#endif //__SECURE_DEVICE_HXX__
