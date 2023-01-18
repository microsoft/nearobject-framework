
#ifndef UWB_APP_CONFIGURATION_HXX
#define UWB_APP_CONFIGURATION_HXX

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <tuple>
#include <vector>
#include <type_traits>
#include <unordered_map>

#include <uwb/protocols/fira/UwbConfiguration.hxx>

#include <UwbCxLrpDeviceGlue.h>

namespace windows::devices
{
namespace detail
{
const std::unordered_map<::uwb::protocol::fira::UwbConfiguration::ParameterTag, UWB_APP_CONFIG_PARAM_TYPE> AppConfigUwbConfigurationTagMap {
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
} // namespace detail

class UwbSetAppConfigurationParametersBuilder;
/**
 * @brief Adaptor for using UWB_APP_CONFIG_PARAM UwbCx DDI structure in a nicer way.
 *
 * @tparam PropertyT The type of property the UWB_APP_CONFIG_PARAM structure
 * holds as its flexible array member.
 */
template <typename PropertyT>
requires std::is_standard_layout_v<PropertyT>
class UwbAppConfigurationParameter
{
    friend class UwbSetAppConfigurationParametersBuilder; // friend class so the unique buffer ptr can be moved out
public:
    /**
     * @brief Construct a new UwbAppConfigurationParameter object.
     * 
     * @param value The value to pack into the UWB_APP_CONFIG_PARAM paramValue flex-array member.
     * @param parameterType The corresponding UwbCx parameter type value.
     * @param parameterSize The size of the value to pack, in bytes.
     */
    explicit UwbAppConfigurationParameter(const PropertyT& value, UWB_APP_CONFIG_PARAM_TYPE parameterType, std::size_t parameterSize = sizeof(PropertyT)) :
        m_size(offsetof(UWB_APP_CONFIG_PARAM, paramValue[parameterSize])),
        m_buffer(std::make_unique<uint8_t[]>(m_size)),
        m_parameter(*reinterpret_cast<UWB_APP_CONFIG_PARAM*>(m_buffer.get())),
        m_value(reinterpret_cast<PropertyT&>(m_parameter.paramValue)) // TODO verify that this assignment of bytes indeed works for all the parameters (what if the enum is a strange number, like 5 choices or something)
    {
        m_parameter.size = m_size;
        m_parameter.paramType = parameterType;
        m_parameter.paramLength = parameterSize;
        m_value = value;
    }

    /**
     * @brief Construct a new Uwb App Configuration Parameter object
     * 
     * @param value The value to pack into the UWB_APP_CONFIG_PARAM paramValue flex-array member.
     * @param parameterTag The FiRa parameter tag for UWB_CONFIGURATION.
     * @param parameterSize The size of the value to pack, in bytes.
     */
    explicit UwbAppConfigurationParameter(const PropertyT& value, ::uwb::protocol::fira::UwbConfiguration::ParameterTag parameterTag, std::size_t parameterSize = sizeof(PropertyT)) :
        m_size(offsetof(UWB_APP_CONFIG_PARAM, paramValue[parameterSize])),
        m_buffer(std::make_unique<uint8_t[]>(m_size)),
        m_parameter(*reinterpret_cast<UWB_APP_CONFIG_PARAM*>(m_buffer.get())),
        m_value(reinterpret_cast<PropertyT&>(m_parameter.paramValue)) // TODO verify that this assignment of bytes indeed works for all the parameters (what if the enum is a strange number, like 5 choices or something)
    {
        m_parameter.size = m_size;
        m_parameter.paramType = detail::AppConfigUwbConfigurationTagMap.at(parameterTag);
        m_parameter.paramLength = parameterSize;
        m_value = value;
    }

    /**
     * @brief The total size of the UWB_APP_CONFIG_PARAM structure.
     *
     * @return std::size_t
     */
    std::size_t
    Size() noexcept
    {
        return m_size;
    }

    /**
     * @brief The buffer containing the complete UWB_APP_CONFIG_PARAM structure,
     * along with the trailing parameter value.
     *
     * @return uint8_t*
     */
    uint8_t*
    Buffer() noexcept
    {
        return m_buffer.get();
    }

    /**
     * @brief Return a typed reference to the flexible array member value.
     *
     * @return PropertyT&
     */
    PropertyT&
    Value() noexcept
    {
        return m_value;
    }

    /**
     * @brief Reference to the UwbCx DDI structure.
     *
     * @return UWB_APP_CONFIG_PARAM&
     */
    UWB_APP_CONFIG_PARAM&
    DdiParameter() noexcept
    {
        return m_parameter;
    }

    /**
     * @brief The buffer and associated size that is suitable for passing to the UwbCx DDI.
     *
     * @return std::tuple<uint8_t*, std::size_t>
     */
    std::tuple<uint8_t*, std::size_t>
    DdiBuffer() noexcept
    {
        return { m_buffer, m_size };
    }

private:
    // order of members here is important to enforce proper initialization order
    std::size_t m_size;
    std::unique_ptr<uint8_t[]> m_buffer;
    UWB_APP_CONFIG_PARAM& m_parameter;
    PropertyT& m_value;
};

/**
 * Builder class that takes in a collection of UwbAppConfigurationParameter and outputs a UWB_SET_APP_CONFIG_PARAMS
 */
class UwbSetAppConfigurationParametersBuilder
{
public:
    UwbSetAppConfigurationParametersBuilder(uint32_t sessionId);

    /**
     * @brief Push a UwbAppConfigurationParameter into the collection
     * 
     * @tparam PropertyT 
     * @param param 
     */
    template <typename PropertyT>
    void
    Push(UwbAppConfigurationParameter<PropertyT> param)
    {
        m_params.push_back(std::move(param.m_buffer));
        m_paramsLengthSum += param.Size();
    }

    /**
     * @brief Writes all the params and returns a buffer that can be reinterpreted as a UWB_SET_APP_CONFIG_PARAMS
     * 
     * @return std::unique_ptr<uint8_t[]> 
     */
    std::unique_ptr<uint8_t[]>
    Publish();

private:
    uint32_t m_sessionId;
    std::size_t m_paramsLengthSum;
    std::vector<std::unique_ptr<uint8_t[]>> m_params;
};
} // namespace windows::devices

bool
operator==(const UWB_APP_CONFIG_PARAM& lhs, const UWB_APP_CONFIG_PARAM& rhs) noexcept;

#endif // UWB_APP_CONFIGURATION_HXX
