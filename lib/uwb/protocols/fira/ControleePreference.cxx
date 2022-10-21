
#include <uwb/protocols/fira/ControleePreference.hxx>

using namespace uwb::protocol::fira;

smartcard::ApduCommand
ControleePreference::ToApdu() const
{
    smartcard::ApduCommand apdu{std::byte(0),std::byte(0),std::byte(0),std::byte(0)};
    // TODO: perform real conversion
    return apdu;
}

/* static */ 
ControleePreference 
ControleePreference::FromApdu()
{
    ControleePreference controlleePreference{};
    // TODO: perform real parsing and populate controleePreference
    return controlleePreference;
}
