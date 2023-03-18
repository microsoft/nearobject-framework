
#ifndef UWB_DEVICE_ADDRESS_HXX
#define UWB_DEVICE_ADDRESS_HXX

#include <algorithm>
#include <array>
#include <chrono>
#include <climits>
#include <cstdint>
#include <istream>
#include <optional>
#include <random>
#include <span>
#include <string>
#include <type_traits>
#include <variant>

#include <notstd/hash.hxx>

namespace uwb
{
/**
 * @brief See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section
 * 7.5.3.3, Table 53.
 */
enum class UwbMacAddressFcsType : uint8_t {
    Crc16 = 0,
    Crc32 = 1,
};

/**
 * @brief The type of uwb device object address.
 *
 * See FiRa Consortium UWB MAC Technical Requirements v1.3.0, Section 7.5.3.3,
 * Table 53.
 *
 * Skipping the value '1' below is intentional, as that value corresponds to an
 * unsupported uwb mac address type.
 */
enum class UwbMacAddressType : uint8_t {
    Short = 0,
    Extended = 2,
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
/**
 * @brief Compile-time address length lookup, given type.
 *
 * @tparam AddressType
 */
template <UwbMacAddressType AddressType>
struct UwbMacAddressSizeImpl
{};

/**
 * @brief Address length lookup full specialization for short addresses.
 *
 * @tparam
 */
template <>
struct UwbMacAddressSizeImpl<UwbMacAddressType::Short>
{
    static constexpr std::size_t value = UwbMacAddressLength::Short;
};

/**
 * @brief Address length lookup full specialization for extended addresses.
 *
 * @tparam
 */
template <>
struct UwbMacAddressSizeImpl<UwbMacAddressType::Extended>
{
    static constexpr std::size_t value = UwbMacAddressLength::Extended;
};

/**
 * @brief Compile-time address type lookup helper, given length.
 *
 * @tparam Length
 */
template <size_t Length>
struct UwbMacAddressTypeImpl
{};

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
 * @brief Helper providing compile-time lookup of UwbMacAddress size for the
 * supported address types.
 *
 * @tparam AddressType The compile-time address type to get the size for.
 */
template <UwbMacAddressType AddressType>
inline constexpr std::size_t UwbMacAddressSizeV = UwbMacAddressSizeImpl<AddressType>::value;

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
     * @brief Get the short address as a value (POD type).
     *
     * @return std::optional<uint16_t>
     */
    std::optional<uint16_t>
    GetValueShort() const;

    /**
     * @brief Return a string representation of the address.
     *
     * The address is output as hexadecimal values, separated by colons, eg.
     *
     *     ab (short address)
     *     1a:2f:3e:4d (extended address)
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
        m_length{ value.length },
        m_type{ value.address_type },
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
     * @brief Construct a new, randomly generated UwbMacAddress object based on
     * compile-time deduced arguments.
     *
     * @tparam AddressType The type of address to generate.
     * @return UwbMacAddress The randomly generated address value.
     */
    template <UwbMacAddressType AddressType = UwbMacAddressType::Extended>
    static UwbMacAddress
    Random()
    {
        using byte_generator = std::independent_bits_engine<std::mt19937, CHAR_BIT * sizeof(uint8_t), uint16_t>;

        // Create a new bit engine seeded with the current time. Note that this
        // could be predictable for a motivated attacker so should not be relied
        // upon if security is a concern.
        const auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
        byte_generator byteGenerator(static_cast<byte_generator::result_type>(timeSinceEpoch.count()));

        std::array<uint8_t, detail::UwbMacAddressSizeV<AddressType>> address{};
        std::generate(std::begin(address), std::end(address), [&byteGenerator]() {
            return static_cast<uint8_t>(byteGenerator());
        });

        return UwbMacAddress{ address };
    }

    /**
     * @brief Construct a new, randomly generated UwbMacAddress object based on
     * runtime provided arguments.
     * 
     * @param type The type of random address to generate.
     * @return UwbMacAddress The randomly generated address value.
     */
    static UwbMacAddress
    Random(UwbMacAddressType type);

    /**
     * @brief Creates a UwbMacAddress from a string representation of a mac address.
     * 
     * @param addressString The mac address string.
     * @param addressType The type of mac address.
     * @return std::optional<UwbMacAddress> The constructed optional UwbMacAddress.
     */
    static std::optional<UwbMacAddress>
    FromString(const std::string addressString, UwbMacAddressType addressType);

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

    /**
     * @brief Three-way comparison operator.
     */
    auto
    operator<=>(const UwbMacAddress& other) const noexcept;

private:
    /**
     * @brief Construct a new UwbMacAddress object given an input string of
     * semicolon-delimited bytes.
     *
     * @param addressString The semicolon-delimited address string.
     * @param addressType The type of mac address that addressString represents.
     */
    UwbMacAddress(std::string addressString, UwbMacAddressType addressType);

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

std::istream&
operator>>(std::istream& stream, UwbMacAddress& uwbMacAddress) noexcept;

} // namespace uwb

namespace std
{
template <>
struct hash<uwb::UwbMacAddress>
{
    size_t
    operator()(const uwb::UwbMacAddress& uwbMacAddress) const noexcept
    {
        const auto& value = uwbMacAddress.GetValue();
        return notstd::hash_range(std::cbegin(value), std::cend(value));
    }
};
} // namespace std

#endif // UWB_DEVICE_ADDRESS_HXX