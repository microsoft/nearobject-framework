
#ifndef NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX
#define NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX

#include "NearObjectDiscoverySource.hxx"

namespace nearobject::discovery
{
class NearObjectDiscoverySourceBluetooth :
    public NearObjectDiscoverySource
{
public:
    std::unique_ptr<NearObjectOobConnection>
    EstablishConnectionImpl() override;
};

} // namespace nearobject::discovery

#endif // NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX
