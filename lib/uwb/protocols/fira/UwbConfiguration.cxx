
#include <stdexcept>

#include <uwb/protocols/fira/UwbConfiguration.hxx>

using namespace uwb::protocol::fira;

/* static */
const std::unordered_set<ResultReportConfiguration> UwbConfiguration::ResultReportConfigurationsDefault = {
    ResultReportConfiguration::AoAAzimuthReport
};

std::unique_ptr<encoding::TlvBer>
UwbConfiguration::ToDataObject() const
{
    throw std::logic_error("not implemented");
}

/* static */
UwbConfiguration
UwbConfiguration::FromDataObject(const encoding::TlvBer& /* tlv */)
{
    throw std::logic_error("not implemented");
}