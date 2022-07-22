
#ifndef __SECURE_DEVICE_HXX__
#define __SECURE_DEVICE_HXX__

#include "ISecureDevice.hxx"

namespace nearobject
{

class SecureDevice :
    public ISecureDevice
{
    ISecureDeviceChannel* 
    CreateChannel(SecureDeviceChannelParameters) override;
};

} // namespace nearobject

#endif //__SECURE_DEVICE_HXX__
