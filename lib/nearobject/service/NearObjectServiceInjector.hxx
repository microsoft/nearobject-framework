
#ifndef __NEAR_OBJECT_SERVICE_INJECTOR_HXX__
#define __NEAR_OBJECT_SERVICE_INJECTOR_HXX__

#include <memory>
#include <vector>

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
    std::unique_ptr<NearObjectProfileManager> ConnectionProfileManager;
    std::vector<std::unique_ptr<NearObjectDeviceManager>> DeviceManagers;
};
} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_SERVICE_INJECTOR_HXX__