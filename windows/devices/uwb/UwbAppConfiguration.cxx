
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <numeric>

#include <windows/devices/uwb/UwbAppConfiguration.hxx>

bool
operator==(const UWB_APP_CONFIG_PARAM& lhs, const UWB_APP_CONFIG_PARAM& rhs) noexcept
{
    return (lhs.size == rhs.size) && (std::memcmp(&lhs, &rhs, lhs.size) == 0);
}

using namespace windows::devices::uwb;

IUwbAppConfigurationParameter::IUwbAppConfigurationParameter(UWB_APP_CONFIG_PARAM_TYPE parameterType, std::span<const uint8_t> parameterValue) :
    m_buffer(offsetof(UWB_APP_CONFIG_PARAM, paramValue[parameterValue.size()])),
    m_parameter(*reinterpret_cast<UWB_APP_CONFIG_PARAM*>(m_buffer.data()))
{
    m_parameter.size = std::size(m_buffer);
    m_parameter.paramType = parameterType;
    m_parameter.paramLength = parameterValue.size();
    std::memcpy(&m_parameter.paramValue, parameterValue.data(), parameterValue.size());
}

std::size_t
IUwbAppConfigurationParameter::DdiSize() const noexcept
{
    return std::size(m_buffer);
}

UWB_APP_CONFIG_PARAM&
IUwbAppConfigurationParameter::DdiParameter() noexcept
{
    return m_parameter;
}

std::vector<uint8_t>&
IUwbAppConfigurationParameter::DdiBuffer() noexcept
{
    return m_buffer;
}

namespace detail
{
/**
 * @brief Calculate the total size in bytes of all the UWB_APP_CONFIG_PARAM DDI
 * structures contained in the provided IUwbAppConfigurationParameter vector.
 *
 * @param parameters
 * @return std::size_t
 */
std::size_t
CalculateTotalUwbAppConfigurationBufferSize(const std::vector<std::unique_ptr<IUwbAppConfigurationParameter>>& parameters)
{
    return std::accumulate(parameters.cbegin(), parameters.cend(), static_cast<std::size_t>(0), [&](std::size_t totalSize, const auto& uwbAppConfigurationParameter) {
        return totalSize + uwbAppConfigurationParameter->DdiSize();
    });
}
} // namespace detail

UwbSetAppConfigurationParameters::UwbSetAppConfigurationParameters(const std::vector<std::unique_ptr<IUwbAppConfigurationParameter>>& parameters, uint32_t sessionId) :
    m_buffer(offsetof(UWB_APP_CONFIG_PARAMS, appConfigParams[0]) + ::detail::CalculateTotalUwbAppConfigurationBufferSize(parameters)),
    m_parameters(*reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS*>(m_buffer.data()))
{
    auto it = std::next(m_buffer.begin(), offsetof(UWB_APP_CONFIG_PARAMS, appConfigParams[0]));

    // Copy each complete UWB_APP_CONFIG_PARAM structure into the byte buffer.
    for (const auto& parameter : parameters) {
        const auto& parameterBuffer = parameter->DdiBuffer();
        it = std::transform(std::cbegin(parameterBuffer), std::cend(parameterBuffer), it, [](const uint8_t byte) {
            return byte;
        });
    }

    m_parameters.appConfigParamsCount = std::size(parameters);
    m_parameters.size = std::size(m_buffer);
    m_parameters.sessionId = sessionId;
}

UWB_SET_APP_CONFIG_PARAMS&
UwbSetAppConfigurationParameters::DdiParameters() noexcept
{
    return m_parameters;
}

std::vector<uint8_t>&
UwbSetAppConfigurationParameters::DdiBuffer() noexcept
{
    return m_buffer;
}
