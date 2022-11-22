
#include <nearobject/service/NearObjectDeviceControllerManager.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>
#include <nearobject/service/NearObjectService.hxx>
#include <nearobject/service/NearObjectSessionIdGeneratorRandom.hxx>

#include <notstd/memory.hxx>

using namespace nearobject::service;

NearObjectService::NearObjectService(NearObjectServiceInjector injector) :
    DeviceManager(std::move(injector.DeviceManager)),
    ProfileManager(std::move(injector.ProfileManager)),
    SessionIdGenerator(std::move(injector.SessionIdGenerator))
{
    if (SessionIdGenerator == nullptr) {
        SessionIdGenerator = std::make_unique<NearObjectSessionIdGeneratorRandom>();
    }
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
