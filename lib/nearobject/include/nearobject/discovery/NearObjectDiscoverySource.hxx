
#ifndef NEAR_OBJECT_DISCOVERY_SOURCE_HXX
#define NEAR_OBJECT_DISCOVERY_SOURCE_HXX

#include "NearObjectOobConnection.hxx"

#include <memory>

namespace nearobject::discovery
{
class NearObjectDiscoverySource
{
public:
    virtual ~NearObjectDiscoverySource() = default;

    std::unique_ptr<NearObjectOobConnection>
    EstablishConnection();

private:
    virtual std::unique_ptr<NearObjectOobConnection>
    EstablishConnectionImpl() = 0;
};

} // namespace nearobject::discovery

#endif // NEAR_OBJECT_DISCOVERY_SOURCE_HXX
