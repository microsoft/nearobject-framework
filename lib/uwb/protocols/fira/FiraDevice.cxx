
#include <uwb/protocols/fira/FiraDevice.hxx>

using namespace uwb::protocol::fira;

// std::istream&
// uwb::protocol::fira::operator>>(std::istream& is, DeviceRole& deviceRole)
// {
//     printf("custom >>\n");
//     char c;
//     is >> c;
//     if (c == 'R')
//         deviceRole = DeviceRole::Responder;
//     else
//         deviceRole = DeviceRole::Initiator;

//     return is;
// }

// bool
// lexical_cast(const std::string& input, DeviceRole& dr)
// {
//     std::cout << "called correct lexical_cast function ! val: " << input << std::endl;
//     if (input == "Responder") {
//         dr = DeviceRole::Responder;
//         return true;
//     } else if (input == "Initiator") {
//         dr = DeviceRole::Initiator;
//         return true;
//     }
//     return false;
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
