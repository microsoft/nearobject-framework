
#include "OobManager.hxx"

using namespace windows::uwb;

void
OobManager::RegisterOobChannel(std::unique_ptr<::uwb::OobChannel> oobChannel)
{
    const auto oobChannelsLock = std::scoped_lock(m_oobChannelsGate);
    m_oobChannels.push_back(std::move(oobChannel));
}
