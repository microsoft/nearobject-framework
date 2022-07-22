
#ifndef __CONNECTION_PROFILE_MANAGER_HXX__
#define __CONNECTION_PROFILE_MANAGER_HXX__

#include <vector>

#include "ConnectionProfile.hxx"

namespace nearobject
{

class ConnectionProfileManager
{
public:
    std::vector<ConnectionProfile>
    FindMatchingProfiles(const ConnectionProfile& connectionProfile) const;
};

} // namespace nearobject

#endif // __CONNECTION_PROFILE_MANAGER_HXX__
