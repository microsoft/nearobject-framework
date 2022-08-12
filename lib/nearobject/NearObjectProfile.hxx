
#ifndef NEAR_OBJECT_PROFILE_HXX
#define NEAR_OBJECT_PROFILE_HXX

#include <optional>
#include <shared/jsonify/jsonify.hxx>
#include <string>

#include "NearObjectProfileSecurity.hxx"

namespace nearobject
{
/**
 * @brief The connection scope, whether a single peer (unicast) or
 * multiple peers (multicast) are supported.
 */
enum class NearObjectConnectionScope {
    Unicast,
    Multicast,
    Unknown
};

/**
 * @brief Turns the enum NearObjectConnectionScope into the right string
 * 
 * @param scope The scope to convert to string
 * @return auto 
 */
auto
NearObjectConnectionScope_ToString(NearObjectConnectionScope scope);

/**
 * @brief Turns a string into the corresponding enum
 *
 * @param scope the string 
 * @return NearObjectConnectionScope The matching enum
 */
NearObjectConnectionScope
NearObjectConnectionScope_FromString(const std::string& scope);

/**
 * @brief A collection of configuration that specifies how to connect to a near
 * object peer or set of peers.
 */
struct NearObjectProfile :
    public persist::Serializable
{
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

    /**
    * @brief checks if the two profiles are the same
    */
    bool
    IsSame(const NearObjectProfile& other) const noexcept;

    /**
     * @brief 
     * 
     * @return rapidjson::Value 
     */
    rapidjson::Value
    ToJson(rapidjson::Document::AllocatorType&) const override;

    /**
     * @brief 
     * 
     * @return persist::ParseResult 
     */
    persist::ParseResult
    ParseAndSet(const rapidjson::Value&) override;
};

bool
operator==(const NearObjectProfile&, const NearObjectProfile&) noexcept;

bool
operator!=(const NearObjectProfile&, const NearObjectProfile&) noexcept;

} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_HXX
