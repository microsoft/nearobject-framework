
#ifndef FIRA_RANGING_CONFIGURATION_HXX
#define FIRA_RANGING_CONFIGURATION_HXX

#include <functional>

#include <notstd/hash.hxx>
#include <notstd/utility.hxx>

#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb::protocol::fira
{
/**
 * @brief Effective ranging configuration.
 */
struct RangingMethod
{
    constexpr RangingMethod() = default;

    constexpr RangingMethod(RangingDirection method, MeasurementReportMode reportMode) :
        Method(method),
        ReportMode(reportMode)
    {}

    RangingDirection Method{ RangingDirection::OneWay };
    MeasurementReportMode ReportMode{ MeasurementReportMode::None };

    auto
    operator<=>(const RangingMethod& other) const = default;

    uint8_t
    ToByte();
};

} // namespace uwb::protocol::fira

namespace std
{
template <>
struct hash<uwb::protocol::fira::RangingMethod>
{
    std::size_t
    operator()(const uwb::protocol::fira::RangingMethod& rangingMethod) const noexcept
    {
        std::size_t value = 0;
        notstd::hash_combine(value, notstd::to_underlying(rangingMethod.Method), notstd::to_underlying(rangingMethod.ReportMode));
        return value;
    }
};
} // namespace std

#endif // FIRA_RANGING_CONFIGURATION_HXX