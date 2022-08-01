
#include "NearObjectManager.hxx"

using namespace nearobject;

NearObjectManager::NearObjectManager()
{}

std::vector<NearObjectProfile>
NearObjectManager::FindAllConnectionProfiles() const
{
    // TODO: proxy to service NearObjectProfileManager, or, use it directly
    // here with a shared instance
    return {};
}

NearObjectManager::NearObjectConnectionResult
NearObjectManager::EstablishConnection(const NearObjectProfile& connectionProfile)
{
    // TODO: proxy to valid NearObjectDeviceManager
    return {};
}
