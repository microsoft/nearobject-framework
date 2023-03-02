
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
    ss << Major << '.' << Minor << '.' << Maintenance;
    return ss.str();
}

/* static */
std::optional<UwbVersion>
UwbVersion::Parse(const std::string& /* str */) noexcept
{
    // TODO: implement me
    return std::nullopt;
}
