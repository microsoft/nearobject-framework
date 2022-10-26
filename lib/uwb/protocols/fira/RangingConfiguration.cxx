
#include <tuple>

#include <uwb/protocols/fira/RangingConfiguration.hxx>

using namespace uwb::protocol::fira;

bool
uwb::protocol::fira::operator==(const RangingConfiguration& lhs, const RangingConfiguration& rhs) noexcept
{
    return std::tie(lhs.Method, lhs.ReportMode) 
        == std::tie(rhs.Method, rhs.ReportMode);
}

bool
uwb::protocol::fira::operator!=(const RangingConfiguration& lhs, const RangingConfiguration& rhs) noexcept
{
    return !(lhs == rhs);
}
