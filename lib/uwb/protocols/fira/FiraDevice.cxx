
#include <uwb/protocols/fira/FiraDevice.hxx>

using namespace uwb::protocol::fira;

std::istream& uwb::protocol::fira::operator>>(std::istream& is, DeviceRole& deviceRole)
{
    char c;
    is >> c;
    if(c=='R') deviceRole = DeviceRole::Responder;
    else deviceRole = DeviceRole::Initiator;

    return is;
}

std::ostream& uwb::protocol::fira::operator<<(std::ostream& os, const DeviceRole& deviceRole){
    switch(deviceRole){
        case DeviceRole::Initiator: os << "Initiator"; break;
        case DeviceRole::Responder: os << "Responder"; break;
    }
    return os;
}
