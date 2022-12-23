
#ifndef UWB_APP_CONFIGURATION_HXX
#define UWB_APP_CONFIGURATION_HXX

#include <cstddef>
#include <cstdint>
#include <memory>
#include <tuple>

#include <UwbCxLrpDeviceGlue.h>

namespace windows::devices
{
template <typename PropertyT>
class UwbAppConfigurationParameter
{
public:
    UwbAppConfigurationParameter(const PropertyT& value, size_t parameterSize = sizeof(PropertyT)) : 
        m_size(offsetof(UWB_APP_CONFIG_PARAM, paramValue[sizeof(PropertyT)])),
        m_buffer(std::make_unique<uint8_t[]>(m_size)),
        m_parameter(*reinterpret_cast<UWB_APP_CONFIG_PARAM*>(m_buffer.get())),
        m_value(reinterpret_cast<PropertyT&>(m_parameter.paramValue))
    {
        m_parameter.size = m_size;
        m_parameter.paramLength = parameterSize;
        m_value = value;
    } 

    std::size_t
    Size() noexcept
    {
        return m_size; 
    } 

    uint8_t*
    Buffer() noexcept 
    {
        return m_buffer;
    } 
   
    PropertyT& 
    Value() noexcept
    {
        return m_value; 
    } 

    UWB_APP_CONFIG_PARAM& 
    DdiParameter() noexcept
    {
        return m_parameter; 
    } 

    std::tuple<uint8_t*, std::size_t>
    GetIoctlBuffer() noexcept
    {
        return { m_buffer, m_size }; 
    }

 private: 
    // order of members here is important to enforce proper initialization order 
    std::size_t m_size;
    std::unique_ptr<uint8_t[]> m_buffer; 
    UWB_APP_CONFIG_PARAM &m_parameter; 
    PropertyT& m_value;
};
} // namespace windows::devices

#endif // UWB_APP_CONFIGURATION_HXX
