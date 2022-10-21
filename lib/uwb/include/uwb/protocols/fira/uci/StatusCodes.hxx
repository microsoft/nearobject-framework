
#ifndef FIRA_UCI_STATUS_CODES_HXX
#define FIRA_UCI_STATUS_CODES_HXX

#include <cstdint>

namespace uwb::protocol::fira::uci
{
enum class StatusCode : uint8_t {
    // Generic Status Codes
    Ok = 0x00,
    Rejected = 0x01,
    Failed = 0x02,
    SyntaxError = 0x03,
    InvalidParameter = 0x04,
    InvalidRange = 0x05,
    InvalidMessageSize = 0x06,
    UnknownGid = 0x07,
    UnknownOid = 0x08,
    ReadOnly = 0x09,
    CommandRetry = 0x0A,
    // RFU = 0x0B-0x0F

    // UWB Session Specific Status Codes
    UwbSessionNotExist = 0x11,
    UwbSessionDuplicate = 0x12,
    UwbSessionActive = 0x13,
    UwbSessionMaxSessionsExceeded = 0x14,
    UwbSessionSessionNotConfigured = 0x15,
    UwbSessionActiveSessionsOngoing = 0x16,
    UwbSessionMulticastListFull = 0x17,
    UwbSessionAddressNotFound = 0x18,
    UwbSessionAddressAlreadyPresent = 0x19,
    // RFU = 0x20-0x1F

    // UWB Ranging Session Specific Status Codes
    RangingTxFailed = 0x20,
    RangingRxTimeout = 0x21,
    RangingRxPhyDecodingFailed = 0x22,
    RangingRxPhyToaFailed = 0x23,
    RangingRxPhyStsFailed = 0x24,
    RangingRxMacDecodingFailed = 0x25,
    RangingRxMacIeDecodedFailed = 0x26,
    RangingRxMaxIeMissing = 0x27,
    // RFU = 0x28-0x4F RFU

    // Proprietary Status Codes
    // Vendor Specific = 0x50-0xFF
};

} // namespace uwb::protocol::fira::uci

#endif // FIRA_UCI_STATUS_CODES_HXX
