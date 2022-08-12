
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

    /**
     * @brief checks if the two profiles are the same
     */
    bool
    IsSame(const NearObjectConnectionProfileSecurity& other) const noexcept;

    rapidjson::Value
    to_json(rapidjson::Document::AllocatorType&) const override;

    persist::ParseResult
    ParseAndSet(const rapidjson::Value&) override;
};

bool
operator==(const NearObjectConnectionProfileSecurity&, const NearObjectConnectionProfileSecurity&) noexcept;
bool
operator!=(const NearObjectConnectionProfileSecurity&, const NearObjectConnectionProfileSecurity&) noexcept;
} // namespace nearobject

#endif // NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
