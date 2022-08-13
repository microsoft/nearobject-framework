
#include <nearobject/discovery/NearObjectDiscoveryService.hxx>

using namespace nearobject::discovery;

void
NearObjectDiscoveryService::RegisterDiscoverySource(std::unique_ptr<NearObjectDiscoverySource> discoverySource)
{
    const auto oobChannelsLock = std::scoped_lock(m_discoverySourcesGate);
    m_discoverySources.push_back(std::move(discoverySource));
}
