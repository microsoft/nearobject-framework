
#ifndef UWB_APP_CONFIGURATION_HXX
#define UWB_APP_CONFIGURATION_HXX

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <tuple>
#include <vector>

#include <UwbCxLrpDeviceGlue.h>

namespace windows::devices
{
class UwbSetAppConfigurationParametersBuilder;
/**
 * @brief Adaptor for using UWB_APP_CONFIG_PARAM UwbCx DDI structure in a nicer way.
 *
 * @tparam PropertyT The type of property the UWB_APP_CONFIG_PARAM structure
 * holds as its flexible array member.
 */
template <typename PropertyT>
class UwbAppConfigurationParameter
{
    friend class UwbSetAppConfigurationParametersBuilder; // friend class so the unique buffer ptr can be moved out
public:
    explicit UwbAppConfigurationParameter(const PropertyT& value, UWB_APP_CONFIG_PARAM_TYPE parameterType, size_t parameterSize = sizeof(PropertyT)) :
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
     * Push a UwbAppConfigurationParameter into the collection
    */
    template <typename PropertyT>
    void
    Push(UwbAppConfigurationParameter<PropertyT> param){
       m_params.push_back(std::move(param.m_buffer));
       m_paramsLengthSum += param.Size();
    }

    /**
     * Writes all the params and returns a buffer that can be reinterpreted as a UWB_SET_APP_CONFIG_PARAMS
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
operator==(const UWB_APP_CONFIG_PARAM& lhs, const UWB_APP_CONFIG_PARAM& rhs) noexcept
{
    return (lhs.size == rhs.size) && (std::memcmp(&lhs, &rhs, lhs.size) == 0);
}

#endif // UWB_APP_CONFIGURATION_HXX
