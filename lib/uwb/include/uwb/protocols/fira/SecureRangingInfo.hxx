
#ifndef FIRA_SECURE_RANGING_INFO_HXX
#define FIRA_SECURE_RANGING_INFO_HXX

#include <cstdint>
#include <vector>

namespace nearobject::protocol::fira
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
    enum class ParameterTag : uint8_t
    {
        UwbSessionKeyInfo = 0x80,
        ResponderSpecificSubSessionKeyInfo = 0x81,
        SusAdditionalParameters = 0x82,
    };

    std::vector<uint8_t> UwbSessionKeyInfo;
    std::vector<uint8_t> ResponderSpecificSubSessionKeyInfo;
    std::vector<uint8_t> SusAdditionalParameters;
};
} // namespace nearobject::protocol::fira

#endif // FIRA_SECURE_RANGING_INFO_HXX
