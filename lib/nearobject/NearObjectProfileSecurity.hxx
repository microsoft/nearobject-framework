
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
    static bool
    profiles_match(const NearObjectConnectionProfileSecurity& p1, const NearObjectConnectionProfileSecurity& p2);

    virtual ~NearObjectConnectionProfileSecurity() = default;

    rapidjson::Value
    to_json(rapidjson::Document::AllocatorType&) const override;
    
    persist::ParseResult
    parse_and_set(const rapidjson::Value&) override;
};
} // namespace nearobject

#endif // NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
