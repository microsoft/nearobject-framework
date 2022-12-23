
#ifndef UWB_APP_CONFIGURATION_HXX
#define UWB_APP_CONFIGURATION_HXX

#include <cstddef>
#include <cstdint>
#include <memory>
#include <tuple>

#include <UwbCxLrpDeviceGlue.h>

namespace windows::devices
{
template <typename T>
class UwbAppConfigurationParameter
{
    UwbAppConfigurationParameter(const T& value) : 
        m_size(offsetof(UWB_APP_CONFIG_PARAM), paramValue[sizeof(T)]),
        m_buffer(std::make_unique<uint8_t[]>(m_parameter.size),
        m_parameter(*reinterpret_cast<UWB_APP_CONFIG_PARAM*>(m_buffer.get())),
        m_parameter.size(m_size),
        m_parameter.paramLength = sizeof(T),
        m_value(reinterpret_cast<T&>(m_parameter.paramValue))
    {
        m_value = value; // set value contents 
    } 

    auto Size() {
        return m_size; } 
    uint8_t* Buffer() {
        return m_buffer; } 
    T& Value() {
        return m_value; } 
    UWB_APP_CONFIG_PARAM& Parameter() {
        m_parameter; } 
    std::tuple<uint8_t*, std::size_t> GetIoctlBuffer() {
        return { m_buffer, m_size }; }

 private: 
    // order of members here is intentional, to enforce proper initialization order 
    std::size_t m_size;
    std::unique_ptr<uint8_t[]> m_buffer; 
    UWB_APP_CONFIG_PARAM &m_parameter; 
    T& m_value;
};
} // namespace windows::devices

#endif // UWB_APP_CONFIGURATION_HXX
