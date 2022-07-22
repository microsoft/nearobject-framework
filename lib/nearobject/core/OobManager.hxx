
#ifndef __OOB_MANAGER_HXX__
#define __OOB_MANAGER_HXX__

#include "OobChannel.hxx"

#include <mutex>
#include <vector>

namespace nearobject
{

class OobManager
{
public:
    void
    RegisterOobChannel(std::unique_ptr<OobChannel> oobChannel);

private:
    std::mutex m_oobChannelsGate;
    std::vector<std::unique_ptr<OobChannel>> m_oobChannels{};
};

} // namespace nearobject

#endif // __OOB_MANAGER_HXX__