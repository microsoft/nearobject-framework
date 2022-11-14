
#ifndef FIRA_CONTROLEE_PREFERENCE_HXX
#define FIRA_CONTROLEE_PREFERENCE_HXX

#include <array>
#include <bitset>
#include <cstdint>
#include <vector>

#include <smartcard/Apdu.hxx>
#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief See FiRa Consortium Common Service Management Layer Technical
 * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
 * pages 99-101.
 * 
 * TODO: many public members need to be converted from bitsets to types with
 * more semantic information.
 */
struct ControleePreference
{
    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 99-101.
     */
    static constexpr uint8_t Tag = 0xA7;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 99-101.
     */
    enum class ParameterTag : uint8_t {
        DeviceRoles = 0x82,
        RangingMethod = 0x83,
        StsConfig = 0x84,
        MultiNodeMode = 0x85,
        RangingMode = 0x86,
        ScheduledMode = 0x87,
        HoppingMode = 0x88,
        BlockStriding = 0x89,
        UwbInitiationTime = 0x8A,
        Channels = 0x8B,
        RFrameConfig = 0x8C,
        CcConstraintLength = 0x8D,
        BprfParameterSets = 0x8E,
        HprfParameterSets = 0x8F,
        AoASupport = 0x90,
        ExtendedMacAddress = 0x91,
    };

    std::vector<uwb::protocol::fira::DeviceRole> Roles;
    bool RangingMethodOneWay;
    bool RangingMethodSsTwrDeferredMode;
    bool RangingMethodDsTwrDeferredMode;
    bool RangingMethodSsTwrNonDeferredMode;
    bool RangingMethodDsTwrNonDeferredMode;
    std::bitset<3> StsConfig;
    std::vector<uwb::protocol::fira::MultiNodeMode> MultiNodeModes;
    std::bitset<2> RangingMode;
    bool ScheduledMode{ false };
    bool HoppingMode{ false };
    bool BlockStriding{ false };
    uint32_t UwbInitiationTime;
    std::bitset<8> Channels;
    std::bitset<4> RFrameConfiguration;
    uint8_t ConvolutionalCodeConstraintLength;
    std::bitset<6> BprfParameterSets;
    std::bitset<35> HprfParameterSets;
    std::bitset<4> AoaSupport;
    uwb::UwbMacAddress ExtendedMacAddress;

    /**
     * @brief Converts this structure to an APDU suitable for use in OOB exchanges.
     * 
     * @return smartcard::ApduCommand 
     */
    smartcard::ApduCommand
    ToApdu() const;

    /**
     * @brief Creates a ControleePreference by parsing information from an APDU
     * describing a UWB_CONTROLEE_PREFERENCE FiRa OOB data object.
     * 
     * TODO: add spec reference foor UWB_CONTROLEE_PREFERENCE
     * 
     * @return ControleePreference 
     */
    static ControleePreference
    FromApdu();
};
} // namespace uwb::protocol::fira

#endif // FIRA_CONTROLEE_PREFERENCE_HXX
