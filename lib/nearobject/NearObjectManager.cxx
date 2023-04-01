
#include <nearobject/NearObjectManager.hxx>
#include <nearobject/service/NearObjectService.hxx>

using namespace nearobject;

std::vector<NearObjectProfile>
NearObjectManager::FindAllConnectionProfiles() const
{
    // TODO: proxy to service NearObjectProfileManager, or, use it directly
    // here with a shared instance
    return {};
}

NearObjectManager::NearObjectCreateSessionResult
NearObjectManager::CreateSession(const NearObjectProfile& /* connectionProfile */)
{
    // TODO: proxy to valid NearObjectDeviceControllerManager
    return {};
}
