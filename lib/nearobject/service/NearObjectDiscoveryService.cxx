
#include "NearObjectDiscoveryService.hxx"

using namespace nearobject::service;

void
NearObjectDiscoveryService::RegisterOobChannel(std::unique_ptr<NearObjectDiscoverySource> oobChannel)
{
    const auto oobChannelsLock = std::scoped_lock(m_oobChannelsGate);
    m_oobChannels.push_back(std::move(oobChannel));
}
