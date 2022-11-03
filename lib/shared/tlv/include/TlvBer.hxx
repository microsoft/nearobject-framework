
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
    static constexpr uint8_t BitmaskClass = 0b11000000;
    static constexpr uint8_t BitmaskType = 0b00100000;
    static constexpr uint8_t BitmaskTagFirstByte = 0b00011111;
    static constexpr uint8_t BitmaskTagLastByte = 0b10000000;
    static constexpr uint8_t BitmaskTagShort = 0b00011111;
    static constexpr uint8_t BitmaskTagLong = 0b01111111;
    static constexpr uint8_t BitmaskLengthForm = 0b10000000;
    static constexpr uint8_t BitmaskLengthShort = 0b01111111;
    static constexpr uint8_t BitmaskLengthNumOctets = 0b01111111;

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
    static constexpr uint8_t LengthFormShort = 0b00000000;
    static constexpr uint8_t LengthFormLong = 0b10000000;

    static constexpr uint8_t TagValueLongField = 0b00011111;
    static constexpr uint8_t TagValueLastByte = 0b10000000;

    static constexpr uint8_t MaxNumOctetsInLengthEncoding = 5;

    /**
     * @brief The class of the TLV.
     */
    enum class TagClass {
        Invalid,
        Universal,
        Application,
        ContextSpecific,
        Private,
    };

    /**
     * @brief The type of TLV.
     */
    enum class TagType {
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
     * @return TagType 
     */
    static TagType
    GetTagType(uint8_t tag);

    /**
     * @brief Get the TlvBer class from the tag value.
     * 
     * @param tag 
     * @return TagClass 
     */
    static TagClass
    GetTagClass(uint8_t tag);

    /**
     * @brief Generate the encoding of the length value. 
     * 
     * See ISO/IEC 7816-4, 2005-01-15 section 5.2.2.2 'BER-TLV length fields',
     * Table 8.
     * 
     * @param length The length value to get the encoding for.
     * @return std::vector<uint8_t> 
     */
    static std::vector<uint8_t>
    GetLengthEncoding(std::size_t length);
    
    /**
     * @brief Construct a new BerTlv with given tag and value.
     * 
     * @param tag The tag to use.
     * @param value The data value to use.
     */
    TlvBer(TagClass tagClass, TagType tagType, const std::vector<uint8_t>& tagNumber, const std::vector<uint8_t>& tagComplete, const std::vector<uint8_t>& value);

    /**
     * @brief Construct a new constructed TlvBer object.
     * 
     * @param tag The tag to use.
     * @param values The constructed values.
     */
    TlvBer(TagClass tagClass, TagType tagType, const std::vector<uint8_t>& tagNumber, const std::vector<uint8_t>& tagComplete, std::vector<TlvBer>& value);
   
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
     * @return TagType 
     */
    TagType
    GetTagType() const noexcept;

    /**
     * @brief Returns the class of this TLV.
     * 
     * @return TagClass
     */
    TagClass
    GetTagClass() const noexcept;

    /**
     * @brief Get the tagNumber of the TLV.
     * 
     * @return std::span<const uint8_t> 
     */
    std::span<const uint8_t>
    GetTagNumber() const noexcept;

    /**
     * @brief Get the tagComplete of the TLV.
     * 
     * @return std::span<const uint8_t> 
     */
    std::span<const uint8_t>
    GetTagComplete() const noexcept;

    /**
     * @brief Get the Values object. Returns empty if this object is Primitive
     * 
     * @return const std::vector<TlvBer> 
     */
    std::vector<TlvBer>
    GetValues() const noexcept;

    /**
     * @brief parses the first bytes to determine if they encode a tag properly. 
     *        Advances the iterator it to once past the tag
     *        Writes to tagClass, tagType, tagNumber, tagComplete, and bytesParsed if a proper tag was parsed
     * 
     */
    template <typename Iterable>
    static
    Tlv::ParseResult
    ParseTag(TagClass& tagClass, TagType& tagType,std::vector<uint8_t>& tagNumber, std::vector<uint8_t>& tagComplete, Iterable& data, size_t& bytesParsed);

    static
    Tlv::ParseResult
    ParseTag(TagClass& tagClass, TagType& tagType,std::vector<uint8_t>& tagNumber, std::vector<uint8_t>& tagComplete, uint8_t tag);

    /**
     * @brief parses the first bytes to determine if they encode a length properly
     *        Writes to length and bytesParsed if a proper length was parsed
     * 
     */
    template <typename Iterable>
    static
    Tlv::ParseResult
    ParseLength(size_t& length, Iterable& data, size_t& bytesParsed);

    /**
     * @brief parses the first bytes to determine if they encode a value properly
     *        Writes to valueOutput if a proper value was parsed
     * 
     */
    template <typename Iterable>
    static
    Tlv::ParseResult
    ParseValue(std::vector<uint8_t>& valueOutput, size_t length, Iterable& data, size_t& bytesParsed);

    /**
     * @brief Decode a Tlv from a blob of BER-TLV data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded). This must be a writeable pointer.
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    template<typename Iterable>
    static ParseResult
    Parse(TlvBer **tlvOutput, Iterable& data);

    /**
     * @brief Encode this TlvBer into binary and returns a vector of bytes
     * 
     */
    std::vector<uint8_t>
    ToBytes() const;

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
         * @tparam data must be an Iterable over uint8_t
         * @param data 
         */
        template<typename Iterable>
        void
        WriteBytes(Iterable& data)
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
        WriteLengthAndValue(std::span<const uint8_t> data);

        void
        WriteLengthAndValue(uint8_t data);

        /**
         * @brief subroutine to write some length octets
         * 
         */
        void
        WriteLength(uint64_t length);

    public:
        /**
         * @brief Set the tag of the top-level/parent BerTlv.
         * 
         * @param tag 
         * @return Builder& 
         */
        Builder&
        SetTag(uint8_t tag);

        /**
         * @brief Set the tag of the top-level/parent BerTlv.
         * 
         * @tparam Iterable 
         * @param tag 
         * @return Builder& 
         */
        template <typename Iterable>
        Builder&
        SetTag(Iterable& tag)
        {
            size_t bytesParsed;
            ParseTag(m_tagClass,m_tagType,m_tagNumber,m_tagComplete,tag,bytesParsed);
            return *this;
        }

        /**
         * @brief Sets a sequence of data, as the tlv value
         * 
         * @tparam V must be uint8_t, std::span<const uint8_t>, std::array<const uint8_t, N>
         * @param value 
         * @return Builder& 
         */
        template<typename Iterable>
        Builder&
        SetValue(Iterable& value){
            m_data.assign(std::cbegin(value), std::cend(value));
            return *this;
        }

        Builder&
        SetValue(uint8_t value);

        /**
         * @brief Add a nested tlv to this tlv. This makes it a constructed TlvBer. TODO will have to add to the m_valuesConstructed field
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

    private:
        bool m_validateConstructed{ false };
        TagClass m_tagClass;
        TagType m_tagType;
        std::vector<uint8_t> m_tagComplete;
        std::vector<uint8_t> m_tagNumber;
        std::vector<uint8_t> m_data;
        std::vector<TlvBer> m_valuesConstructed;
    };

private:
    TagClass m_tagClass;
    TagType m_tagType;
    std::vector<uint8_t> m_tagNumber;
    std::vector<uint8_t> m_tagComplete;
    std::vector<uint8_t> m_value;
    std::vector<TlvBer> m_valuesConstructed;
};

} // namespace encoding

#endif // TLV_BER_HXX
