
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
    RegisterOobChannel(std::unique_ptr<NearObjectDiscoverySource> oobChannel);

private:
    std::mutex m_oobChannelsGate;
    std::vector<std::unique_ptr<NearObjectDiscoverySource>> m_oobChannels{};
};

} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__
