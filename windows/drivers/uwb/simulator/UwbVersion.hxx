
#ifndef UWB_VERSION_HXX
#define UWB_VERSION_HXX

#include <cstdint>
#include <stdexcept>
#include <string>

namespace windows::devices::uwb
{
struct Version
{
    uint8_t Major;
    uint8_t Minor;
    uint8_t Maintenance;

    operator uint32_t() const noexcept
    {
        return Major << 24U | (Minor | Maintenance);
    }

    static Version
    Parse(const std::string & /* str */)
    {
        throw std::runtime_error("not implemented");
    }
};
} // namespace windows::devices::uwb

#endif // UWB_VERSION_HXX
