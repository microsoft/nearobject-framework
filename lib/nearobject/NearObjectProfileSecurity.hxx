
#ifndef NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
#define NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX

namespace nearobject
{
/**
 * @brief A collection of configuration that specifies what security is required
 * when communicating with a near object peer or set of peers.
 */
struct NearObjectConnectionProfileSecurity
{
    virtual ~NearObjectConnectionProfileSecurity() = default;
};
} // namespace nearobject

#endif // NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
