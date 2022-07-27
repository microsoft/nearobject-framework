
#ifndef __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__
#define __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__

#include "OobChannel.hxx"

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
    RegisterOobChannel(std::unique_ptr<OobChannel> oobChannel);

private:
    std::mutex m_oobChannelsGate;
    std::vector<std::unique_ptr<OobChannel>> m_oobChannels{};
};

} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_DISCOVERY_SERVICE_HXX__
