
#include "NearObjectProfileManager.hxx"

using namespace nearobject;
using namespace nearobject::service;

std::vector<NearObjectProfile>
NearObjectProfileManager::FindMatchingProfiles(const NearObjectProfile& connectionProfile) const
{
    const std::shared_lock profilesLockShared{ m_profilesGate };
    return {};
}

std::vector<NearObjectProfile>
NearObjectProfileManager::GetAllProfiles() const
{
    const std::shared_lock profilesLockShared{ m_profilesGate };
    return m_profiles;
}

void
NearObjectProfileManager::AddProfile(const NearObjectProfile& profile, ProfileLifetime lifetime)
{
    const auto profileToAdd = profile;
    {
        const std::unique_lock profilesLockExclusive{ m_profilesGate };
        m_profiles.push_back(profileToAdd);
    }

    if (lifetime == ProfileLifetime::Persistent) {
        PersistProfile(profileToAdd);
    }
}

using namespace rapidjson;
using namespace std;

void
NearObjectProfileManager::PersistProfile(const NearObjectProfile& profile)
{
    // persistence is achieved via a file (what are the perms on the file?)
    // after you determine the file to persist to, read the entire file, 

    std::string location;
    // get json from the file at location
    

}

std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles() const
{
    // TODO
    return {};
}
