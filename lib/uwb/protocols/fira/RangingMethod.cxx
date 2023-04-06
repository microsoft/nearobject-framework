
#include <stdexcept>
#include <uwb/protocols/fira/RangingMethod.hxx>
uint8_t
uwb::protocol::fira::RangingMethod::ToByte() const
{
    if (Method == RangingDirection::SingleSidedTwoWay and ReportMode == MeasurementReportMode::Deferred) {
        return 1;
    } else if (Method == RangingDirection::DoubleSidedTwoWay and ReportMode == MeasurementReportMode::Deferred) {
        return 2;
    } else if (Method == RangingDirection::SingleSidedTwoWay and ReportMode == MeasurementReportMode::NonDeferred) {
        return 3;
    } else if (Method == RangingDirection::DoubleSidedTwoWay and ReportMode == MeasurementReportMode::NonDeferred) {
        return 4;
    } else {
        throw std::runtime_error("invalid ranging method");
    }
}
