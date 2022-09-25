
#ifndef NEAR_OBJECT_IDENTITY_TOKEN_HXX
#define NEAR_OBJECT_IDENTITY_TOKEN_HXX

#include <cstddef>
#include <span>

namespace nearobject
{
/**
 * @brief Represents an object which provides a near object identity token.
 * 
 * An identity token uniquely identifies a near object for the purpose of
 * participating in a near object session. It is an opaque, serializable value.
 * This unstructured format allows flexibility in how a particular scenario or
 * underlying device might identify a near object.
 * 
 * Note that the implementation must be careful to manage the lifetime of the
 * data appropriately, as this interface only exposes a view of the token, not
 * the token data itself.
 */
struct NearObjectIdentityToken
{
    /**
     * @brief Destroy the NearObjectIdentityToken object.
     */
    virtual ~NearObjectIdentityToken() = default;

    /**
     * @brief Get the token value.
     * 
     * @return std::span<const std::byte> 
     */
    virtual std::span<const std::byte>
    GetToken() const noexcept = 0;

protected:
    /**
     * @brief Construct a new NearObjectIdentityToken object.
     * 
     * Restricted to derived implementations.
     */
    NearObjectIdentityToken() = default;
};

} // namespace nearobject

#endif // NEAR_OBJECT_IDENTITY_TOKEN_HXX
