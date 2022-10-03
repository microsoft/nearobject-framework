
#ifndef UWB_DEVICE_ADDRESS_HXX
#define UWB_DEVICE_ADDRESS_HXX

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <variant>
#include <type_traits>

#include <notstd/hash.hxx>

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
    UwbMacAddressLength() = delete;
    static constexpr auto Short = 2;
    static constexpr auto Extended = 8;
};

/**
 * @brief Concept to encode valid address lengths.
 * 
 * @tparam UwbMacAddessLength
 */
template <size_t UwbMacAddessLength>
concept ValidUwbMacAddressLength = 
    (UwbMacAddessLength == UwbMacAddressLength::Short) ||
    (UwbMacAddessLength == UwbMacAddressLength::Extended);

namespace detail
{
    template <size_t Length>
    struct UwbMacAddressTypeImpl {};

    /**
     * @brief Address type lookup full specialization for short addresses. 
     * 
     * @tparam  
     */
    template <>
    struct UwbMacAddressTypeImpl<UwbMacAddressLength::Short> 
    {
        static constexpr UwbMacAddressType value = UwbMacAddressType::Short;
    };

    /**
     * @brief Address type lookup full specialization for extended addresses. 
     * 
     * @tparam  
     */
    template <>
    struct UwbMacAddressTypeImpl<UwbMacAddressLength::Extended> 
    {
        static constexpr UwbMacAddressType value = UwbMacAddressType::Extended;
    };

    /**
     * @brief Helper providing compile-time lookup of UwbMacAddressType for the
     * supported address lengths. 
     * 
     * @tparam Length The compile-time address length, typically from an array extent. 
     */
    template <size_t Length>
    inline constexpr UwbMacAddressType UwbMacAddressTypeV = UwbMacAddressTypeImpl<Length>::value;

    /**
     * @brief Type traits for uwb mac addresses.
     * 
     * This collects traits of uwb mac addresses for compile-time usage, based
     * on the address length. This maps from address length to mac address type,
     * value type (eg. std::array<> with correct extent), and their
     * corresponding types. 
     * 
     * @tparam Length 
     */
    template <std::size_t Length>
    struct UwbMacAddressTraits
    {
        using value_type = std::array<uint8_t, Length>;
        using length_type = std::integral_constant<std::size_t, Length>;

        static constexpr std::size_t length = length_type::value;
        static constexpr UwbMacAddressType address_type = detail::UwbMacAddressTypeV<Length>;
    };

    /**
     * @brief Helper which generically wraps a uwb mac address value.
     * 
     * This is used later in a templated constructor to allow a single declaration.
     * 
     * @tparam Length The length of the address.
     */
    template <std::size_t Length>
    struct UwbMacAddressValueWrapper : public UwbMacAddressTraits<Length>
    {
        explicit UwbMacAddressValueWrapper(typename UwbMacAddressTraits<Length>::value_type addressIn) : 
            address(std::move(addressIn)) 
        {}

        typename UwbMacAddressTraits<Length>::value_type address;
    };
} // namespace detail

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
     * @brief C++ types for each address type and aggregate value type.
     */
    using ShortType = std::array<uint8_t, ShortLength>;
    using ExtendedType = std::array<uint8_t, ExtendedLength>;
    using ValueType = std::variant<ShortType, ExtendedType>;

    /**
     * @brief Get the address type.
     * 
     * @return UwbMacAddressType 
     */
    UwbMacAddressType 
    GetType() const noexcept;

    /**
     * @brief Get the length of the address, in bytes.
     * 
     * @return std::size_t 
     */
    std::size_t
    GetLength() const noexcept;

    /**
     * @brief Get a view of the underlying value.
     * 
     * @return std::span<const uint8_t> 
     */
    std::span<const uint8_t> 
    GetValue() const noexcept;

    /**
     * @brief Return a string representation of the address.
     * 
     * The address is output as hexadecimal values, separated by colons, eg.
     *  
     *     ab (short address)
     *     1a2f3e4d (extended address)
     * 
     * @return std::string 
     */
    std::string
    ToString() const;

private:
    /**
     * @brief Compile-time helper to retrieve a reference to the active value of
     * the variant holding the mac address.
     * 
     * @tparam Length 
     * @param uwbMacAddress 
     * @return std::array<uint8_t, Length>& 
     */
    template <size_t Length>
    std::array<uint8_t, Length>& 
    UwbMacAddressValue(UwbMacAddress& uwbMacAddress)
    {
        return std::get<std::array<uint8_t, Length>>(uwbMacAddress.m_value);
    }

    /**
     * @brief Compile-time helper to generate a view of the address based on the
     * selected type.
     * 
     * @tparam Length 
     * @param uwbMacAddress 
     * @return std::span<const uint8_t> 
     */
    template <size_t Length>
    std::span<const uint8_t> 
    UwbMacAddressView(UwbMacAddress& uwbMacAddress)
    {
        auto& value = UwbMacAddressValue<Length>(uwbMacAddress);
        return { std::begin(value), std::end(value) };
    }

    /**
     * @brief Construct a new UwbMacAddress object based on compile-time deduced
     * arguments from a value wrapper.
     * 
     * @tparam Length The length of the address.
     * @param value The address value.
     */
    template <size_t Length>
    UwbMacAddress(detail::UwbMacAddressValueWrapper<Length> value) :
        m_type{ value.address_type },
        m_length{ value.length },
        m_value{ value.address },
        m_view{ UwbMacAddressView<Length>(*this) }
    {}

public:
    /**
     * @brief Construct a new UwbMacAddress object based on compile-time deduced
     * arguments.
     * 
     * This constructor should be preferred when possible.
     * 
     * @tparam Length The length of the address.
     * @param address The address value.
     */
    template <size_t Length>
    UwbMacAddress(std::array<uint8_t, Length> address) :
        UwbMacAddress(detail::UwbMacAddressValueWrapper<Length>{ address })
    {}

    /**
     * @brief Construct a default UwbMacAddress.
     */
    UwbMacAddress();

    /**
     * @brief Copy constructor.
     * 
     * @param other 
     */
    UwbMacAddress(const UwbMacAddress& other);

    /**
     * @brief Copy assignment operator.
     * 
     * @param other
     * @return UwbMacAddress& 
      */
    UwbMacAddress& 
    operator=(UwbMacAddress other);

private:
    /**
     * @brief Swap the data members of this instance with another one.
     * 
     * @param other The other instance to swap with.
     */
    void
    Swap(UwbMacAddress& other) noexcept;

    /**
     * @brief Initialize the address view.
     * 
     * This is the runtime version that must be used for the copy-constructor,
     * since the other object being copied does not have compile-time address
     * length information available.
     * 
     * Note that the assigned view will reflect whatever is in the m_value
     * variant at the time of the call. Thus, m_value must be appropriately
     * initialized prior to invoking this function for the view to be assigned
     * coherently.
     */
    void
    InitializeView();

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
     * @brief Note: the order of variable declarations here is critical. The
     * m_view member must be declared after m_value since it is a read-only view
     * of its contents. 
     */
    std::size_t m_length{ UwbMacAddressLength::Short };
    UwbMacAddressType m_type{ UwbMacAddressType::Short };

    /**
     * @brief The address value, which depends on the 'm_type' field.
     * 
     * Despite the 'm_type' field driving this value, the std::variant is the
     * ultimate ground-truth for the value, and the appropriate methods from
     * that class should be used to obtain it.
     */
    std::variant<ShortType, ExtendedType> m_value{ ShortType{ 0xBE, 0xEF } };

    /**
     * @brief Read-only view of the address data. 
     */
    std::span<const uint8_t> m_view;
};

bool
operator==(const UwbMacAddress&, const UwbMacAddress&) noexcept;

bool
operator!=(const UwbMacAddress&, const UwbMacAddress&) noexcept;

} // namespace uwb

namespace std
{
    template <>
    struct hash<uwb::UwbMacAddress>
    {
        size_t operator()(const uwb::UwbMacAddress& uwbMacAddress) const noexcept
        {
            const auto& value = uwbMacAddress.GetValue();
            return notstd::hash_range(std::cbegin(value), std::cend(value));
        }
    };
} // namespace std

#endif // UWB_DEVICE_ADDRESS_HXX