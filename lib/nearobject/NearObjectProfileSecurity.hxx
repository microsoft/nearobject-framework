
#ifndef NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
#define NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX

#include <shared/jsonify/jsonify.hxx>

namespace nearobject
{
/**
 * @brief A collection of configuration that specifies what security is required
 * when communicating with a near object peer or set of peers.
 */
struct NearObjectConnectionProfileSecurity : public persist::Serializable
{
    virtual ~NearObjectConnectionProfileSecurity() = default;
    rapidjson::Value to_serial(rapidjson::Document::AllocatorType&) override;
};
} // namespace nearobject



#endif // NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
