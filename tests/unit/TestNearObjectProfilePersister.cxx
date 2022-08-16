
#include <array>
#include <chrono>
#include <filesystem>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <vector>

#include <catch2/catch.hpp>
#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/persist/NearObjectProfilePersister.hxx>
#include <nearobject/persist/NearObjectProfilePersisterFilesystem.hxx>
#include <notstd/tostring.hxx>

using namespace strings::ostream_operators;

namespace nearobject
{
namespace persistence
{
namespace test
{
using persist::PersistResult;

/**
 * @brief RAII helper to delete the path associated with a
 * NearObjectProfilePersisterFilesystem object. The parent path is deleted when
 * this object goes out of scope.
 */
struct DeletePersisterPathOnScopeExit
{
    explicit DeletePersisterPathOnScopeExit(const NearObjectProfilePersisterFilesystem& persisterFs) :
        PathToDelete(persisterFs.GetPersistenceFilepath().parent_path())
    {}

    ~DeletePersisterPathOnScopeExit()
    {
        std::filesystem::remove_all(PathToDelete);
    }

    std::filesystem::path PathToDelete;
};

/**
 * @brief Persists a profile using a generic persister, validates the interface
 * returned the expected result, then reads back the persisted profile and
 * verifies it matches the one in memory that was persisted.
 * 
 * @param persister The persister to use.
 * @param profileToPersist The profile to persist.
 * @param expectedSize The number of profiles expected to be read back.
 */
void
PersistProfileAndValidate(NearObjectProfilePersister& persister, const NearObjectProfile& profileToPersist)
{
    // Read current vector of profiles.
    PersistResult persistResult = PersistResult::UnknownError;
    const auto profilesPersistedBefore = persister.ReadPersistedProfiles(persistResult);
    REQUIRE(persistResult == PersistResult::Succeeded);

    // Persist new profile.
    persistResult = persister.PersistProfile(profileToPersist);
    REQUIRE(persistResult == PersistResult::Succeeded);

    // Read back updated profiles.
    persistResult = PersistResult::UnknownError;
    const auto profilesPersistedAfter = persister.ReadPersistedProfiles(persistResult);
    REQUIRE(persistResult == PersistResult::Succeeded);
    REQUIRE(profilesPersistedAfter.size() == profilesPersistedBefore.size() + 1);
    const auto& profileToCompare = profilesPersistedAfter.back();
    REQUIRE(profileToCompare == profileToPersist);
}

/**
 * @brief Read profiles from persistence store and validate they match the ones
 * provided. Note that the profilesExpected vector must be in the same order
 * that the persister will provide them.
 * 
 * @param persister The persister to read profiles from.
 * @param profilesExpected The expected profiles to be read from the persister.
 */
void
ValidateProfilesOnDisk(NearObjectProfilePersister& persister, const std::vector<NearObjectProfile>& profilesExpected)
{
    PersistResult persistResult = PersistResult::UnknownError;
    const auto profilesActual = persister.ReadPersistedProfiles(persistResult);
    REQUIRE(persistResult == PersistResult::Succeeded);
    REQUIRE(profilesExpected == profilesActual);
}

/**
 * @brief Get the test directory path suffix.
 * 
 * @return const std::filesystem::path& 
 */
const std::filesystem::path&
GetTestPersistenceDirectorySuffix()
{
    try {
        static const std::filesystem::path suffix = "Test/TestNearObjectProfilePersister";
        return suffix;
    } catch (const std::filesystem::filesystem_error& /* filesystemError */) {
        throw std::runtime_error("failed to create test path suffix");
    }
}

/**
 * @brief Generate a utc timestamp string in the iso8601('ish) format. 
 * 
 * One such example is '20220815T223819+0000Z'.
 * 
 * @return std::string 
 */
std::string
GetIso8601Timestamp()
{
    const auto nowUtc = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    const auto timestampAsTimeT = std::chrono::system_clock::to_time_t(nowUtc);
    const auto timestampAsTm = *(std::gmtime(&timestampAsTimeT));

    std::ostringstream timestampBuilder{};
    timestampBuilder << std::put_time(&timestampAsTm, "%Y%m%dT%H%M%S%zZ");

    return timestampBuilder.str();
}

/**
 * @brief Generates a pseudo-unique path that uses the system temporary
 * directory, a common base directory, a timestamp, and a common test directory
 * suffix. 
 * 
 * One such example on a Linux system is:
 *  /tmp/NearObject/20220815T223724+0000Z/Test/TestNearObjectProfilePersister
 * 
 * @return std::filesystem::path 
 */
std::filesystem::path
GenerateUniqueTestTempPath()
{
    return std::filesystem::temp_directory_path() / "NearObject" / GetIso8601Timestamp() / GetTestPersistenceDirectorySuffix();
}
} // namespace test
} // namespace persistence
} // namespace nearobject

TEST_CASE("near object filesystem profile persister can be created", "[basic][persist]")
{
    using namespace nearobject::persistence;

    SECTION("creation doesn't cause a crash")
    {
        NearObjectProfilePersisterFilesystem persisterFs{};
    }

    SECTION("creation with custom path doesn't cause a crash")
    {
        NearObjectProfilePersisterFilesystem persisterFs{ test::GenerateUniqueTestTempPath() };
    }

    SECTION("creation with invalid path is disallowed")
    {
        REQUIRE_THROWS_AS(NearObjectProfilePersisterFilesystem{ "\0" }, std::filesystem::filesystem_error);
    }
}

TEST_CASE("near object filesystem persister uses custom persistence path", "[basic][persist]")
{
    using namespace nearobject::persistence;

    SECTION("custom persistence path is stored")
    {
        const std::filesystem::path persistencePath = test::GenerateUniqueTestTempPath();
        NearObjectProfilePersisterFilesystem persisterFs{ persistencePath };

        const auto persistenceFile = persisterFs.GetPersistenceFilepath();
        REQUIRE(persistenceFile.parent_path() == persistencePath);
        REQUIRE(std::filesystem::exists(persistenceFile.parent_path()));
    }

    SECTION("persisted files are actually stored in specified persistence path")
    {
        using namespace nearobject;

        const std::filesystem::path persistencePath = test::GenerateUniqueTestTempPath();
        NearObjectProfilePersisterFilesystem persisterFs{ persistencePath };
        const auto persistenceFile = persisterFs.GetPersistenceFilepath();

        persisterFs.PersistProfile(NearObjectProfile{});
        REQUIRE(persistenceFile.parent_path() == persistencePath);
        REQUIRE(std::filesystem::exists(persistenceFile));
        REQUIRE(std::filesystem::is_regular_file(persistenceFile));
        REQUIRE(std::filesystem::file_size(persistenceFile) > 0);
    }
}

TEST_CASE("near object filesystem persister cannot be modified by non-framework users", "[basic][persist][security]")
{
    SECTION("persistence file cannot be written by non-framework user accounts")
    {
        // TODO
    }

    SECTION("persistence file cannot be deleted by non-framework user accounts")
    {
        // TODO
    }

    SECTION("persistence file cannot be overwrriten/truncated by non-frametwork user accounts")
    {
        // TODO
    }
}

TEST_CASE("near object filesystem persister persists profiles", "[basic][persist]")
{
    using namespace nearobject;
    using namespace nearobject::persistence;

    constexpr std::array<NearObjectConnectionScope, 3> AllScopes = {
        NearObjectConnectionScope::Unicast,
        NearObjectConnectionScope::Multicast,
        NearObjectConnectionScope::Unknown,
    };

    std::vector<NearObjectProfile> ProfilesWithVariedScopes{};
    std::transform(std::cbegin(AllScopes), std::cend(AllScopes), std::back_inserter(ProfilesWithVariedScopes), [&](const auto& scope) {
        return NearObjectProfile{ scope };
    });

    std::vector<NearObjectProfile> ProfilesWithVariedScopesAndSecurity{};
    std::transform(std::cbegin(AllScopes), std::cend(AllScopes), std::back_inserter(ProfilesWithVariedScopesAndSecurity), [&](const auto& scope) {
        return NearObjectProfile{ scope, NearObjectProfileSecurity{} };
    });

    SECTION("single profile can be persisted")
    {
        // Persist default-constructed profile.
        {
            NearObjectProfile profile{};
            NearObjectProfilePersisterFilesystem persisterFs{ test::GenerateUniqueTestTempPath() };
            test::DeletePersisterPathOnScopeExit persisterPathDeleter{ persisterFs };
            test::PersistProfileAndValidate(persisterFs, profile);
        }

        // Persist profiles with all possible scopes.
        for (const auto& profile : ProfilesWithVariedScopes) {
            NearObjectProfilePersisterFilesystem persisterFs{ test::GenerateUniqueTestTempPath() };
            test::DeletePersisterPathOnScopeExit persisterPathDeleter{ persisterFs };
            test::PersistProfileAndValidate(persisterFs, profile);
        }
        
        // Persist profiles with instantiated security settings.
        for (const auto& profileWithSecurity : ProfilesWithVariedScopesAndSecurity) {
            NearObjectProfilePersisterFilesystem persisterFs{ test::GenerateUniqueTestTempPath() };
            test::DeletePersisterPathOnScopeExit persisterPathDeleter{ persisterFs };
            test::PersistProfileAndValidate(persisterFs, profileWithSecurity);
        }
    }

    SECTION("multiple profiles can be persisted")
    {
        NearObjectProfilePersisterFilesystem persisterFs{ test::GenerateUniqueTestTempPath() };
        test::DeletePersisterPathOnScopeExit persisterPathDeleter{ persisterFs };

        // Persist profiles with all possible scopes.
        for (const auto& profile : ProfilesWithVariedScopes) {
            test::PersistProfileAndValidate(persisterFs, profile);
        }

        // Persist profiles with instantiated security settings.
        for (const auto& profileWithSecurity : ProfilesWithVariedScopesAndSecurity) {
            test::PersistProfileAndValidate(persisterFs, profileWithSecurity);
        }
    }

    SECTION("persister state is not required to persist profiles")
    {
        const std::filesystem::path persistLocation = test::GenerateUniqueTestTempPath();

        for (const auto& profile : ProfilesWithVariedScopes) {
            // Use a distinct persister instance for each operation.
            NearObjectProfilePersisterFilesystem persisterFs{ persistLocation };
            test::PersistProfileAndValidate(persisterFs, profile);
        }

        // Read the profiles with a different persister instance than was used to persist the profiles.
        NearObjectProfilePersisterFilesystem persisterFsForValidation{ persistLocation };
        test::DeletePersisterPathOnScopeExit persisterPathDeleter{ persisterFsForValidation };
        test::ValidateProfilesOnDisk(persisterFsForValidation, ProfilesWithVariedScopes);
    }

    SECTION("existing profiles are retained upon persisting new profiles")
    {
        // TODO
    }
}
