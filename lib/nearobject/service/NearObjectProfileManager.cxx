
#include <algorithm>
#include <fstream>
#include <mutex>
#include <sstream>
#include <stdexcept>

#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>
#include <nearobject/service/NearObjectProfileManager.hxx>

using namespace nearobject;
using namespace nearobject::persistence;
using namespace nearobject::service;

NearObjectProfileManager::NearObjectProfileManager() :
    NearObjectProfileManager(std::make_unique<NearObjectProfilePersisterFilesystem>())
{}

NearObjectProfileManager::NearObjectProfileManager(std::unique_ptr<NearObjectProfilePersister> persister) :
    m_persister(std::move(persister))
{
    if (m_persister == nullptr) {
        throw std::runtime_error("NearObjectProfilePersister must not be null");
    }
}

std::vector<NearObjectProfile>
NearObjectProfileManager::FindMatchingProfiles(const NearObjectProfile& profileToMatch) const
{
    std::vector<NearObjectProfile> profilesMatching{};
    const std::shared_lock profilesLockShared(m_profilesGate);
    std::copy_if(std::cbegin(m_profiles), std::cend(m_profiles), std::back_inserter(profilesMatching), [&](const auto profileToCheck) {
        return (profileToCheck == profileToMatch);
    });

    return profilesMatching;
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
    return m_persister->PersistProfile(profile);
}

std::vector<NearObjectProfile>
NearObjectProfileManager::ReadPersistedProfiles(persist::PersistResult& parseResult) const
{
    return m_persister->ReadPersistedProfiles(parseResult);
}
