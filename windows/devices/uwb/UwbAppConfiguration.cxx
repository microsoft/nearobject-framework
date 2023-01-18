
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <tuple>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

bool
operator==(const UWB_APP_CONFIG_PARAM& lhs, const UWB_APP_CONFIG_PARAM& rhs) noexcept
{
    return (lhs.size == rhs.size) && (std::memcmp(&lhs, &rhs, lhs.size) == 0);
}

using namespace windows::devices;

IUwbAppConfigurationParameter::IUwbAppConfigurationParameter(UWB_APP_CONFIG_PARAM_TYPE parameterType, std::span<const uint8_t> parameterValue) :
    m_buffer(offsetof(UWB_APP_CONFIG_PARAM, paramValue[parameterValue.size()])),
    m_parameter(*reinterpret_cast<UWB_APP_CONFIG_PARAM*>(m_buffer.data()))
{
    m_parameter.size = std::size(m_buffer);
    m_parameter.paramType = parameterType;
    m_parameter.paramLength = parameterValue.size();
    std::memcpy(&m_parameter.paramValue, parameterValue.data(), parameterValue.size());
}

UWB_APP_CONFIG_PARAM&
IUwbAppConfigurationParameter::DdiParameter() noexcept
{
    return m_parameter;
}

const std::vector<uint8_t>&
IUwbAppConfigurationParameter::DdiBuffer() noexcept
{
    return m_buffer;
}

UwbSetAppConfigurationParametersBuilder::UwbSetAppConfigurationParametersBuilder(uint32_t sessionId) :
    m_sessionId{ sessionId }
{}

std::unique_ptr<uint8_t[]>
UwbSetAppConfigurationParametersBuilder::Publish()
{
    auto desiredSize = offsetof(UWB_SET_APP_CONFIG_PARAMS, appConfigParams[0]) + m_paramsLengthSum;
    auto buffer = std::make_unique<uint8_t[]>(desiredSize);
    UWB_SET_APP_CONFIG_PARAMS& setParams = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS*>(buffer.get());

    setParams.size = desiredSize;
    setParams.sessionId = m_sessionId;
    setParams.appConfigParamsCount = m_params.size();

    uint8_t* dstBuffer = reinterpret_cast<uint8_t*>(setParams.appConfigParams);

    for (const auto& paramBuffer : m_params) {
        auto& param = *reinterpret_cast<UWB_APP_CONFIG_PARAM*>(paramBuffer.get());
        std::memcpy(dstBuffer, paramBuffer.get(), param.size);
        dstBuffer = &dstBuffer[param.size];
    }
    return buffer;
}