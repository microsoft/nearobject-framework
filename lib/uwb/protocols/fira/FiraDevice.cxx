
#include <uwb/protocols/fira/FiraDevice.hxx>

using namespace uwb::protocol::fira;

// std::istream&
// uwb::protocol::fira::operator>>(std::istream& is, DeviceRole& deviceRole) noexcept
// {
//     printf("custom >>\n");
//     std::string s;
//     is >> s;
//     if (s == "Responder")
//         deviceRole = DeviceRole::Responder;
//     else
//         deviceRole = DeviceRole::Initiator;

//     return is;
// }

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
