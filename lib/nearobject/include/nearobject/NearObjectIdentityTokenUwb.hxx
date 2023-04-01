
#ifndef NEAR_OBJECT_IDENTITY_TOKEN_UWB_HXX
#define NEAR_OBJECT_IDENTITY_TOKEN_UWB_HXX

#include <cstddef>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include <nearobject/NearObjectIdentityToken.hxx>
#include <uwb/UwbMacAddress.hxx>

namespace nearobject
{
/**
 * @brief Class that adapts a uwb peer to a near object identifier.
 */
struct NearObjectIdentityTokenUwb :
    public NearObjectIdentityToken
{
    /**
     * @brief Deserialize a token into a NearObjectIdentityTokenUwb object.
     * 
     * @param token The token to deserialize.
     * @return NearObjectIdentityTokenUwb 
     */
    static NearObjectIdentityTokenUwb
    FromToken(std::span<const uint8_t> token);

    /**
     * @brief Serialize an object to a binary token.
     * 
     * @param identityTokenUwb The object to serialize.
     * @return std::span<const uint8_t> 
     */
    static std::vector<uint8_t>
    ToToken(const NearObjectIdentityTokenUwb& identityTokenUwb);

    /**
     * @brief Helper to deserialize into a std::unique_ptr<>
     * 
     * @param token The token to deserialize.
     * @return std::unique_ptr<NearObjectIdentityToken> 
     */
    static std::unique_ptr<NearObjectIdentityToken>
    UniqueFromToken(std::span<const uint8_t> token);

    /**
     * @brief Construct a new Near Object Identity Token Uwb object
     * 
     * @param uwbMacAddress The uwb address to initialize the token with.
     */
    explicit NearObjectIdentityTokenUwb(uwb::UwbMacAddress uwbMacAddress = {});

    /**
     * @brief Get the uwb mac address used to represent this token.
     * 
     * @return const uwb::UwbMacAddress 
     */
    const uwb::UwbMacAddress 
    GetMacAddress() const noexcept;

    /**
     * @brief Get the token value.
     * 
     * @return std::span<const std::byte> 
     */
    std::span<const uint8_t>
    GetToken() const noexcept override;

    /**
     * @brief 
     * 
     * @return std::string 
     */
    std::string
    ToString() const override;

protected:
    /**
     * @brief Override to include mac address for equality comparison.
     * 
     * @param other The other instance to compare for equality.
     * @return true 
     * @return false 
     */
    bool
    IsEqual(const NearObjectIdentityToken& other) const noexcept override;

private:
    uwb::UwbMacAddress m_uwbMacAddress{};
    std::vector<uint8_t> m_token{};
};

} // namespace nearobject

#endif // NEAR_OBJECT_IDENTITY_TOKEN_UWB_HXX
