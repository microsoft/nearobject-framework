
#ifndef __OOB_MANAGER_HXX__
#define __OOB_MANAGER_HXX__

#include <OobChannel.hxx>

#include <mutex>
#include <vector>

namespace windows
{
namespace uwb
{

class OobManager
{
public:
    void
    RegisterOobChannel(std::unique_ptr<::uwb::OobChannel> oobChannel);

private:
    std::mutex m_oobChannelsGate;
    std::vector<std::unique_ptr<::uwb::OobChannel>> m_oobChannels{};
};

} // namespace uwb
} // namespace windows

#endif // __OOB_MANAGER_HXX__