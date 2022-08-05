
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
    Value to_serial() override;
};
} // namespace nearobject

Value
NearObjectConnectionProfileSecurity::to_serial(Document::AllocatorType& allocator) {
    Value v(kObject);
    return v;
}

#endif // NEAR_OBJECT_CONNECTION_PROFILE_SECURITY_HXX
