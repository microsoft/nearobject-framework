
#include <sstream>

#include <uwb/UwbVersion.hxx>

using namespace uwb;

UwbVersion::operator uint32_t() const noexcept
{
    return Major << 24U | (Minor | Maintenance);
}

std::string
UwbVersion::ToString() const
{
    std::ostringstream ss;
    ss << +Major << '.' << +Minor << '.' << +Maintenance;
    return ss.str();
}

/* static */
std::optional<UwbVersion>
UwbVersion::Parse(const std::string& /* str */) noexcept
{
    // TODO: implement me
    return std::nullopt;
}

/* static */
UwbVersion
UwbVersion::FromUci(uint8_t major, uint8_t minorAndMaintenance)
{
    static constexpr uint8_t MaskMinor = 0b11110000;
    static constexpr uint8_t MaskMaintenance = 0b00001111U;
    static constexpr uint8_t ShiftMinor = 4U;
    static constexpr uint8_t ShiftMaintenance = 0U;

    return UwbVersion{
        .Major = major,
        .Minor = static_cast<uint8_t>((minorAndMaintenance & MaskMinor) >> ShiftMinor),
        .Maintenance = static_cast<uint8_t>((minorAndMaintenance & MaskMaintenance) >> ShiftMaintenance),
    };
}
