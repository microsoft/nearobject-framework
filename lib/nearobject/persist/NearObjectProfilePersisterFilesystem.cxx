
#include <algorithm>
#include <fstream>

#include <nearobject/persist/NearObjectProfileJsonSerializer.hxx>
#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>

using namespace nearobject;
using namespace nearobject::persistence;

NearObjectProfilePersisterFilesystem::NearObjectProfilePersisterFilesystem() :
    NearObjectProfilePersisterFilesystem(std::filesystem::temp_directory_path())
{}

NearObjectProfilePersisterFilesystem::NearObjectProfilePersisterFilesystem(const std::filesystem::path& persistLocation) :
    m_persistFilepath(persistLocation / "Profiles")
{
    if (!std::filesystem::exists(m_persistFilepath.parent_path())) {
        bool persistLocationCreated = std::filesystem::create_directories(m_persistFilepath.parent_path());
        if (!persistLocationCreated) {
            // TODO: throw ..
        }
    }
}

std::filesystem::path
NearObjectProfilePersisterFilesystem::GetPersistenceFilepath() const noexcept
{
    return m_persistFilepath;
}

persist::PersistResult
NearObjectProfilePersisterFilesystem::PersistProfile(const NearObjectProfile& profile)
{
    // Read existing profiles from disk.
    persist::PersistResult persistResult = persist::PersistResult::UnknownError;
    auto profiles = ReadPersistedProfiles(persistResult);
    if (persistResult != persist::PersistResult::Succeeded) {
        return persistResult;
    }

    // Add new profile and serialize updated list.
    profiles.push_back(profile);
    auto json = nlohmann::json(profiles);

    // Write updated list to disk.
    std::ofstream profilesFile{ m_persistFilepath };
    profilesFile << json;
    profilesFile.close();

    persistResult = profilesFile.fail()
        ? persist::PersistResult::Failed
        : persist::PersistResult::Succeeded;

    return persistResult;
}

std::vector<NearObjectProfile>
NearObjectProfilePersisterFilesystem::ReadPersistedProfiles(persist::PersistResult& persistResult)
{
    if (!std::filesystem::exists(m_persistFilepath)) {
        persistResult = persist::PersistResult::Succeeded;
        return {};
    }

    std::ifstream profilesFile{ m_persistFilepath };
    if (profilesFile.fail()) {
        persistResult = persist::PersistResult::FailedToOpenFile;
        return {};
    }

    const auto json = nlohmann::json::parse(profilesFile);
    persistResult = persist::PersistResult::Succeeded;
    if (json.empty()) {
        return {};
    }

    auto profiles = json.get<std::vector<NearObjectProfile>>();
    return profiles;
}
