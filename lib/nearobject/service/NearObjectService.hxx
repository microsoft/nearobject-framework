
#ifndef __NEAR_OBJECT_SERVICE_HXX__
#define __NEAR_OBJECT_SERVICE_HXX__

#include <memory>
#include <vector>

namespace nearobject
{
namespace service
{
class NearObjectDeviceManager;

/**
 * @brief Central service object managing all other framework components and
 * lifetimes.
 * 
 * TODO: consider taking in all service dependencies (or a reasonably operative
 * set) at creation time through the constructor. We may not care to add or
 * change these dependencies at runtime.
 */
class NearObjectService
{
public:
    void
    AddDeviceManager(std::unique_ptr<NearObjectDeviceManager> nearObjectDeviceManager);

private:
    std::vector<std::unique_ptr<NearObjectDeviceManager>> m_nearObjectDeviceManagers{};
};
} // namespace service
} // namespace nearobject

#endif //__NEAR_OBJECT_SERVICE_HXX__