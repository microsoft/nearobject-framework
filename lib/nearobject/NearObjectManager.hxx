
#ifndef NEAR_OBJECT_MANAGER_HXX
#define NEAR_OBJECT_MANAGER_HXX

#include <memory>
#include <vector>

#include <nearobject/NearObjectProfile.hxx>

namespace nearobject
{
namespace service
{
class NearObjectService;
} // namespace service

/**
 * @brief Unified interface for interacting with near objects.
 */
class NearObjectManager
{
public:
    struct NearObjectConnectionResult
    {
    };

    /**
     * @brief Attempt to establish a near object connection using the specified profile.
     *
     * @param connectionProfile
     * @return NearObjectConnectionResult
     */
    NearObjectConnectionResult
    EstablishConnection(const NearObjectProfile& connectionProfile);

    /**
     * @brief Find and enumerate all known profiles.
     *
     * @return std::vector<NearObjectProfile>
     */
    std::vector<NearObjectProfile>
    FindAllConnectionProfiles() const;

private:
    std::shared_ptr<service::NearObjectService> m_service;
};
} // namespace nearobject

#endif //NEAR_OBJECT_MANAGER_HXX
