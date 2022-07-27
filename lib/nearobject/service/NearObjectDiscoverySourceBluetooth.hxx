
#ifndef __NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX__
#define __NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX__

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

#endif // __NEAR_OBJECT_DISCOVERY_SOURCE_BLUETOOTH_HXX__
