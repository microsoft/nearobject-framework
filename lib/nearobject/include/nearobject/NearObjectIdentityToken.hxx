
#ifndef NEAR_OBJECT_IDENTITY_TOKEN_HXX
#define NEAR_OBJECT_IDENTITY_TOKEN_HXX

#include <cstdint>
#include <span>
#include <string>

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
     * @return std::span<const uint8_t> 
     */
    virtual std::span<const uint8_t>
    GetToken() const noexcept = 0;

    /**
     * @brief Return a string representation of the token.
     * 
     * The default implementation outputs the token as hexadecimal.
     * 
     * @return std::string 
     */
    virtual std::string
    ToString() const;

protected:
    /**
     * @brief Default implementation that is called by operator==(). Ensures
     * that the generated tokens are the same.
     * 
     * Derived classes may override this is additional members should be
     * included in the comparison.
     * 
     * @param other The other instance to compare.
     * @return true 
     * @return false 
     */
    virtual bool
    IsEqual(const NearObjectIdentityToken& other) const noexcept;

    /**
     * @brief Construct a new NearObjectIdentityToken object.
     * 
     * Restricted to derived implementations.
     */
    NearObjectIdentityToken() = default;

    /**
     * @brief Allow global == to access private members. 
     * 
     * @param NearObjectIdentityToken 
     * @return true 
     * @return false 
     */
    friend
    bool operator==(const NearObjectIdentityToken&, const NearObjectIdentityToken&) noexcept;
};

bool
operator==(const NearObjectIdentityToken&, const NearObjectIdentityToken&) noexcept;

bool
operator!=(const NearObjectIdentityToken&, const NearObjectIdentityToken&) noexcept;

} // namespace nearobject

#endif // NEAR_OBJECT_IDENTITY_TOKEN_HXX
