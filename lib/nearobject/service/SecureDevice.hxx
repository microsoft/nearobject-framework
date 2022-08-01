
#ifndef __SECURE_DEVICE_HXX__
#define __SECURE_DEVICE_HXX__

#include "SecureDeviceChannel.hxx"

namespace nearobject
{
namespace service
{
class SecureDevice
{
public:
    virtual ~SecureDevice() = default;

    SecureDeviceChannel*
        CreateChannel(SecureDeviceChannelParameters);

private:
    virtual SecureDeviceChannel*
        CreateChannelImpl(SecureDeviceChannelParameters) = 0;
};

} // namespace service
} // namespace nearobject

#endif //__SECURE_DEVICE_HXX__
