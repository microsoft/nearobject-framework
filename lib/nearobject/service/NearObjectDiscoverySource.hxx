
#ifndef __NEAR_OBJECT_DISCOVERY_SOURCE_HXX__
#define __NEAR_OBJECT_DISCOVERY_SOURCE_HXX__

#include "OobConnection.hxx"

#include <memory>

namespace nearobject
{
namespace service
{
class NearObjectDiscoverySource
{
public:
    virtual ~NearObjectDiscoverySource() = default;

    std::unique_ptr<OobConnection>
    EstablishConnection();

private:
    virtual std::unique_ptr<OobConnection>
    EstablishConnectionImpl() = 0;
};

} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_DISCOVERY_SOURCE_HXX__
