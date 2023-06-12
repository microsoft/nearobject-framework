
#ifndef FIRA_SECURE_RANGING_INFO_HXX
#define FIRA_SECURE_RANGING_INFO_HXX

#include <cstdint>
#include <iterator>
#include <vector>

#include <notstd/hash.hxx>

#include <tlv/TlvBer.hxx>

namespace uwb::protocol::fira
{
struct SecureRangingInfo
{
    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 107-108.
     */
    static constexpr uint8_t Tag = 0xA5;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 101-102.
     */
    enum class ParameterTag : uint8_t {
        UwbSessionKeyInfo = 0x80,
        ResponderSpecificSubSessionKeyInfo = 0x81,
        SusAdditionalParameters = 0x82,
    };

    /**
     * @brief Convert this object into a FiRa Data Object (DO).
     *
     * @return std::unique_ptr<encoding::TlvBer>
     */
    std::unique_ptr<encoding::TlvBer>
    ToDataObject() const;

    /**
     * @brief 
     * 
     * @param tlv 
     * @return SecureRangingInfo 
     */
    static SecureRangingInfo
    FromDataObject(const encoding::TlvBer& tlv);

    std::vector<uint8_t> UwbSessionKeyInfo;
    std::vector<uint8_t> ResponderSpecificSubSessionKeyInfo;
    std::vector<uint8_t> SusAdditionalParameters;
};
} // namespace nearobject::protocol::fira

namespace std
{
template <>
struct hash<uwb::protocol::fira::SecureRangingInfo>
{
    std::size_t
    operator()(const uwb::protocol::fira::SecureRangingInfo& secureRangingInfo) const noexcept
    {
        std::size_t value = 0;
        notstd::hash_combine(value,
            notstd::hash_range(std::cbegin(secureRangingInfo.UwbSessionKeyInfo), std::cend(secureRangingInfo.UwbSessionKeyInfo)),
            notstd::hash_range(std::cbegin(secureRangingInfo.ResponderSpecificSubSessionKeyInfo), std::cend(secureRangingInfo.ResponderSpecificSubSessionKeyInfo)),
            notstd::hash_range(std::cbegin(secureRangingInfo.SusAdditionalParameters), std::cend(secureRangingInfo.SusAdditionalParameters)));
        return value;
    }
};
} // namespace std

#endif // FIRA_SECURE_RANGING_INFO_HXX
