
#include <iomanip>
#include <sstream>
#include <uwb/protocols/fira/StaticRangingInfo.hxx>

std::string
uwb::protocol::fira::StaticRangingInfo::ToString() const
{
    std::ostringstream ss;
    ss << "{" << std::endl;
    ss << "VendorId: " << VendorId << std::endl;
    ss << "InitializationVector: " << std::hex << std::setw(2) << std::setfill('0') << std::showbase << std::internal;
    for (const auto i : InitializationVector) {
        ss << +i << " ";
    }
    ss << std::endl;
    ss << "}" << std::endl;
    return ss.str();
}