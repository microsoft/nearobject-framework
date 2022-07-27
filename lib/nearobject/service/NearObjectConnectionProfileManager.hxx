
#ifndef __CONNECTION_PROFILE_MANAGER_HXX__
#define __CONNECTION_PROFILE_MANAGER_HXX__

#include <vector>

#include <nearobject/NearObjectConnectionProfile.hxx>

namespace nearobject
{
namespace service
{
class NearObjectConnectionProfileManager
{
public:
    std::vector<NearObjectConnectionProfile>
    FindMatchingProfiles(const NearObjectConnectionProfile& connectionProfile) const;
};

} // namespace service
} // namespace nearobject

#endif // __CONNECTION_PROFILE_MANAGER_HXX__
