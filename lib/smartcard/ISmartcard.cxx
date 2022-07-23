
#include "ISmartcard.hxx"

using namespace smartcard;

void
ISmartcard::Transmit(const ApduCommand& command, ApduResponse& response, std::chrono::milliseconds timeout)
{
    return TransmitImpl(command, response, timeout);
}

ISmartcard::TransmitAsyncRequest
ISmartcard::TransmitAsync(const ApduCommand& command, std::chrono::milliseconds timeout)
{
    return TransmitAsync(command, timeout);
}

void
ISmartcard::TransmitImpl(const ApduCommand& command, ApduResponse& response, std::chrono::milliseconds timeout)
{
}

ISmartcard::TransmitAsyncRequest
ISmartcard::TransmitAsyncImpl(const ApduCommand& command, std::chrono::milliseconds timeout)
{
    return { false, {} };
}