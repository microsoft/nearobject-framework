
#ifndef NEAR_OBJECT_PROFILE_MANAGER_HXX
#define NEAR_OBJECT_PROFILE_MANAGER_HXX

#include <filesystem>
#include <memory>
#include <shared_mutex>
#include <vector>

#include <jsonify/jsonify.hxx>

#include <nearobject/NearObjectProfile.hxx>
#include <nearobject/persist/NearObjectProfilePersister.hxx>

namespace nearobject::service
{
class NearObjectProfileManager
{
public:
    /**
     * @brief Construct a new Near Object Profile Manager object. A default
     * persister will be used.
     */
    NearObjectProfileManager();

    /**
     * @brief Construct a new Near Object Profile Manager object with a specific
     * profile persister.
     * 
     * @param persister The object to use to persist profiles.
     */
    NearObjectProfileManager(std::unique_ptr<persistence::NearObjectProfilePersister> persister);

    /**
     * @brief Describes the lifetime of the profile.
     */
    enum ProfileLifetime {
        Persistent,
        Ephemeral
    };

    /**
     * @brief Adds a new profile to support.
     *
     * @param profile The profile to add.
     * @param lifetime The lifetime of the profile.
     */
    void
    AddProfile(const NearObjectProfile& profile, ProfileLifetime lifetime = ProfileLifetime::Persistent);

    /**
     * @brief Find all profiles which match the provided profile.
     *
     * TODO: Matching by another profile might not make sense. Consider what
     * input argument(s) are more sensible, and update this prototype with them.
     * This will likely come from an out-of-band connection, and may identify
     * this device in some way.
     *
     * @param profile
     * @return std::vector<NearObjectProfile>
     */
    std::vector<NearObjectProfile>
    FindMatchingProfiles(const NearObjectProfile& profile) const;

    /**
     * @brief Return all known profiles.
     *
     * @return std::vector<NearObjectProfile>
     */
    std::vector<NearObjectProfile>
    GetAllProfiles() const;

protected:
    /**
     * @brief Persist the profile.
     *
     * @param profile The profile to persist.
     * @return the PersistResult
     */
    persist::PersistResult
    PersistProfile(const NearObjectProfile& profile);

    /**
     * @brief Obtain all persisted profiles.
     *
     * @param persistResult the return code
     * @return std::vector<NearObjectProfile>
     */
    std::vector<NearObjectProfile>
    ReadPersistedProfiles(persist::PersistResult& persistResult) const;

private:
    mutable std::shared_mutex m_profilesGate{};
    std::vector<NearObjectProfile> m_profiles{};
    const std::unique_ptr<persistence::NearObjectProfilePersister> m_persister;
};

} // namespace nearobject::service

#endif // NEAR_OBJECT_PROFILE_MANAGER_HXX
