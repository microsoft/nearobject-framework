
#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>

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
    // TODO
    return persist::PersistResult::UnknownError;
}

std::vector<NearObjectProfile>
NearObjectProfilePersisterFilesystem::ReadPersistedProfiles(persist::PersistResult& persistResult)
{
    // TODO
    return {};
}
