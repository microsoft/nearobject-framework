
#ifndef __NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX__
#define __NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX__

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

#endif // __NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX__
