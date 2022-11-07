
#ifndef TLV_BER_HXX
#define TLV_BER_HXX

#include "Tlv.hxx"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace encoding
{
/**
 * @brief Validates the given type is the TlvBer data unit type.
 * 
 * @tparam T 
 */
template <typename T>
concept IsTlvBerDataUnitType = std::is_same_v<T, uint8_t>;

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

    static constexpr uint8_t ClassUniversal = 0b00000000;
    static constexpr uint8_t ClassApplication = 0b01000000;
    static constexpr uint8_t ClassContextSpecific = 0b10000000;
    static constexpr uint8_t ClassPrivate = 0b11000000;

    static constexpr uint8_t TypeConstructed = 0b00100000;
    static constexpr uint8_t TypePrimitive = 0b00000000;

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
     * @return TlvBer::Type 
     */
    static TlvBer::Type
    GetType(uint8_t tag);

    /**
     * @brief Get the TlvBer class from the tag value.
     * 
     * @param tag 
     * @return Class 
     */
    static TlvBer::Class
    GetClass(uint8_t tag);

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
    TlvBer(TlvBer::Class tlvClass, TlvBer::Type tlvType, const std::vector<uint8_t>& tagNumber, const std::vector<uint8_t>& tagComplete, const std::vector<uint8_t>& value);

    /**
     * @brief Construct a new constructed TlvBer object.
     * 
     * @param tag The tag to use.
     * @param values The constructed values.
     */
    TlvBer(TlvBer::Class tlvClass, TlvBer::Type tlvType, const std::vector<uint8_t>& tagNumber, const std::vector<uint8_t>& tagComplete, std::vector<TlvBer>& value);
   
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
     * @return TlvBer::Type 
     */
    TlvBer::Type
    GetType() const noexcept;

    /**
     * @brief Returns the class of this TLV.
     * 
     * @return Class
     */
    TlvBer::Class
    GetClass() const noexcept;

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
    GetTag() const noexcept;

    /**
     * @brief Get the Values object. Returns empty if this object is Primitive
     * 
     * @return const std::vector<TlvBer> 
     */
    std::vector<TlvBer>
    GetValues() const noexcept;

    /**
     * @brief Parses the first bytes to determine if they encode a tag properly.
     * Advances the iterator it to once past the tag. Writes to tlvClass,
     * tlvType, tagNumber, tagComplete, and bytesParsed if a proper tag was
     * parsed.
     * 
     * @tparam Iterable 
     * @param tlvClass 
     * @param tlvType 
     * @param tagNumber 
     * @param tagComplete 
     * @param data 
     * @param bytesParsed 
     * @return Tlv::ParseResult 
     */
    template <typename Iterable>
    static Tlv::ParseResult
    ParseTag(TlvBer::Class& tlvClass, TlvBer::Type& tlvType, std::vector<uint8_t>& tagNumber, std::vector<uint8_t>& tagComplete, Iterable& data, std::size_t& bytesParsed)
    {
        auto dataIt = std::cbegin(data);
        auto dataEnd = std::cend(data);
        if (dataIt == dataEnd) {
            return Tlv::ParseResult::Failed;
        }

        bytesParsed = 0;
        tlvClass = GetClass(*dataIt);
        tlvType = GetType(*dataIt);

        // Is tag short type?
        if ((*dataIt & BitmaskTagFirstByte) != TagValueLongField) {
            tagComplete.push_back(*dataIt);
            tagNumber.push_back(*dataIt & BitmaskTagShort);
            return Tlv::ParseResult::Succeeded;
        }

        // Tag is long-type. 
        tagComplete.push_back(*dataIt);
        
        // check the second byte
        std::advance(dataIt, 1);
        if (dataIt == dataEnd) {
            return Tlv::ParseResult::Failed;
        } else if (*dataIt < 0x1F) {
            return Tlv::ParseResult::Failed;
        }

        tagNumber.push_back(*dataIt & BitmaskTagLong);
        tagComplete.push_back(*dataIt);

        // check the third byte?
        if ((*dataIt & BitmaskTagLastByte) == TagValueLastByte) {
            std::advance(dataIt, 1);
            if (dataIt == dataEnd) {
                return Tlv::ParseResult::Failed;
            }
            
            // make sure there's no fourth byte
            if ((*dataIt & BitmaskTagLastByte) == TagValueLastByte) {
                return Tlv::ParseResult::Failed;
            }
            
            tagNumber.push_back(*dataIt & BitmaskTagLong);
            tagComplete.push_back(*dataIt);
        }

        // advance the dataIt to once past the tag
        std::advance(dataIt, 1); 
        bytesParsed = std::distance(std::cbegin(data), dataIt);

        return Tlv::ParseResult::Succeeded;
    }

    /**
     * @brief 
     * 
     * @param tlvClass 
     * @param tlvType 
     * @param tagNumber 
     * @param tagComplete 
     * @param tag 
     * @return Tlv::ParseResult 
     */
    static Tlv::ParseResult
    ParseTag(Class& tlvClass, TlvBer::Type& tlvType, std::vector<uint8_t>& tagNumber, std::vector<uint8_t>& tagComplete, uint8_t tag);

    /**
     * @brief Parses the first bytes to determine if they encode a length
     * properly. Writes to length and bytesParsed if a proper length was parsed.    * @brief 
     * 
     * @tparam Iterable 
     * @param length 
     * @param data 
     * @param bytesParsed 
     * @return Tlv::ParseResult 
     */
    template <typename Iterable>
    static Tlv::ParseResult
    ParseLength(size_t& length, Iterable& data, std::size_t& bytesParsed) 
    {
        auto dataIt = std::cbegin(data);
        auto dataEnd = std::cend(data);
        if (dataIt == dataEnd) {
            return Tlv::ParseResult::Failed;
        }

        bytesParsed = length = 0;

        // Is length short form?
        if ((*dataIt & BitmaskLengthForm) == LengthFormShort) {
            length = *dataIt & BitmaskLengthShort;
        } else {
            // Length is long form.
            auto numFollowingOctets = *dataIt & BitmaskLengthNumOctets;
            if (numFollowingOctets >= MaxNumOctetsInLengthEncoding) {
                return Tlv::ParseResult::Failed;
            }

            for (auto i = 0; i < numFollowingOctets; i++) {
                std::advance(dataIt, 1);
                if (dataIt == dataEnd) {
                    return Tlv::ParseResult::Failed;
                }

                length  = length << 8U;
                length |= *dataIt;
            }
        }

        // Advance the dataIt to once past the lengthEncoding.
        std::advance(dataIt, 1); 
        bytesParsed = std::distance(std::cbegin(data), dataIt);

        return Tlv::ParseResult::Succeeded;
    }

    /**
     * @brief Parses the first bytes to determine if they encode a value
     * properly. Writes to valueOutput if a proper value was parsed.
     * 
     * @tparam Iterable 
     * @param valueOutput 
     * @param length 
     * @param data 
     * @param bytesParsed 
     * @return Tlv::ParseResult 
     */
    template <typename Iterable>
    static Tlv::ParseResult
    ParseValue(std::vector<uint8_t>& valueOutput, std::size_t length, Iterable& data, std::size_t& bytesParsed) {
        const auto dataIt = std::cbegin(data);
        const auto dataEnd = std::cend(data);
        if (std::distance(dataIt, dataEnd) < length) {
            return Tlv::ParseResult::Failed;
        }

        valueOutput = std::vector<uint8_t>{ dataIt, std::next(dataIt, length) };
        bytesParsed = length;

        return Tlv::ParseResult::Succeeded;
    }

    /**
     * @brief Decode a Tlv from a blob of BER-TLV data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded). This must be a writeable pointer.
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    template <typename Iterable>
    static ParseResult
    Parse(TlvBer **tlvOutput, Iterable& dataInput)
    {
        if (!tlvOutput) {
            return Tlv::ParseResult::Failed;
        }

        std::span<uint8_t> data{std::cbegin(dataInput), std::cend(dataInput)};

        *tlvOutput = nullptr;
        auto parseResult = Tlv::ParseResult::Failed;

        // Parse tag.
        TlvBer::Class tlvClass = TlvBer::Class::Invalid;
        TlvBer::Type tlvType = TlvBer::Type::Primitive;
        std::vector<uint8_t> tagNumber;
        std::vector<uint8_t> tagComplete;
        std::size_t offset = 0;
        std::size_t bytesParsed = 0;
        parseResult = ParseTag(tlvClass, tlvType, tagNumber, tagComplete, data, bytesParsed);
        if (Tlv::ParseResult::Succeeded != parseResult) {
            return parseResult;
        }

        // Parse length.
        offset += bytesParsed;
        std::size_t length = 0;
        auto subspan = data.subspan(offset);
        parseResult = ParseLength(length, subspan, bytesParsed);
        if (Tlv::ParseResult::Succeeded != parseResult) {
            return parseResult;
        }

        // Parse value.
        offset += bytesParsed;
        std::vector<uint8_t> value;
        subspan = data.subspan(offset);
        parseResult = ParseValue(value, length, subspan, bytesParsed);
        if (Tlv::ParseResult::Succeeded != parseResult) {
            return parseResult;
        }

        switch (tlvType) {
        case TlvBer::Type::Constructed:
            // TODO: parse nested tlvs
            break;
        case TlvBer::Type::Primitive: {
            *tlvOutput = std::make_unique<TlvBer>(tlvClass, tlvType, tagNumber, tagComplete, value).release();
            parseResult = ParseResult::Succeeded;
            break;
        }
        }

        return parseResult;
    }

    /**
     * @brief Encode this TlvBer into binary and return a vector of bytes.
     * 
     * @return std::vector<uint8_t> 
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
         * @tparam Iterable 
         * @param data 
         */
        template <typename Iterable>
        requires IsTlvBerDataUnitType<typename Iterable::value_type>
        void
        WriteBytes(Iterable& data)
        {
            m_data.insert(std::cend(m_data), std::cbegin(data), std::cend(data));
        }

        /**
         * @brief Write a Value field, including the Length, to the tlv storage
         * buffer.
         * 
         * @tparam N 
         * @param data 
         */
        template <size_t N>
        void
        WriteLengthAndValue(const std::array<const uint8_t, N>& data);

        /**
         * @brief 
         * 
         * @param data 
         */
        void
        WriteLengthAndValue(std::span<const uint8_t> data);

        /**
         * @brief 
         * 
         * @param data 
         */
        void
        WriteLengthAndValue(uint8_t data);

        /**
         * @brief Subroutine to write some length octets.
         * 
         * @param length 
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
            std::size_t bytesParsed;
            ParseTag(m_class, m_type, m_tagNumber, m_tag, tag, bytesParsed);
            return *this;
        }

        /**
         * @brief Sets a sequence of data, as the tlv value.
         * 
         * @tparam Iterable 
         * @param value 
         * @return requires& 
         */
        template <typename Iterable>
        requires IsTlvBerDataUnitType<typename Iterable::value_type>
        Builder&
        SetValue(Iterable& value)
        {
            m_data.assign(std::cbegin(value), std::cend(value));
            return *this;
        }

        /**
         * @brief Set the Value object.
         * 
         * @param value 
         * @return Builder& 
         */
        Builder&
        SetValue(uint8_t value);

        /**
         * @brief Add a nested tlv to this tlv. This makes it a constructed
         * TlvBer. TODO will have to add to the m_valuesConstructed field.
         * 
         * @tparam TagT
         * @tparam ValueT
         * @param tag 
         * @param value 
         * @return Builder& 
         */
        template <typename TagT, typename ValueT>
        requires 
            IsTlvBerDataUnitType<typename TagT::value_type> && 
            IsTlvBerDataUnitType<typename ValueT::value_type>
        Builder&
        AddTlv(const TagT& tag, const ValueT& value)
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
        TlvBer::Class m_class;
        TlvBer::Type m_type;
        std::vector<uint8_t> m_tag;
        std::vector<uint8_t> m_tagNumber;
        std::vector<uint8_t> m_data;
        std::vector<TlvBer> m_valuesConstructed;
    };

private:
    TlvBer::Class m_class;
    TlvBer::Type m_type;
    std::vector<uint8_t> m_tagNumber;
    std::vector<uint8_t> m_tag;
    std::vector<uint8_t> m_value;
    std::vector<TlvBer> m_valuesConstructed;
};

} // namespace encoding

#endif // TLV_BER_HXX
