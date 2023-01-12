
#include <iomanip>
#include <sstream>
#include <uwb/protocols/fira/StaticRangingInfo.hxx>

std::string
uwb::protocol::fira::StaticRangingInfo::ToString() const
{
    std::ostringstream staticRangingInfoString{};
    staticRangingInfoString 
        << "VendorId " << VendorId
        << ", InitializationVector 0x";
    for (const auto& i : InitializationVector) {
        staticRangingInfoString << std::hex << std::setw(2) << std::setfill('0') << std::showbase << std::internal << +i;
    }

    return staticRangingInfoString.str();
}
