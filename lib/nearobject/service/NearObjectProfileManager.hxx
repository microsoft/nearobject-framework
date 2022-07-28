
#ifndef __NEAR_OBJECT_PROFILE_MANAGER_HXX__
#define __NEAR_OBJECT_PROFILE_MANAGER_HXX__

#include <vector>

#include <nearobject/NearObjectProfile.hxx>

namespace nearobject
{
namespace service
{
class NearObjectProfileManager
{
public:
    std::vector<NearObjectProfile>
    FindMatchingProfiles(const NearObjectProfile& connectionProfile) const;
};

} // namespace service
} // namespace nearobject

#endif // __NEAR_OBJECT_PROFILE_MANAGER_HXX__
