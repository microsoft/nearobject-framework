
#ifndef __SMARTCARD_HXX__
#define __SMARTCARD_HXX__

#include <chrono>
#include <future>
#include <vector>

#include "Apdu.hxx"

namespace smartcard
{
class Smartcard
{
public:
    virtual ~Smartcard() = default;

    /**
     * @brief Synchronously transmits a command apdu and provides the response.
     * This must handle any fragmentation of both transmitting the command and
     * receiving the response.
     *
     * @param command The command apdu to send.
     * @param response The response apdu to populate
     * @param timeout The timeout period after which the tranmission should be canceled.
     */
    void
    Transmit(const ApduCommand& command, ApduResponse& response, std::chrono::milliseconds timeout);

    /**
     * @brief Asynchronous transmission request.
     */
    struct TransmitAsyncRequest {
        // TODO: should this indicate why submission failed? (ie. a status instead of bool?)
        bool Submitted;
        std::future<ApduResponse> Response;
    };

    /**
     * @brief Asynchronously transmits a command apdu and provides a future for
     * the response.
     * 
     * @param command The command apdu to send.
     * @param timeout The timeout period after which the tranmission should be canceled.
     * @return std::future<smartcard::ApduResponse> 
     */
    virtual TransmitAsyncRequest
    TransmitAsync(const ApduCommand& command, std::chrono::milliseconds timeout);

private:
    virtual void
    TransmitImpl(const ApduCommand& command, ApduResponse& response, std::chrono::milliseconds timeout) = 0;

    virtual TransmitAsyncRequest
    TransmitAsyncImpl(const ApduCommand& command, std::chrono::milliseconds timeout) = 0;
};

} // namespace smartcard

#endif // __SMARTCARD_HXX__
