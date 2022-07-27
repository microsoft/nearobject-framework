
#ifndef __NEAR_OBJECT_CONNECTION_PROFILE_HXX__
#define __NEAR_OBJECT_CONNECTION_PROFILE_HXX__

#include <optional>

#include "NearObjectConnectionProfileSecurity.hxx"

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

/**
 * @brief A collection of configuration that specifies how to connect to a near
 * object peer or set of peers.
 */
struct NearObjectConnectionProfile {
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
};

} // namespace nearobject

#endif // __NEAR_OBJECT_CONNECTION_PROFILE_HXX__
