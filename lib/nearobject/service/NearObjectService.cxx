
#include <nearobject/service/NearObjectService.hxx>
#include <nearobject/service/NearObjectDeviceControllerManager.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>

#include <notstd/memory.hxx>

using namespace nearobject::service;

NearObjectService::NearObjectService(NearObjectServiceInjector injector) :
    DeviceManager(std::move(injector.DeviceManager)),
    ProfileManager(std::move(injector.ProfileManager))
{
}

std::shared_ptr<NearObjectService>
NearObjectService::Create(NearObjectServiceInjector injector)
{
    return std::make_shared<notstd::enable_make_protected<NearObjectService>>(std::move(injector));
}

std::shared_ptr<NearObjectService>
NearObjectService::GetInstance()
{
    return shared_from_this();
}
