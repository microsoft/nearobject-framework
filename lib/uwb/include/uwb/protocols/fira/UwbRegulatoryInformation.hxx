
#ifndef UWB_REGULATORY_INFORMATION_HXX
#define UWB_REGULATORY_INFORMATION_HXX

#include <bitset>
#include <cstdint>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <tlv/TlvBer.hxx>

namespace uwb::protocol::fira
{
struct UwbRegulatoryInformation
{
    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 101-102.
     */
    static constexpr uint8_t Tag = 0xA6;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 102-103.
     */
    enum class ParameterTag : uint8_t {
        InformationSource = 0x80,
        OutdoorPermitted = 0x81,
        CountryCode = 0x82,
        Timestamp = 0x83,
        Channel5 = 0x84,
        Channel6 = 0x85,
        Channel7 = 0x86,
        Channel8 = 0x87,
        Channel9 = 0x88,
        Channel10 = 0x89,
        Channel11 = 0x8A,
        Channel12 = 0x8B,
        Channel13 = 0x8C,
        Channel14 = 0x8D,
    };

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * page 102.
     */
    enum class InformationSource {
        UserDefined,
        Satellite,
        Cellular,
        OtherFiraDevice,
    };

    /**
     * @brief Convert this object into a FiRa Data Object (DO).
     *
     * @return std::unique_ptr<encoding::TlvBer>
     */
    std::unique_ptr<encoding::TlvBer>
    ToDataObject() const;

    /**
     * @brief Attempt to create a UwbRegulatoryInformation object from a TlvBer.
     * 
     * @param tlv 
     * @return UwbRegulatoryInformation 
     */
    static UwbRegulatoryInformation
    FromDataObject(const encoding::TlvBer& tlv);

    InformationSource InformationSource{ InformationSource::UserDefined };
    bool OutdoorPermitted{ true };
    uint16_t CountryCode{ 0x0000U };
    uint32_t Timestamp{ 0x00000000U };
    std::unordered_map<Channel, uint8_t> MaximumTransmissionPower;
};
} // namespace uwb::protocol::fira

#endif // UWB_REGULATORY_INFORMATION_HXX
