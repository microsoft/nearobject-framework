
#ifndef __NEAR_OBJECT_SERVICE_HXX__
#define __NEAR_OBJECT_SERVICE_HXX__

#include <memory>

#include "NearObjectServiceInjector.hxx"

namespace nearobject
{
namespace service
{
class NearObjectDeviceManager;
class NearObjectProfileManager;

/**
 * @brief Central service object managing all other framework components and
 * their lifetime.
 */
struct NearObjectService :
    public std::enable_shared_from_this<NearObjectService> {
    /**
     * @brief Safely create an instance of the service.
     *
     * This is the only function where an instance may be created to ensure that
     * the control block for std::shared_ptr is always created.
     *
     * @param injector
     * @return std::shared_ptr<NearObjectService>
     */
    [[nodiscard]] static std::shared_ptr<NearObjectService>
    Create(NearObjectServiceInjector&& injector);

    /**
     * @brief Get an instance of the service.
     *
     * @return std::shared_ptr<NearObjectService>
     */
    std::shared_ptr<NearObjectService>
    GetInstance();

    /**
     * @brief Profile manager.
     */
    std::shared_ptr<NearObjectProfileManager> ProfileManager;

    /**
     * @brief Device manager.
     */
    std::shared_ptr<NearObjectDeviceManager> DeviceManager;

    /**
     * @brief Destroy the Near Object Service object
     */
    virtual ~NearObjectService() = default;

protected:
    /**
     * @brief Construct a new NearObjectService object.
     *
     * This must be hidden to prevent instances from being created without the
     * std::shared_ptr control block. Without this, shared_from_this() would
     * produce an invalid/corrupt value.
     *
     * @param injector The dependency injector for the service.
     */
    explicit NearObjectService(NearObjectServiceInjector&& injector);
};
} // namespace service
} // namespace nearobject

#endif //__NEAR_OBJECT_SERVICE_HXX__
