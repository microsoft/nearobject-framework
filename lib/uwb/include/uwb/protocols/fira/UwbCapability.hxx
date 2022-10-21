
#ifndef FIRA_UWB_CAPABILITY_HXX
#define FIRA_UWB_CAPABILITY_HXX

#include <cstdint>

namespace uwb::protocol::fira
{
struct UwbCapability
{
    // TODO: add public fields for each parameter defined below

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 96-99.
     */
    static constexpr uint8_t Tag = 0xA3;

    /**
     * @brief See FiRa Consortium Common Service Management Layer Technical
     * Specification v1.0.0, Section 7.5.3.2, 'UWB Controlee Info', Table 52,
     * pages 96-99.
     */
    enum class ParameterTag : uint8_t
    {
        FiraPhyVersion = 0x80,
        FiraMacVersion = 0x81,
        DeviceRole = 0x82,
        RangingMethod = 0x83,
        StsConfig = 0x84,
        MultiNodeMode = 0x85,
        RangingTimeStruct = 0x86,
        ScheduledMode = 0x87,
        HoppingMode = 0x88,
        BlockStriding = 0x89,
        UwbInitiationTime = 0x8A,
        ChannelNumber = 0x8B,
        RFrameConfig = 0x8C,
        CcConstraintLength = 0x8D,
        PrfMode = 0x8E,
        Sp0PhySetNumber = 0x8F,
        Sp1PhySetNumber = 0x90,
        Sp3PhySetNumber = 0x91,
        PreableCodeIndex = 0x92,
        ResultReportConfig = 0x93,
        MacAddressMode = 0x94,
        ControleeShortMacAddress = 0x95,
        ControllerMacAddress = 0x96,
        SlotsPerRr = 0x97,
        MaxContentionPhaseLength = 0x98,
        SlotDuration = 0x99,
        RangingInterval = 0x9A,
        KeyRotationRate = 0x9B,
        MacFcsType = 0x9C,
        MaxRrRetry = 0x9D,
    };
};
} // namespace uwb::protocol::fira

#endif // FIRA_UWB_CAPABILITY_HXX
