
#ifndef __SMARTCARD_HXX__
#define __SMARTCARD_HXX__

#include <chrono>
#include <future>
#include <vector>

#include "Apdu.hxx"

namespace smartcard
{

class ISmartcard
{
public:
    /**
     * @brief Synchronously transmits a command apdu and provides the response.
     * This must handle any fragmentation of both transmitting the command and
     * receiving the response.
     *
     * @param command The command apdu to send.
     * @param response The response apdu to populate
     * @param timeout The timeout period after which the tranmission should be canceled.
     */
    virtual void
    Transmit(const ApduCommand& command, ApduResponse& response, std::chrono::milliseconds timeout) = 0;

    /**
     * @brief Asynchronously transmits a command apdu and provides a future for
     * the response.
     * 
     * @param command The command apdu to send.
     * @param timeout The timeout period after which the tranmission should be canceled.
     * @return std::future<smartcard::ApduResponse> 
     */
    virtual std::future<ApduResponse>
    TransmitAsync(const ApduCommand& command, std::chrono::milliseconds timeout) = 0;
};

} // namespace smartcard

#endif // __SMARTCARD_HXX__
