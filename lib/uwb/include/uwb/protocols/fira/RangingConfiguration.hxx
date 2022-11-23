
#ifndef FIRA_RANGING_CONFIGURATION_HXX
#define FIRA_RANGING_CONFIGURATION_HXX

#include <functional>

#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Effective ranging configuration.
 */
struct RangingConfiguration
{
    constexpr RangingConfiguration(RangingMethod method, MeasurementReportMode reportMode) :
        Method(method),
        ReportMode(reportMode)
    {}

    RangingMethod Method;
    MeasurementReportMode ReportMode;
};

bool
operator==(const RangingConfiguration& lhs, const RangingConfiguration& rhs) noexcept;

bool
operator!=(const RangingConfiguration& lhs, const RangingConfiguration& rhs) noexcept;

} // namespace uwb::protocol::fira

namespace std
{
template <>
struct hash<uwb::protocol::fira::RangingConfiguration>
{
    // TODO replace this hash function with something actually usable
    size_t
    operator()(const uwb::protocol::fira::RangingConfiguration& rangingConfiguration) const noexcept
    {
        return (std::size_t)&rangingConfiguration;
    }
};
} // namespace std

#endif // FIRA_RANGING_CONFIGURATION_HXX