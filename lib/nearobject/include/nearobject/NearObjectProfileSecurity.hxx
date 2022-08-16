
#ifndef NEAR_OBJECT_PROFILE_SECURITY_HXX
#define NEAR_OBJECT_PROFILE_SECURITY_HXX

#include <string>

#include <jsonify.hxx>

namespace nearobject
{
/**
 * @brief A collection of configuration that specifies what security is required
 * when communicating with a near object peer or set of peers.
 */
struct NearObjectProfileSecurity
{
    virtual ~NearObjectProfileSecurity() = default;
    NearObjectProfileSecurity() = default;
    NearObjectProfileSecurity(const NearObjectProfileSecurity&) = default;
    NearObjectProfileSecurity(NearObjectProfileSecurity&&) = default;
    NearObjectProfileSecurity& operator=(const NearObjectProfileSecurity&) = default;
    NearObjectProfileSecurity& operator=(NearObjectProfileSecurity&&) = default;

    /**
     * @brief Returns a string representation of this security profile.
     * 
     * @return std::string 
     */
    std::string
    ToString() const noexcept;

    /**
     * @brief checks if the two profiles are the same
     */
    bool
    IsSame(const NearObjectProfileSecurity& other) const noexcept;
};

bool
operator==(const NearObjectProfileSecurity&, const NearObjectProfileSecurity&) noexcept;

bool
operator!=(const NearObjectProfileSecurity&, const NearObjectProfileSecurity&) noexcept;
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_SECURITY_HXX
