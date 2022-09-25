
#ifndef UWB_DEVICE_ADDRESS_HXX
#define UWB_DEVICE_ADDRESS_HXX

#include <array>
#include <cstdint>
#include <span>
#include <variant>

#include <type_traits>

namespace uwb
{
/**
 * @brief The type of uwb device object address.
 */
enum class UwbMacAddressType {
    Short,
    Extended,
};

/**
 * @brief Helper class defining near object address length constants.
 */
struct UwbMacAddressLength final
{
    /**
     * @brief Prevent instantiation since this is for constants only.
     */
    UwbMacAddressLength() = delete;

    /**
     * @brief The length of a short-type address.
     */
    static constexpr auto Short = 2;

    /**
     * @brief The length of an extended-type address.
     */
    static constexpr auto Extended = 8;
};

namespace detail
{
using enum UwbMacAddressType;

/**
 * @brief Concept to encode valid address properties.
 * 
 * @tparam AddressType The type of address.
 * @tparam AddressLength The length of the address, in bytes.
 */
template <UwbMacAddressType AddressType, std::size_t AddressLength>
concept ValidAddressProperties = 
    (AddressType == Short    && AddressLength == UwbMacAddressLength::Short) ||
    (AddressType == Extended && AddressLength == UwbMacAddressLength::Extended);
}

/**
 * @brief Represents the address of a near object.
 */
class UwbMacAddress 
{
public:
    /**
     * @brief The address length, in bytes, when Type == Short.
     */
    static constexpr auto ShortLength = UwbMacAddressLength::Short;

    /**
     * @brief The address length, in bytes, when Type == Extended.
     */
    static constexpr auto ExtendedLength = UwbMacAddressLength::Extended;

    /**
     * @brief C++ types for each address type.
     */
    using ShortType = std::array<uint8_t, ShortLength>;
    using ExtendedType = std::array<uint8_t, ExtendedLength>;

    /**
     * @brief The address type.
     */
    const UwbMacAddressType Type;

    /**
     * @brief View of the underlying value.
     */
    std::span<uint8_t> Value;

    /**
     * @brief The length of the address, in bytes.
     */
    const std::size_t Length;

    /**
     * @brief Construct a default UwbMacAddress.
     */
    UwbMacAddress();

    /**
     * @brief Creates a short UwbMacAddress.
     * 
     * @param address The address to initialize.
     */
    explicit UwbMacAddress(ShortType address);

    /**
     * @brief Creates an extended UwbMacAddress.
     * 
     * @param address The address to initialize.
     */
    explicit UwbMacAddress(ExtendedType address);

private:
    /**
     * @brief Initializes the value span based on the underlying type.
     * 
     * @tparam ActiveType 
     */
    template <typename ActiveType> 
        requires (std::is_same_v<ActiveType, UwbMacAddress::ShortType> || std::is_same_v<ActiveType, UwbMacAddress::ExtendedType>)
    void 
    InitializeValue();

    /**
     * @brief Syntactic sugar for directly retrieving the short address.
     * 
     * This will throw std::bad_variant_access if called when the type is
     * actually an extended address.
     * 
     * @return std::array<uint8_t, ShortLength> 
     */
    std::array<uint8_t, ShortLength>
    GetShort() const;

    /**
     * @brief Syntactic sugar for directly retrieving the short address.
     * 
     * This will throw std::bad_variant_access if called when the type is
     * actually an short.
     * 
     * @return std::array<uint8_t, ExtendedLength> 
     */
    std::array<uint8_t, ExtendedLength>
    GetExtended() const;

    /**
     * @brief Allow global equality function to access private members.
     * 
     * @return true 
     * @return false 
     */
    friend bool
    operator==(const UwbMacAddress&, const UwbMacAddress&) noexcept;

private:
    /**
     * @brief The address value, which depends on the 'Type' field.
     * 
     * Despite the 'Type' field driving this value, the std::variant is the
     * ultimate ground-truth for the value, and the appropriate methods from
     * that class should be used to obtain it.
     */
    std::variant<ShortType, ExtendedType> m_value{ ShortType{ 0x00, 0x00 } };
};

bool
operator==(const UwbMacAddress&, const UwbMacAddress&) noexcept;

bool
operator!=(const UwbMacAddress&, const UwbMacAddress&) noexcept;

} // namespace uwb

#endif // UWB_DEVICE_ADDRESS_HXX