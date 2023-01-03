
#ifndef WINDOWS_UWB_CX_ADAPTER_HXX
#define WINDOWS_UWB_CX_ADAPTER_HXX

#include <cstdint>

// NB: This must come before any other Windows include
#include <UwbCxLrpDeviceGlue.h>
#include <wil/resource.h>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>

namespace windows::devices::uwb::detail
{
/**
 * @brief Maps the param to the length of the value. If the value is 0, means a variable length and special code to handle
 * Taken from the FiRa UCI spec Table 29:APP Configuration IDs
 * 
 */
const
std::unordered_map<UWB_APP_CONFIG_PARAM_TYPE,unsigned int> appConfigParamLengths {
    {UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER,1},
    {UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES,1},
    {UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS,0},
    {UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS,0},
    {UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION,2},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL,4},
    {UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX,4},
    {UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL,1},
    {UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR,2},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR,2},
    {UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SFD_ID,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR,1},
    {UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX,1},
    {UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION,1},
    {UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID,2},
    {UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV,6},
    {UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY,2},
    {UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME,4},
    {UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH,1},
    {UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG,1},
    {UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT,1},
    {UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID,4},
    {UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE,1},
    {UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS,2},
    {UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH,1}
};

const
std::unordered_map<
    ::uwb::protocol::fira::UwbConfiguration::ParameterTag,
    UWB_APP_CONFIG_PARAM_TYPE>
    service_ddi_map{
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::DeviceRole, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RangingMethod, UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::StsConfig, UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MultiNodeMode, UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RangingTimeStruct, UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ScheduledMode, UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::HoppingMode, UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::BlockStriding, UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH }, // TODO the stride length needs to be decided somewhere
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::UwbInitiationTime, UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ChannelNumber, UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RFrameConfig, UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::CcConstraintLength, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO ccclength is related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::PrfMode, UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::Sp0PhySetNumber, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::Sp1PhySetNumber, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::Sp3PhySetNumber, UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE }, // TODO related but not directly the psdu data rate
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::PreableCodeIndex, UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ResultReportConfig, UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MacAddressMode, UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControleeShortMacAddress, UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::ControllerMacAddress, UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS }, // TODO this field only applies when the host is the controller
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::SlotsPerRr, UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MaxContentionPhaseLength, UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR }, // TODO figure out this field
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::SlotDuration, UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::RangingInterval, UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::KeyRotationRate, UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MacFcsType, UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE },
        { ::uwb::protocol::fira::UwbConfiguration::ParameterTag::MaxRrRetry, UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY },
    };

std::unique_ptr<uint8_t[]>
GenerateUwbSetAppConfigParameterDdi(const ::uwb::protocol::fira::UwbSessionData& uwbSessionData);
} // namespace windows::devices::uwb::detail

#endif // WINDOWS_UWB_CX_ADAPTER_HXX
