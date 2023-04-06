
#ifndef NEAR_OBJECT_SERVICE_INJECTOR_HXX
#define NEAR_OBJECT_SERVICE_INJECTOR_HXX

#include <memory>

#include <nearobject/service/NearObjectDeviceControllerManager.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>
#include <nearobject/service/NearObjectSessionIdGenerator.hxx>

namespace nearobject::service
{
/**
 * @brief NearObjectService dependency injector.
 */
struct NearObjectServiceInjector
{
    std::shared_ptr<NearObjectProfileManager> ProfileManager;
    std::shared_ptr<NearObjectDeviceControllerManager> DeviceManager;
    std::unique_ptr<NearObjectSessionIdGenerator> SessionIdGenerator;
};
} // namespace nearobject::service

#endif // NEAR_OBJECT_SERVICE_INJECTOR_HXX
