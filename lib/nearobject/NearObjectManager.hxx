
#ifndef __NEAR_OBJECT_MANAGER_HXX__
#define __NEAR_OBJECT_MANAGER_HXX__

#include <memory>
#include <vector>

#include <nearobject/NearObjectConnectionProfile.hxx>

namespace nearobject
{
namespace service
{
class NearObjectService;
} // namespace service

/**
 * @brief Central class managing the use of Near Object services. This class
 * exposes the primary API of the framework.
 *
 * This class ties together all of the different framework components.
 */
class NearObjectManager
{
public:
    NearObjectManager();

    struct NearObjectConnectionResult {
    };

    /**
     * @brief Attempt to establish a near object connection using the specified profile. 
     * 
     * @param connectionProfile 
     * @return NearObjectConnectionResult 
     */
    NearObjectConnectionResult
    EstablishConnection(const NearObjectConnectionProfile& connectionProfile);

    /**
     * @brief Find and enumerate all known connection profiles. 
     * 
     * @return std::vector<NearObjectConnectionProfile> 
     */
    std::vector<NearObjectConnectionProfile>
    FindAllConnectionProfiles() const;

private:
    std::shared_ptr<service::NearObjectService> m_service;
};
} // namespace nearobject

#endif //__NEAR_OBJECT_MANAGER_HXX__
