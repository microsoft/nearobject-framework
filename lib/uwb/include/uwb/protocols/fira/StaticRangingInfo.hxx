
#ifndef FIRA_STATIC_RANGING_INFO_HXX
#define FIRA_STATIC_RANGING_INFO_HXX

#include <array>
#include <cstdint>

namespace uwb::protocol::fira
{
struct StaticRangingInfo
{
    /**
     * @brief See FiRa Consortium MAC Technical Requirements v1.3.0, 
     * Section D.1.8 STS, Figure 19, page 70.
     */
    static constexpr auto InitializationVectorLength = 6;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * page 101.
     */
    static constexpr uint8_t Tag = 0xA4;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * page 101.
     */
    enum class ParameterTag : uint8_t {
        VendorId = 0x80,
        StaticStsIv = 0x81,
    };

    uint16_t VendorId;
    std::array<uint8_t, InitializationVectorLength> InitializationVector;
};
} // namespace uwb::protocol::fira

#endif // FIRA_STATIC_RANGING_INFO_HXX
