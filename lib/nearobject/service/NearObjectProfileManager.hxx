
#ifndef NEAR_OBJECT_PROFILE_MANAGER_HXX
#define NEAR_OBJECT_PROFILE_MANAGER_HXX

#include <shared_mutex>
#include <vector>

#include <fstream>
#include <iostream>
#include <nearobject/NearObjectProfile.hxx>
#include <shared/jsonify/jsonify.hxx>

namespace nearobject
{
namespace service
{
class NearObjectProfileManager
{
public:
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
     * this device as a peer in some way.
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


    /**
    * @brief Sets the location to persist files to. 
    */
    void
    SetPersistLocation(std::string loc);

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
     * @param rcode the return code
     * @return std::vector<NearObjectProfile>
     */
    std::vector<NearObjectProfile>
    ReadPersistedProfiles(persist::PersistResult& rcode) const;

private:
    mutable std::shared_mutex m_profilesGate{};
    std::vector<NearObjectProfile> m_profiles{};
    std::string persist_location{ "profiles" };
};

} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_MANAGER_HXX
