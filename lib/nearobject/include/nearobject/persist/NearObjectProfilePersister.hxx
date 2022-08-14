
#ifndef NEAR_OBJECT_PROFILE_PERSISTER
#define NEAR_OBJECT_PROFILE_PERSISTER

#include <vector>

#include <jsonify.hxx>
#include <nearobject/NearObjectProfile.hxx>

namespace nearobject
{
namespace persistence
{
struct NearObjectProfilePersister
{
    virtual ~NearObjectProfilePersister() = default;

    /**
     * @brief Persist the profile.
     *
     * @param profile The profile to persist.
     * @return the PersistResult
     */
    virtual persist::PersistResult
    PersistProfile(const nearobject::NearObjectProfile& profile) = 0;

    /**
     * @brief Obtain all persisted profiles.
     *
     * @param persistResult the return code
     * @return std::vector<NearObjectProfile>
     */
    virtual std::vector<nearobject::NearObjectProfile>
    ReadPersistedProfiles(persist::PersistResult& persistResult) = 0;
};
} // namespace persistence
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_PERSISTER
