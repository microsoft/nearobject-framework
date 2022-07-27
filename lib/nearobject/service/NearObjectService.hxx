
#ifndef __NEAR_OBJECT_SERVICE_HXX__
#define __NEAR_OBJECT_SERVICE_HXX__

#include <memory>
#include <vector>

#include "NearObjectServiceInjector.hxx"

namespace nearobject
{
namespace service
{
class NearObjectDeviceManager;
class NearObjectConnectionProfileManager;

/**
 * @brief Central service object managing all other framework components and
 * their lifetime.
 */
class NearObjectService
{
public:
    NearObjectService(NearObjectServiceInjector&& injector);
    virtual ~NearObjectService() = default;

private:
    std::unique_ptr<NearObjectConnectionProfileManager> m_connectionProfileManager;
    std::vector<std::unique_ptr<NearObjectDeviceManager>> m_deviceManagers{};
};
} // namespace service
} // namespace nearobject

#endif //__NEAR_OBJECT_SERVICE_HXX__