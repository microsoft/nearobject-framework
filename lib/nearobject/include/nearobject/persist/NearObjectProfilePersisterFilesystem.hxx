
#ifndef NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM
#define NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM

#include <filesystem>
#include <vector>

#include <jsonify/jsonify.hxx>
#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/persist/NearObjectProfilePersister.hxx>

namespace nearobject::persistence
{
/**
 * @brief Common persistence path suffix to use across platforms.
 */
const std::filesystem::path PathSuffix = ".nearobject";

/**
 * @brief Object to persist files to/from a local filesystem.
 * 
 * Note that this class is NOT thread-safe. The caller must ensure that access
 * to the file is synchronized to avoid corruption.
 */
struct NearObjectProfilePersisterFilesystem
    : public NearObjectProfilePersister
{
    /**
     * @brief Construct a new Near Object Profile Persister Filesystem object
     * with a default persistence location.
     */
    NearObjectProfilePersisterFilesystem();

    /**
     * @brief Construct a new Near Object Profile Persister Filesystem object
     * with a custom persistence location.
     * 
     * @param persistLocation The parent directory to create the persistence file.
     */
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
} // namespace nearobject::persistence

#endif // NEAR_OBJECT_PROFILE_PERSISTER_FILESYSTEM
