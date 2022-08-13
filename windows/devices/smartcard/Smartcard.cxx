
#include <windows/smartcard/Smartcard.hxx>

#include <stdexcept>

using namespace windows::devices;

void
Smartcard::TransmitImpl(const smartcard::ApduCommand& command, smartcard::ApduResponse& response, std::chrono::milliseconds timeout)
{
    // TODO: Send IOCTL_SMARTCARD_TRANSMIT to underlying device and receive response.
    throw std::logic_error("not implemented");
}

smartcard::Smartcard::TransmitAsyncRequest
Smartcard::TransmitAsyncImpl(const smartcard::ApduCommand& command, std::chrono::milliseconds timeout)
{
    static constexpr bool SubmissionSucceeded = true;
    static constexpr bool SubmissionFailed = false;

    const auto pendingTransmitLock = std::scoped_lock(m_pendingTxGate);
    if (m_pendingTxResponsePromise.has_value()) {
        // only a single asynchronous transmission is currently allowed, bail out
        return { SubmissionFailed, {} };
    }

    // TODO: Set up asynchronous i/o with DeviceIoControl() and start it

    // Create a new promise that will signal the response is available.
    m_pendingTxResponsePromise.emplace();
    return { SubmissionSucceeded, m_pendingTxResponsePromise->get_future() };
}

void
Smartcard::CompletePendingTransmission(smartcard::ApduResponse response)
{
    const auto pendingTransmitLock = std::scoped_lock(m_pendingTxGate);
    if (!m_pendingTxResponsePromise.has_value()) {
        return;
    }

    m_pendingTxResponsePromise->set_value(std::move(response));
    m_pendingTxResponsePromise.reset();
}
