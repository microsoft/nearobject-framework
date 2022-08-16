
#ifndef NEAR_OBJECT_PROFILE_HXX
#define NEAR_OBJECT_PROFILE_HXX

#include <optional>
#include <string>

#include <jsonify.hxx>
#include <nlohmann/json.hpp>

#include <nearobject/NearObjectProfileSecurity.hxx>

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
 * @brief A collection of configuration that specifies how to connect to a near
 * object peer or set of peers.
 */
struct NearObjectProfile
{
    /**
     * @brief Construct a new Near Object Profile object with default scope and
     * no security configuration.
     */
    NearObjectProfile() = default;

    /**
     * @brief Construct a new Near Object Profile object with explicit scope.
     * 
     * @param scope The scope to create the profile with.
     */
    NearObjectProfile(NearObjectConnectionScope scope);

    /**
     * @brief Construct a new Near Object Profile object with explicit scope and security settings.
     * 
     * @param scope The scope to create the profile with.
     * @param security The security settings to create the profile with.
     */
    NearObjectProfile(NearObjectConnectionScope scope, NearObjectProfileSecurity security);

    /**
     * @brief The supported connection scope.
     *
     * Note that this designates support and not an absolute requirement. Thus,
     * when 'Multicast' is specified, Unicast connections are still permitted.
     * 
     * @return NearObjectConnectionScope 
     */
    NearObjectConnectionScope
    GetScope() const noexcept;

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
     * 
     * @return std::optional<NearObjectProfileSecurity> 
     */
    const std::optional<NearObjectProfileSecurity>
    GetSecurity() const noexcept;

    /**
     * @brief Returns a string representation of this profile.
     * 
     * @return std::string 
     */
    std::string
    ToString() const noexcept;

    /**
    * @brief checks if the two profiles are the same
    */
    bool
    IsSame(const NearObjectProfile& other) const noexcept;

private:
    NearObjectConnectionScope m_scope{ NearObjectConnectionScope::Unicast };
    std::optional<NearObjectProfileSecurity> m_security{ std::nullopt };

    /**
     * @brief Declare friendship with nlohmann::json ADL serialization functions
     * in order to access private m_security member. 
     */
    friend void
    to_json(nlohmann::json& json, const NearObjectProfile& profile);

    friend void
    from_json(const nlohmann::json& json, NearObjectProfile& profile);
};

bool
operator==(const NearObjectProfile&, const NearObjectProfile&) noexcept;

bool
operator!=(const NearObjectProfile&, const NearObjectProfile&) noexcept;

} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_HXX
