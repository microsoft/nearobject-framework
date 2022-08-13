
#ifndef NEAR_OBJECT_DISCOVERY_SERVICE_HXX
#define NEAR_OBJECT_DISCOVERY_SERVICE_HXX

#include "NearObjectDiscoverySource.hxx"

#include <mutex>
#include <vector>

namespace nearobject
{
namespace discovery
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

} // namespace discovery
} // namespace nearobject

#endif // NEAR_OBJECT_DISCOVERY_SERVICE_HXX
