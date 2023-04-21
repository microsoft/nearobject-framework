
#ifndef UWB_SESSION_DATA_HXX
#define UWB_SESSION_DATA_HXX

#include <cstdint>
#include <memory>
#include <optional>

#include <notstd/hash.hxx>
#include <tlv/TlvBer.hxx>

#include <uwb/protocols/fira/SecureRangingInfo.hxx>
#include <uwb/protocols/fira/StaticRangingInfo.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Describes UWB session configuration.
 *
 * See FiRa Consortium Common Service Management Layer Technical Specification
 * v1.0.0, Section 6.3, 'SESSION_DATA', pages 50-54.
 */
struct UwbSessionData
{
    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Session Data structure',
     * Table 53, pages 103-109.
     */
    static constexpr uint16_t Tag = 0xBF78;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Session Data structure',
     * Table 53, pages 103-107.
     */
    enum class ParameterTag : uint8_t {
        SessionDataVersion = 0x80,
        SessionId = 0x81,
        SubSessionId = 0x82,
        ConfigurationParameters = 0xA3,
        StaticRangingInfo = 0xA4,
        SecureRangingInfo = 0xA5,
        RegulatoryInformation = 0xA6,
        UwbConfigAvailable = 0x87,
    };

    /**
     * @brief Construct UwbSessionData object with default values and settings.
     */
    UwbSessionData() = default;

    /**
     * @brief Convert this object into a FiRa Data Object (DO).
     *
     * @return std::unique_ptr<encoding::TlvBer>
     */
    std::unique_ptr<encoding::TlvBer>
    ToDataObject() const;

    /**
     * @brief Attempt to create a UwbConfiguration object from a TlvBer.
     *
     * @param tlv
     * @return UwbConfiguration
     */
    static UwbSessionData
    FromDataObject(const encoding::TlvBer& tlv);

    uint16_t sessionDataVersion{ 0 };
    uint32_t sessionId{ 0 };
    uint32_t subSessionId{ 0 };
    UwbConfiguration uwbConfiguration{};
    std::optional<StaticRangingInfo> staticRangingInfo;
    std::optional<SecureRangingInfo> secureRangingInfo;
};

} // namespace uwb::protocol::fira

namespace std
{
template <>
struct hash<uwb::protocol::fira::UwbSessionData>
{
    std::size_t
    operator()(const uwb::protocol::fira::UwbSessionData& uwbSessionData) const noexcept
    {
        std::size_t value = 0;
        notstd::hash_combine(value,
            uwbSessionData.sessionDataVersion,
            uwbSessionData.sessionId,
            uwbSessionData.subSessionId,
            uwbSessionData.uwbConfiguration,
            uwbSessionData.staticRangingInfo,
            uwbSessionData.secureRangingInfo);
        return value;
    }
};
} // namespace std

#endif // UWB_SESSION_DATA_HXX
