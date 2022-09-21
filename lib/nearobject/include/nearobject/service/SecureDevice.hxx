
#ifndef SECURE_DEVICE_HXX
#define SECURE_DEVICE_HXX

#include <nearobject/service/SecureDeviceChannel.hxx>

namespace nearobject::service
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

} // namespace nearobject::service

#endif // SECURE_DEVICE_HXX
