
#ifndef UWB_VERSION_HXX
#define UWB_VERSION_HXX

#include <cstdint>
#include <optional>
#include <stdexcept>
#include <string>

namespace uwb
{
/**
 * @brief Abstraction of a version number commonly used to describe UWB
 * versions.
 */
struct UwbVersion
{
    uint8_t Major;
    uint8_t Minor;
    uint8_t Maintenance;

    /**
     * @brief Returns the aggregated version number as a 32-bit unsigned
     * integer.
     *
     * @return uint32_t
     */
    operator uint32_t() const noexcept;

    /**
     * @brief Parses the specified string and converts it to a UwbVersion
     * object, if valid.
     *
     * Valid string formats:
     *  X.Y
     *  X.Y.Z
     *
     * X -> version major number, up to 8-bits.
     * Y -> verison minor number, up to 8-bits.
     * Z -> version maintenance number, up to 8-bits.
     *
     * @param str The string to parse, containing a valid UwbVersion string.
     * @return std::optional<UwbVersion>
     */
    static std::optional<UwbVersion>
    Parse(const std::string& str) noexcept;
};
} // namespace uwb

#endif // UWB_VERSION_HXX
