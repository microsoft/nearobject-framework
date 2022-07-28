
#include "NearObjectManager.hxx"

using namespace nearobject;

NearObjectManager::NearObjectManager()
{}

std::vector<NearObjectConnectionProfile>
NearObjectManager::FindAllConnectionProfiles() const
{
    // TODO: proxy to service NearObjectConnectionProfileManager, or, use it directly
    // here with an shared instance
    return {};
}

NearObjectManager::NearObjectConnectionResult
NearObjectManager::EstablishConnection(const NearObjectConnectionProfile& connectionProfile)
{
    // TODO: proxy to valid NearObjectDeviceManager
    return {};
}
