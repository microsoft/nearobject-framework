
#ifndef NEAR_OBJECT_PROFILE_HXX
#define NEAR_OBJECT_PROFILE_HXX

#include <optional>

#include "NearObjectProfileSecurity.hxx"
#include <shared/jsonify/jsonify.hxx>

namespace nearobject
{
/**
 * @brief The connection scope, whether a single peer (unicast) or
 * multiple peers (multicast) are supported.
 */
enum class NearObjectConnectionScope {
    Unicast,
    Multicast,
};

std::string
NearObjectConnectionScope_ToString(NearObjectConnectionScope s);
NearObjectConnectionScope
NearObjectConnectionScope_FromString(std::string s);

/**
 * @brief A collection of configuration that specifies how to connect to a near
 * object peer or set of peers.
 */
struct NearObjectProfile : public persist::Serializable
{
    static bool
    profiles_match(const NearObjectProfile& p1, const NearObjectProfile& p2);

    /**
     * @brief The supported connection scope.
     *
     * Note that this designates support and not an absolute requirement. Thus,
     * when 'Multicast' is specified, Unicast connections are still permitted.
     */
    NearObjectConnectionScope Scope{ NearObjectConnectionScope::Unicast };

    /**
     * @brief The security configuration of the connection.
     *
     * Specifies whether security is required, and if so, the details of such
     * requirements. This includes secure device, secure channel, and key
     * requirements.
     *
     * If not specified, no security is required and the use of this profile
     * will ignore all security features of the NearObjectDevice and all peers
     * associated with it.
     */
    std::optional<NearObjectConnectionProfileSecurity> Security{ std::nullopt };

    rapidjson::Value
    to_serial(rapidjson::Document::AllocatorType&) const override;
    persist::ParseResult
    parse_and_set(const rapidjson::Value&) override;
};

} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_HXX
