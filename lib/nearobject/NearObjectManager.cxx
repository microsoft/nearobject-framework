
#include "NearObjectManager.hxx"

using namespace nearobject;

std::vector<NearObjectConnectionProfile>
NearObjectManager::FindAllConnectionProfiles() const
{
    // TODO: proxy to service ConnectionProfileManager, or, use it directly
    // here with an shared instance
    return {};
}

NearObjectManager::NearObjectConnectionResult
NearObjectManager::EstablishConnection(const NearObjectConnectionProfile& connectionProfile)
{
    // TODO: proxy to valid NearObjectDeviceManager
    return {};
}
