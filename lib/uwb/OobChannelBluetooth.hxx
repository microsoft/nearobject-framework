
#ifndef __OOB_CHANNEL_BLUETOOTH_HXX__
#define __OOB_CHANNEL_BLUETOOTH_HXX__

#include "OobChannel.hxx"

namespace uwb
{

class OobChannelBluetooth :
    public OobChannel
{
public:
    std::unique_ptr<OobConnection>
    EstablishConnection() override;
};

} // namespace uwb

#endif // __OOB_CHANNEL_HXX__