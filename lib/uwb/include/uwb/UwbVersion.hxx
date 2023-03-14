
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
     * @brief Returns a string representation of the version number.
     *
     * @return std::string
     */
    std::string
    ToString() const;

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

    /**
     * @brief Create an instance from a pari of UCI encoded version values. 
     * 
     * This assumes the minor and maintenance value are encoded per FiRa UCI
     * Specification v1.1.0, page 18, Table 5: 'Control Messages to get the
     * Device Information'.
     *
     * @param major The major version value.
     * @param minorAndMaintenance The minor and maintenance version values.
     * @return UwbVersion 
     */
    static UwbVersion
    FromUci(uint8_t major, uint8_t minorAndMaintenance);
};
} // namespace uwb

#endif // UWB_VERSION_HXX
