
#include "NearObjectService.hxx"
#include "NearObjectDeviceManager.hxx"

using namespace nearobject::service;

void
NearObjectService::AddDeviceManager(std::unique_ptr<NearObjectDeviceManager> nearObjectDeviceManager)
{
    m_nearObjectDeviceManagers.push_back(std::move(nearObjectDeviceManager));
}
