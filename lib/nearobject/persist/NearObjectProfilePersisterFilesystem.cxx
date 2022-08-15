
#include <algorithm>
#include <fstream>

#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>
#include <nearobject/persist/NearObjectProfileJsonSerializer.hxx>
#include <JsonSerializer.hxx>

using namespace nearobject;
using namespace nearobject::persistence;

NearObjectProfilePersisterFilesystem::NearObjectProfilePersisterFilesystem() :
    NearObjectProfilePersisterFilesystem(std::filesystem::temp_directory_path() / "NearObject/Profiles")
{}

NearObjectProfilePersisterFilesystem::NearObjectProfilePersisterFilesystem(std::filesystem::path persistLocation) :
    m_persistLocation(std::move(persistLocation))
{}

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
    auto json = nlohmann::json{ profiles };

    // Write updated list to disk.
    std::ofstream profilesFile{ m_persistLocation };
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
    if (!std::filesystem::exists(m_persistLocation)) {
        return {};
    }

    std::ifstream profilesFile{ m_persistLocation };
    if (profilesFile.fail()) {
        persistResult = persist::PersistResult::FailedToOpenFile;
        return {};
    }

    const auto json = nlohmann::json::parse(profilesFile);
    auto profiles = json.get<std::vector<NearObjectProfile>>();

    return profiles;
}
