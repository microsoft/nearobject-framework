
#ifndef NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM
#define NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM

#include <filesystem>
#include <vector>

#include <jsonify.hxx>
#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/persist/NearObjectProfilePersister.hxx>

namespace nearobject
{
namespace persistence
{
struct NearObjectProfilePersisterFilesystem
    : public NearObjectProfilePersister
{
    NearObjectProfilePersisterFilesystem();
    NearObjectProfilePersisterFilesystem(const std::filesystem::path& persistLocation);

    persist::PersistResult
    PersistProfile(const nearobject::NearObjectProfile& profile) override;

    std::vector<nearobject::NearObjectProfile>
    ReadPersistedProfiles(persist::PersistResult& persistResult) override;

    /**
     * @brief Get the path of the file where profiles are persisted.
     * 
     * @return std::filesystem::path 
     */
    std::filesystem::path
    GetPersistenceFilepath() const noexcept;

private:
    const std::filesystem::path m_persistFilepath;
};
} // namespace persistence
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM
