
#include <uwb/protocols/fira/FiraDevice.hxx>

using namespace uwb::protocol::fira;

std::ostream&
uwb::protocol::fira::operator<<(std::ostream& os, const DeviceRole& deviceRole)
{
    printf("hello!\n");
    switch (deviceRole) {
    case DeviceRole::Initiator:
        os << "Initiator";
        break;
    case DeviceRole::Responder:
        os << "Responder";
        break;
    }
    return os;
}
