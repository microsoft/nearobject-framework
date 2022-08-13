
#ifndef NEAR_OBJECT_MANAGER_HXX
#define NEAR_OBJECT_MANAGER_HXX

#include <memory>
#include <vector>

#include <nearobject/NearObjectProfile.hxx>

namespace nearobject
{
namespace service
{
struct NearObjectService;
} // namespace service

/**
 * @brief Unified interface for interacting with near objects.
 *
 * TODO: If this facade really just wraps the NearObjectService class, is it
 * really necessary?
 */
class NearObjectManager
{
public:
    struct NearObjectCreateSessionResult
    {
    };

    /**
     * @brief Attempt to establish a near object session using the specified profile.
     *
     * @param connectionProfile
     * @return NearObjectCreateSessionResult
     */
    NearObjectCreateSessionResult
    CreateSession(const NearObjectProfile& connectionProfile);

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

#endif // NEAR_OBJECT_MANAGER_HXX
