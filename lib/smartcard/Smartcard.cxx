
#include <smartcard/Smartcard.hxx>

using namespace smartcard;

void
Smartcard::Transmit(const ApduCommand& command, ApduResponse& response, std::chrono::milliseconds timeout)
{
    return TransmitImpl(command, response, timeout);
}

Smartcard::TransmitAsyncRequest
Smartcard::TransmitAsync(const ApduCommand& command, std::chrono::milliseconds timeout)
{
    return TransmitAsyncImpl(command, timeout);
}

void
Smartcard::TransmitImpl(const ApduCommand& /* command */, ApduResponse& /* response */, std::chrono::milliseconds /* timeout */)
{
}

Smartcard::TransmitAsyncRequest
Smartcard::TransmitAsyncImpl(const ApduCommand& /* command */, std::chrono::milliseconds /* timeout */)
{
    return { false, {} };
}
