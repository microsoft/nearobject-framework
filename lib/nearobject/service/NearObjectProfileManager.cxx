
#include <fstream>
#include <sstream>

#include <nearobject/service/NearObjectProfileManager.hxx>
#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>

using namespace nearobject;
using namespace nearobject::persistence;
using namespace nearobject::service;

NearObjectProfileManager::NearObjectProfileManager() :
    NearObjectProfileManager(std::make_unique<NearObjectProfilePersisterFilesystem>())
{}

NearObjectProfileManager::NearObjectProfileManager(std::unique_ptr<NearObjectProfilePersister> persister) :
    m_persister(std::move(persister))
{}

std::vector<NearObjectProfile>
NearObjectProfileManager::FindMatchingProfiles(const NearObjectProfile& connectionProfile) const
{
    const std::shared_lock profilesLockShared(m_profilesGate);
    return {};
}

std::vector<NearObjectProfile>
NearObjectProfileManager::GetAllProfiles() const
{
    const std::shared_lock profilesLockShared(m_profilesGate);
    return m_profiles;
}

void
NearObjectProfileManager::AddProfile(const NearObjectProfile& profile, ProfileLifetime lifetime)
{
    {
        const std::unique_lock profilesLockExclusive(m_profilesGate);
        m_profiles.push_back(profile);
    }

    if (lifetime == ProfileLifetime::Persistent) {
        PersistProfile(profile);
    }
}

persist::PersistResult
NearObjectProfileManager::PersistProfile(const NearObjectProfile& profile)
{
    return persist::PersistResult::Failed;
}

void
NearObjectProfileManager::SetPersistLocation(std::filesystem::path persistLocation)
{
    m_persistLocation = std::move(persistLocation);
}

std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles(persist::PersistResult& parseResult) const
{
    return {};
}
