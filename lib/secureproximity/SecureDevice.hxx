
#ifndef __SECURE_DEVICE_HXX__
#define __SECURE_DEVICE_HXX__

#include "ISecureDevice.hxx"

class SecureDevice :
    public ISecureDevice
{
    ISecureDeviceChannel* 
    CreateChannel(SecureDeviceChannelParameters) override;
};

#endif //__SECURE_DEVICE_HXX__
