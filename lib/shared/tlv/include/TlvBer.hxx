
#ifndef TLV_BER_HXX
#define TLV_BER_HXX

#include "Tlv.hxx"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <vector>

namespace encoding
{
/**
 * @brief Represents a Basic Encoding Rules (BER) Tag-Length-Value (TLV)
 * structure, as defined by ISO/IEC 8825-1:2015.
 */
class TlvBer : public Tlv
{
public:
    /**
     * @brief See ISO/IEC 7816-4, 2005-01-15 section 5.2.2.1 'BER-TLV tag
     * fields', Table 7.
     */
    static constexpr uint8_t BitmaskClass          = 0b11000000;
    static constexpr uint8_t BitmaskType           = 0b00100000;
    static constexpr uint8_t BitmaskTag            = 0b00011111;

    static constexpr uint8_t ClassUniversal        = 0b00000000;
    static constexpr uint8_t ClassApplication      = 0b01000000;
    static constexpr uint8_t ClassContextSpecific  = 0b10000000;
    static constexpr uint8_t ClassPrivate          = 0b11000000;

    static constexpr uint8_t TypeConstructed       = 0b00100000;
    static constexpr uint8_t TypePrimitive         = 0b00000000;

    static constexpr uint8_t LengthTag2Byte = 0x81U;
    static constexpr uint8_t LengthTag3Byte = 0x82U;
    static constexpr uint8_t LengthTag4Byte = 0x83U;
    static constexpr uint8_t LengthTag5Byte = 0x84U;

    /**
     * @brief The class of the TLV.
     */
    enum class Class {
        Invalid,
        Universal,
        Application,
        ContextSpecific,
        Private,
    };

    /**
     * @brief The type of TLV.
     */
    enum class Type {
        Primitive,
        Constructed,
    };

    /**
     * @brief Determines if the specified tag denotes a constructed TlvBer.
     * 
     * @param tag The tag to check.
     * @return true 
     * @return false 
     */
    static bool
    TagIsConstructed(std::span<const uint8_t> tag);

    /**
     * @brief Get the TlvBer type from the tag value.
     * 
     * @param tag 
     * @return Type 
     */
    static Type
    GetTagType(std::span<const uint8_t> tag);

    /**
     * @brief Get the TlvBer class from the tag value.
     * 
     * @param tag 
     * @return Class 
     */
    static Class
    GetClass(std::span<const uint8_t> tag);

    /**
     * @brief Construct a new TlvBer with given tag and value.
     * 
     * @param tag The tag to use.
     * @param value The data value to use.
     */
    TlvBer(const std::vector<uint8_t>& tag, const std::vector<uint8_t>& length, const std::vector<uint8_t>& value);

    /**
     * @brief Construct a new constructed TlvBer object.
     * 
     * @param tag The tag to use.
     * @param values The constructed values.
     */
    TlvBer(const std::vector<uint8_t>& tag, std::vector<TlvBer> values);

    /**
     * @brief Returns whether this TLV contains a constructed value.
     * 
     * @return true 
     * @return false 
     */
    bool
    IsConstructed() const noexcept;

    /**
     * @brief Returns whether this TLV contains a primitive value.
     * 
     * @return true 
     * @return false 
     */
    bool
    IsPrimitive() const noexcept;

    /**
     * @brief Returns the type of this TLV.
     * 
     * @return Type 
     */
    Type
    GetType() const noexcept;

    /**
     * @brief Get the tag of the TLV.
     * 
     * @return std::span<const uint8_t> 
     */
    std::span<const uint8_t>
    GetTag() const noexcept;

    /**
     * @brief Get the Values object
     * 
     * @return const std::vector<TlvBer> 
     */
    std::vector<TlvBer>
    GetValues() const noexcept;

    /**
     * @brief Decode a Tlv from a blob of BER-TLV data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded). This must be a writeable pointer.
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    static ParseResult
    Parse(TlvBer** tlvOutput, const std::span<uint8_t>& data);

    /**
     * @brief Encode this TlvBer into binary and returns a vector of bytes
     * 
     */
    std::vector<uint8_t>
    ToBytes();

    /**
     * @brief Helper class to iteratively build a TlvBer. This allows separating
     * the creation logic from the main class and enables it to be immutable.
     * 
     * This allows the following pattern to build a TlvBer:
     * 
     * std::vector<uint8_t> tag81Value{};
     * Tlv tlv = MakeATlv{};
     * 
     * TlvBer::Builder builder{};
     * auto tlvBer = builder
     *      .SetTag(0xA3)
     *      .AddTlv(0x81, tag81Value)
     *      .AddTlv(tlv)
     *      .Build();
     */
    class Builder
    {
    private:
        /**
         * @brief Write a fixed-length array of data to the tlv storage buffer.
         * 
         * @tparam D must be std::span<const uint8_t> or std::array<uint8_t, N>
         * @param data 
         */
        template<class D>
        void
        WriteBytes(const D& data)
        {
            m_data.insert(std::cend(m_data), std::cbegin(data), std::cend(data));
        }

        /**
         * @brief Write a Value field, including the Length, to the tlv storage buffer.
         * 
         * @tparam D must be uint8_t, or std::span<const uint8_t>, or std::array<const uint8_t, N>
         * @param data 
         */
        template<size_t N>
        void
        WriteLengthAndValue(const std::array<const uint8_t, N>& data);

        void
        WriteLengthAndValue(const std::span<const uint8_t>& data);

        void
        WriteLengthAndValue(const uint8_t& data);

        /**
         * @brief subroutine to write some length octets
         * 
         */
        void
        WriteLength(uint64_t length);

        /**
         * @brief Generate the encoding of the length value. 
         * 
         * See ISO/IEC 7816-4, 2005-01-15 section 5.2.2.2 'BER-TLV length fields',
         * Table 8.
         * 
         * @param length The length value to get the encoding for.
         * @return std::vector<uint8_t> 
         */
        std::vector<uint8_t>
        GetLengthEncoding(size_t length);

    public:
        /**
         * @brief Set the tag of the top-level/parent TlvBer. This assumes that the input tag is already well constructed
         * 
         * @tparam T must be uint8_t, std::span<const uint8_t>, std::array<const uint8_t, N>
         * @param tag 
         * @return Builder& 
         */
        Builder&
        SetTag(const uint8_t& tag);

        Builder&
        SetTag(const std::span<const uint8_t>& tag);

        template<size_t N>
        Builder&
        SetTag(const std::array<uint8_t, N>& tag);

        /**
         * @brief Sets a sequence of data, as the tlv value
         * 
         * @tparam V must be uint8_t, std::span<const uint8_t>, std::array<const uint8_t, N>
         * @param value 
         * @return Builder& 
         */
        template<size_t N>
        Builder&
        SetValue(const std::array<const uint8_t, N>& value);

        Builder&
        SetValue(const std::span<const uint8_t>& value);

        Builder&
        SetValue(const uint8_t& value);

        /**
         * @brief Add a nested tlv to this tlv. This makes it a constructed TlvBer.
         * 
         * @tparam T, must be std::array<uint8_t, N>, or std::span<const uint8_t>
         * @tparam V, must be std::array<uint8_t, N>, std::span<const uint8_t> or uint8_t
         * @param tag 
         * @param value 
         * @return Builder& 
         */
        template<class T, class V>
        Builder&
        AddTlv(const T& tag, const V& value)
        {
            WriteBytes(tag);
            WriteLengthAndValue(value);
            m_validateConstructed = true;
            return *this;
        }

        /**
         * @brief Add a pre-existing tlv to this tlv. This makes it a constructed TlvBer.
         * 
         * @param tlv 
         * @return Builder& 
         */
        Builder&
        AddTlv(const Tlv& tlv);

        /**
         * @brief Build and return the TlvBer.
         * 
         * @return TlvBer 
         */
        TlvBer
        Build();

        /**
         * @brief clears the tag and data
         *
         * @return Builder& 
         */
        Builder&
        Reset();

        /**
         * @brief Top-level exception from which all context-specific exceptions derived.
         */
        struct InvalidTlvBerException : public std::exception
        {};

        /**
         * @brief Exception thrown when the specified tag does not specify the
         * constructed bit, which is required for tlvs constructed with the
         * builder.
         */
        struct InvalidTlvBerTagException : public InvalidTlvBerException
        {};

    private:
        /**
         * @brief Validates the to-be-built tlv has a valid tag.
         */
        void
        ValidateTag();

        /**
         * @brief Populates the Length member, only called by Build()
         * 
         */
        void
        CalculateAndSetLength();

    private:
        bool m_validateConstructed{ false };
        std::vector<uint8_t> m_tag;
        std::vector<uint8_t> m_length;
        std::vector<uint8_t> m_data;
    };

private:
    std::vector<uint8_t> m_tag;
    std::vector<uint8_t> m_length;
    std::vector<uint8_t> m_value;
    std::vector<TlvBer> m_valuesConstructed;
};

} // namespace encoding

#endif // TLV_BER_HXX
