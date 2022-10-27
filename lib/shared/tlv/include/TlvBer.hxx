
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

    /**
     * @brief Determines if the specified tag denotes a constructed BerTlv.
     * 
     * @param tag The tag to check.
     * @return true 
     * @return false 
     */
    static 
    bool TagIsConstructed(std::span<const uint8_t> tag);

    /**
     * @brief Construct a new BerTlv with given tag and value.
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
     * @brief Helper class to iteratively build a BerTlv. This allows separating
     * the creation logic from the main class and enables it to be immutable.
     * 
     * This allows the following pattern to build a TlvBer:
     * 
     * std::vector<uint8_t> tag81Value{};
     * Tlv tag82Value = MakeATlv{};
     * 
     * BerTlv::Builder builder{};
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
         * @tparam N 
         * @param data 
         */
        template <size_t N>
        void
        WriteData(std::array<uint8_t, N> data)
        {
            m_data.insert(std::cend(m_data), std::cbegin(data), std::cend(data));
        }

        /**
         * @brief Write an unsigned 32-bit integer to the tlv storage buffer.
         * 
         * @param data 
         */
        void 
        WriteData(uint32_t data);

        /**
         * @brief Write a sequence of data to the tlv storage buffer.
         * 
         * @param data 
         */
        void
        WriteData(std::span<const uint8_t> data);

        /**
         * @brief Write a single-byte value, including its length, to the tlv
         * storage buffer.
         * 
         * @param value 
         */
        void
        WriteValue(uint8_t value);

        /**
         * @brief Write a sequence of data, including its length, to the tlv
         * storage buffer.
         * 
         * @param value 
         */
        void
        WriteValue(std::span<const uint8_t> value);

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
            m_tag.assign(std::cbegin(tag), std::cend(tag));
            return *this;
        }

        /**
         * @brief Add a nested tlv to this tlv. This makes it a constructed BerTlv.
         * 
         * @tparam N 
         * @param tag 
         * @param value 
         * @return Builder& 
         */
        template <size_t N>
        Builder&
        AddTlv(std::array<uint8_t, N> tag, uint8_t value)
        {
            return AddTlv([&]{
                WriteData(tag);
                WriteValue(value);
            });
        }

        /**
         * @brief Add a nested tlv to this tlv. This makes it a constructed BerTlv.
         * 
         * @tparam N 
         * @param tag 
         * @param value 
         * @return Builder& 
         */
        template <size_t N>
        Builder&
        AddTlv(std::array<uint8_t, N> tag, std::span<uint8_t> value)
        {
            return AddTlv([&]{
                WriteData(tag);
                WriteValue(value);
            });
        }

        /**
         * @brief Add a pre-existing tlv to this tlv. This makes it a constructed BerTlv.
         * 
         * @param tlv 
         * @return Builder& 
         */
        Builder&
        AddTlv(const Tlv& tlv);

        /**
         * @brief Build and return the BerTlv.
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
         * @brief Helper function which adds a tlv to the parent tlv.
         * 
         * This does internal housekeeping to ensure the parent tag is set as
         * constructed when the tlv is eventually built.
         * 
         * @param func The function to add the actual tlv.
         * @return Builder& 
         */
        Builder&
        AddTlv(auto&& func);

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
