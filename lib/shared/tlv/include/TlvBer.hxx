
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
class TlvBer : public Tlv
{
public:
    static constexpr uint8_t BitmaskClass = 0b11000000;
    static constexpr uint8_t BitmaskType  = 0b00100000;
    static constexpr uint8_t BitmaskTag   = 0b00011111;

    static constexpr uint8_t TypeConstructed = 0b00100000;
    static constexpr uint8_t TypePrimitive   = 0b00000000;

    enum class ClassTypes {
        UniversalClass                    = 0b00000000,
        ApplicationClass                  = 0b01000000,
        ContextSpecificClass              = 0b10000000,
        PrivateClass                      = 0b11000000
    };

    /**
     * @brief Determines if the specified tag denotes a constructed TlvBer.
     * 
     * @param tag The tag to check.
     * @return true 
     * @return false 
     */
    static 
    bool TagIsConstructed(std::span<const uint8_t> tag);

    /**
     * @brief Construct a new TlvBer with given tag and value.
     * 
     * @param tag The tag to use.
     * @param value The data value to use.
     */
    TlvBer(const std::vector<uint8_t>& tag, const std::vector<uint8_t>& value);

    /**
     * @brief Decode a Tlv from a blob of BER-TLV data.
     *
     * @param tlvOutput The decoded Tlv, if parsing was successful (ParseResult::Succeeded). This must be a writeable pointer.
     * @param data The data to parse a Tlv from.
     * @return ParseResult The result of the parsing operation.
     */
    static ParseResult
    Parse(TlvBer **tlvOutput, const std::span<uint8_t>& data);

    /**
     * @brief Helper class to iteratively build a TlvBer. This allows separating
     * the creation logic from the main class and enables it to be immutable.
     * 
     * This allows the following pattern to build a TlvBer:
     * 
     * std::vector<uint8_t> tag81Value{};
     * Tlv tag82Value = MakeATlv{};
     * 
     * TlvBer::Builder builder{};
     * auto tlvBer = builder
     *      .SetTag(0xA3)
     *      .AddTlv(0x81, tag81Value)
     *      .AddTlv(Tlv.Tag, tag82Value)
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
        template <class D>
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
        template <class D>
        void
        WriteLengthAndValue(const D& data);
 
        /**
         * @brief subroutine to write some length octets
         * 
         */
        void WriteLength(uint64_t length);

    public:
        /**
         * @brief Set the tag of the top-level/parent TlvBer.
         * 
         * @tparam IterableOfBytes must be uint8_t, std::span<const uint8_t>, std::array<const uint8_t, N>
         * @param tag 
         * @return Builder& 
         */
        template <typename IterableOfBytes>
        Builder&
        SetTag(const IterableOfBytes& tag);

        /**
         * @brief Sets a sequence of data, including its length, as the tlv
         * storage buffer.
         * 
         * @tparam V must be uint8_t, std::span<const uint8_t>, std::array<const uint8_t, N>
         * @param value 
         * @return Builder& 
         */
        template<class V>
        Builder&
        SetValue(const V& value){
            WriteLengthAndValue(value);
            return *this;
        }

        /**
         * @brief Add a nested tlv to this tlv. This makes it a constructed TlvBer.
         * 
         * @tparam T, must be std::array<uint8_t, N>, or std::span<const uint8_t>
         * @tparam V, must be std::array<uint8_t, N>, std::span<const uint8_t> or uint8_t
         * @param tag 
         * @param value 
         * @return Builder& 
         */
        template <class T, class V>
        Builder&
        AddTlv(const T& tag, const V& value){
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
         * @brief Top-level exception from which all context-specific exceptions derived.
         */
        struct InvalidTlvBerException : public std::exception {};

        /**
         * @brief Exception thrown when the specified tag does not specify the
         * constructed bit, which is required for tlvs constructed with the
         * builder.
         */
        struct InvalidTlvBerTagException : public InvalidTlvBerException {};

    private:
        /**
         * @brief Validates the to-be-built tlv has a valid tag.
         */
        void
        ValidateTag();

    private:
        bool m_validateConstructed{ false };
        std::vector<uint8_t> m_tag;
        std::vector<uint8_t> m_data;
    };

private:
    std::vector<uint8_t> m_tag;
    std::vector<uint8_t> m_value;
};

} // namespace encoding

#endif // TLV_BER_HXX
