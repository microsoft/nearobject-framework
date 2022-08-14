
#ifndef NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM
#define NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM

#include <filesystem>
#include <vector>

#include <jsonify.hxx>
#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/service/NearObjectProfilePersister.hxx>

namespace nearobject
{
namespace service
{
namespace persistence
{
struct NearObjectProfilePersisterFilesystem
    : public NearObjectProfilePersister
{
    NearObjectProfilePersisterFilesystem();
    NearObjectProfilePersisterFilesystem(std::filesystem::path persistLocation);

    persist::PersistResult
    PersistProfile(const nearobject::NearObjectProfile& profile) override;

    std::vector<nearobject::NearObjectProfile>
    ReadPersistedProfiles(persist::PersistResult& persistResult) override;

private:
    const std::filesystem::path m_persistLocation;
};
} // namespace persistence
} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM


