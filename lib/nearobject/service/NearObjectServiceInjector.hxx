
#ifndef NEAR_OBJECT_SERVICE_INJECTOR_HXX
#define NEAR_OBJECT_SERVICE_INJECTOR_HXX

#include <memory>

#include "NearObjectDeviceManager.hxx"
#include "NearObjectProfileManager.hxx"

namespace nearobject
{
namespace service
{
/**
 * @brief NearObjectService dependency injector.
 */
struct NearObjectServiceInjector
{
    std::shared_ptr<NearObjectProfileManager> ProfileManager;
    std::shared_ptr<NearObjectDeviceManager> DeviceManager;
};
} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_SERVICE_INJECTOR_HXX
