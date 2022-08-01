
#ifndef NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX
#define NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX

#include "NearObjectDiscoverySource.hxx"

namespace nearobject
{
namespace service
{
class NearObjectDiscoverySourceBluetooth :
    public NearObjectDiscoverySource
{
public:
    std::unique_ptr<NearObjectOobConnection>
    EstablishConnectionImpl() override;
};

} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX
