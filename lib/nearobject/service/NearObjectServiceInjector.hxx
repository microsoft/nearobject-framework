
#ifndef __NEAR_OBJECT_SERVICE_INJECTOR_HXX__
#define __NEAR_OBJECT_SERVICE_INJECTOR_HXX__

#include <memory>

#include "NearObjectProfileManager.hxx"
#include "NearObjectDeviceManager.hxx"

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

#endif // __NEAR_OBJECT_SERVICE_INJECTOR_HXX__
