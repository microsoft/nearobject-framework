
#ifndef __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__
#define __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__

#include "NearObjectDiscoverySource.hxx"

#include <mutex>
#include <vector>

namespace nearobject
{
namespace service
{
class NearObjectDiscoveryService
{
public:
    void
    RegisterDiscoverySource(std::unique_ptr<NearObjectDiscoverySource> discoverySource);

private:
    std::mutex m_discoverySourcesGate;
    std::vector<std::unique_ptr<NearObjectDiscoverySource>> m_discoverySources{};
};

} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__
