
#include "NearObjectConnectionProfileManager.hxx"
#include "NearObjectService.hxx"
#include "NearObjectDeviceManager.hxx"

using namespace nearobject::service;

NearObjectService::NearObjectService(NearObjectServiceInjector&& injector) :
    m_deviceManagers(std::move(injector.DeviceManagers)),
    m_connectionProfileManager(std::move(injector.ConnectionProfileManager))
{
}
