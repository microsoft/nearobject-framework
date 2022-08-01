
#ifndef NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX
#define NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX

#include "NearObjectDiscoverySource.hxx"

namespace nearobject
{
namespace discovery
{
class NearObjectDiscoverySourceBluetooth :
    public NearObjectDiscoverySource
{
public:
    std::unique_ptr<NearObjectOobConnection>
    EstablishConnectionImpl() override;
};

} // namespace discovery
} // namespace nearobject

#endif // NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX
