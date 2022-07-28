
#include "NearObjectProfileManager.hxx"
#include "NearObjectService.hxx"
#include "NearObjectDeviceManager.hxx"

#include <notstd/memory.hxx>

using namespace nearobject::service;

NearObjectService::NearObjectService(NearObjectServiceInjector&& injector) :
    DeviceManager(injector.DeviceManager),
    ProfileManager(injector.ProfileManager)
{
}

std::shared_ptr<NearObjectService>
NearObjectService::Create(NearObjectServiceInjector&& injector)
{
    return std::make_shared<notstd::enable_make_protected<NearObjectService>>(std::move(injector));
}

std::shared_ptr<NearObjectService>
NearObjectService::GetInstance()
{
    return shared_from_this();
}
