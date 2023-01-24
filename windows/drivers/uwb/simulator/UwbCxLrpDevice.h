/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    UwbCxLrpDevice.h

Abstract:

    Header file for the UWB device driver interface

--*/
// clang-format off
#pragma once

#include <windows.h>
#include <winioctl.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (NTDDI_VERSION >= NTDDI_WIN10_CU)

// {A7424370-45E5-49C3-AE35-42A3C41B34AD}
DEFINE_GUID(GUID_UWB_DEVICE_INTERFACE, 0xa7424370, 0x45e5, 0x49c3, 0xae, 0x35, 0x42, 0xa3, 0xc4, 0x1b, 0x34, 0xad);

// UWB DDI
// Comment for review: For commands we could alternatively define a single IOCTL_UWB_COMMAND
// but there seems to be no clear benefit in doing that
// TODO: Define FILE_DEVICE_UWB in %SDXROOT%\minkernel\published\base\devioctl.w
#define IOCTL_UWB_DEVICE_RESET                             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0100, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_DEVICE_RESET
#define IOCTL_UWB_GET_DEVICE_INFO                          CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0101, METHOD_BUFFERED, FILE_ANY_ACCESS) // Output: UWB_DEVICE_INFO
#define IOCTL_UWB_GET_DEVICE_CAPABILITIES                  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0102, METHOD_BUFFERED, FILE_ANY_ACCESS) // Output: UWB_DEVICE_CAPABILITIES
#define IOCTL_UWB_SET_DEVICE_CONFIG_PARAMS                 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0103, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_SET_DEVICE_CONFIG_PARAMS, Output: UWB_SET_DEVICE_CONFIG_PARAMS_STATUS
#define IOCTL_UWB_GET_DEVICE_CONFIG_PARAMS                 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0104, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_GET_DEVICE_CONFIG_PARAMS, Output: UWB_DEVICE_CONFIG_PARAMS
#define IOCTL_UWB_SESSION_INIT                             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0105, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_SESSION_INIT
#define IOCTL_UWB_SESSION_DEINIT                           CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0106, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_SESSION_DEINIT
#define IOCTL_UWB_SET_APP_CONFIG_PARAMS                    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0107, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_SET_APP_CONFIG_PARAMS, Output: UWB_SET_APP_CONFIG_PARAMS_STATUS
#define IOCTL_UWB_GET_APP_CONFIG_PARAMS                    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0108, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_GET_APP_CONFIG_PARAMS, Output: UWB_APP_CONFIG_PARAMS
#define IOCTL_UWB_GET_SESSION_COUNT                        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0109, METHOD_BUFFERED, FILE_ANY_ACCESS) // Output: UWB_SESSION_COUNT
#define IOCTL_UWB_GET_SESSION_STATE                        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x010A, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_GET_SESSION_STATE, Output: UWB_SESSION_STATE_STATUS
#define IOCTL_UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST CTL_CODE(FILE_DEVICE_UNKNOWN, 0x010B, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST
#define IOCTL_UWB_START_RANGING_SESSION                    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x010C, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_START_RANGING_SESSION
#define IOCTL_UWB_STOP_RANGING_SESSION                     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x010D, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_STOP_RANGING_SESSION
#define IOCTL_UWB_GET_RANGING_COUNT                        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x010E, METHOD_BUFFERED, FILE_ANY_ACCESS) // Input: UWB_GET_RANGING_COUNT, Output: UWB_RANGING_COUNT

// Comment for review: For notifications we define a single IOCTL so the caller of this interface
// could receive all notifications using a single thread
#define IOCTL_UWB_NOTIFICATION                             CTL_CODE(FILE_DEVICE_UNKNOWN, 0x0140, METHOD_BUFFERED, FILE_ANY_ACCESS) // Output: UWB_NOTIFICATION_DATA

// UWB Status Codes
enum UWB_STATUS {
    // Generic Status Codes
    // Success
    UWB_STATUS_OK = 0x00,
    // Intended operation is not supported in the current state
    UWB_STATUS_REJECTED,
    // Intended operation is failed to complete
    UWB_STATUS_FAILED,
    // UCI packet structure is not per spec
    UWB_STATUS_SYNTAX_ERROR,
    // Config ID is not correct, and it is not present in UWBS
    UWB_STATUS_INVALID_PARAM,
    // Config ID is correct, and value is not in proper range
    UWB_STATUS_INVALID_RANGE,
    // UCI packet payload size is not as per spec
    UWB_STATUS_INVALID_MESSAGE_SIZE,
    // UCI Group ID is not per spec
    UWB_STATUS_UNKNOWN_GID,
    // UCI Opcode ID is not per spec
    UWB_STATUS_UNKNOWN_OID,
    // Config ID is read-only
    UWB_STATUS_READ_ONLY,
    // UWBS request retransmission from Host
    UWB_STATUS_COMMAND_RETRY,

    // UWB Session Specific Status Codes
    // Session is not existing or not created
    UWB_STATUS_ERROR_SESSION_NOT_EXIST = 0x11,
    // Session is already created/exist
    UWB_STATUS_ERROR_SESSION_DUPLICATE,
    // Session is active
    UWB_STATUS_ERROR_SESSION_ACTIVE,
    // Max number of sessions already created
    UWB_STATUS_ERROR_MAX_SESSIONS_EXCEEDED,
    // Session is not configured with required app configurations
    UWB_STATUS_ERROR_SESSION_NOT_CONFIGURED,
    // Sessions are actively running in UWBS
    UWB_STATUS_ERROR_ACTIVE_SESSIONS_ONGOING,
    // Indicates when multicast list is full during one to many ranging
    UWB_STATUS_ERROR_MULTICAST_LIST_FULL,
    // Indicates when short address is not available multicast list
    UWB_STATUS_ERROR_ADDRESS_NOT_FOUND,
    // Indicates when short address is already present
    UWB_STATUS_ERROR_ADDRESS_ALREADY_PRESENT,

    // UWB Ranging Session Specific Status Codes
    // Failed to transmit UWB packet
    UWB_STATUS_RANGING_TX_FAILED = 0x20,
    // No UWB packet detected by the receiver
    UWB_STATUS_RANGING_RX_TIMEOUT,
    // UWB packet channel decoding error
    UWB_STATUS_RANGING_RX_PHY_DEC_FAILED,
    // Failed to detect time of arrival of the UWB packet from CIR samples
    UWB_STATUS_RANGING_RX_PHY_TOA_FAILED,
    // UWB packet STS segment mismatch
    UWB_STATUS_RANGING_RX_PHY_STS_FAILED,
    // MAC CRC or syntax error
    UWB_STATUS_RANGING_RX_MAC_DEC_FAILED,
    // IE syntax error
    UWB_STATUS_RANGING_RX_MAC_IE_DEC_FAILED,
    // Expected IE missing in the packet
    UWB_STATUS_RANGING_RX_MAC_IE_MISSING
};

#pragma region UciCoreGroup

typedef struct _UWB_DEVICE_INFO {
    uint32_t size;
    UWB_STATUS status;
    uint8_t uciGenericVersionMajor;
    uint8_t uciGenericVersionMinorAndMaintenance;
    uint8_t macVersionMajor;
    uint8_t macVersionMinorAndMaintenance;
    uint8_t phyVersionMajor;
    uint8_t phyVersionMinorAndMaintenance;
    uint8_t uciTestVersionMajor;
    uint8_t uciTestVersionMinorAndMaintenance;
    uint32_t vendorSpecificInfoLength;
    _Field_size_bytes_(vendorSpecificInfoLength) uint8_t vendorSpecificInfo[ANYSIZE_ARRAY];
} UWB_DEVICE_INFO, *PUWB_DEVICE_INFO;

enum UWB_CAPABILITY_PARAM_TYPE {
    UWB_CAPABILITY_PARAM_TYPE_PHY_VERSION_RANGE = 0x00,
    UWB_CAPABILITY_PARAM_TYPE_MAC_VERSION_RANGE,
    UWB_CAPABILITY_PARAM_TYPE_DEVICE_ROLES,
    UWB_CAPABILITY_PARAM_TYPE_RANGING_METHOD,
    UWB_CAPABILITY_PARAM_TYPE_STS_CONFIG,
    UWB_CAPABILITY_PARAM_TYPE_MULTI_NODE_MODE,
    UWB_CAPABILITY_PARAM_TYPE_RANGING_TIME_STRUCT,
    UWB_CAPABILITY_PARAM_TYPE_SCHEDULED_MODE,
    UWB_CAPABILITY_PARAM_TYPE_HOPPING_MODE,
    UWB_CAPABILITY_PARAM_TYPE_BLOCK_STRIDING,
    UWB_CAPABILITY_PARAM_TYPE_UWB_INITIATION_TIME,
    UWB_CAPABILITY_PARAM_TYPE_CHANNELS,
    UWB_CAPABILITY_PARAM_TYPE_RFRAME_CONFIG,
    UWB_CAPABILITY_PARAM_TYPE_CC_CONSTRAINT_LENGTH,
    UWB_CAPABILITY_PARAM_TYPE_BPRF_PARAMETER_SETS,
    UWB_CAPABILITY_PARAM_TYPE_HPRF_PARAMETER_SETS,
    UWB_CAPABILITY_PARAM_TYPE_AOA_SUPPORT,
    UWB_CAPABILITY_PARAM_TYPE_EXTENDED_MAC_ADDRESS
};

typedef struct _UWB_CAPABILITY_PARAM {
    uint32_t size;
    UWB_CAPABILITY_PARAM_TYPE paramType;
    uint32_t paramLength;
    _Field_size_bytes_(paramLength) uint8_t paramValue[ANYSIZE_ARRAY];
} UWB_CAPABILITY_PARAM, *PUWB_CAPABILITY_PARAM;

typedef struct _UWB_DEVICE_CAPABILITIES {
    uint32_t size;
    UWB_STATUS status;
    uint32_t capabilityParamsCount;
    _Field_size_(capabilityParamsCount) UWB_CAPABILITY_PARAM capabilityParams[ANYSIZE_ARRAY];
} UWB_DEVICE_CAPABILITIES, *PUWB_DEVICE_CAPABILITIES;

enum UWB_DEVICE_CONFIG_PARAM_TYPE {
    UWB_DEVICE_CONFIG_PARAM_TYPE_DEVICE_STATE = 0x00,
    UWB_DEVICE_CONFIG_PARAM_TYPE_LOW_POWER_MODE
};

typedef struct _UWB_DEVICE_CONFIG_PARAM {
    uint32_t size;
    UWB_DEVICE_CONFIG_PARAM_TYPE paramType;
    uint32_t paramLength;
    _Field_size_bytes_(paramLength) uint8_t paramValue[ANYSIZE_ARRAY];
} UWB_DEVICE_CONFIG_PARAM, *PUWB_DEVICE_CONFIG_PARAM;

typedef struct _UWB_SET_DEVICE_CONFIG_PARAMS {
    uint32_t size;
    uint32_t deviceConfigParamsCount;
    _Field_size_(deviceConfigParamsCount) UWB_DEVICE_CONFIG_PARAM deviceConfigParams[ANYSIZE_ARRAY];
} UWB_SET_DEVICE_CONFIG_PARAMS, *PUWB_SET_DEVICE_CONFIG_PARAMS;

typedef struct _UWB_DEVICE_CONFIG_PARAM_STATUS {
    uint32_t size;
    UWB_DEVICE_CONFIG_PARAM_TYPE paramType;
    UWB_STATUS status;
} UWB_DEVICE_CONFIG_PARAM_STATUS, *PUWB_DEVICE_CONFIG_PARAM_STATUS;

typedef struct _UWB_SET_DEVICE_CONFIG_PARAMS_STATUS {
    uint32_t size;
    UWB_STATUS status;
    uint32_t deviceConfigParamsCount;
    _Field_size_(deviceConfigParamsCount) UWB_DEVICE_CONFIG_PARAM_STATUS deviceConfigParamsStatus[ANYSIZE_ARRAY];
} UWB_SET_DEVICE_CONFIG_PARAMS_STATUS, *PUWB_SET_DEVICE_CONFIG_PARAMS_STATUS;

typedef struct _UWB_GET_DEVICE_CONFIG_PARAMS {
    uint32_t size;
    uint32_t deviceConfigParamsCount;
    _Field_size_(deviceConfigParamsCount) UWB_DEVICE_CONFIG_PARAM_TYPE deviceConfigParams[ANYSIZE_ARRAY];
} UWB_GET_DEVICE_CONFIG_PARAMS, *PUWB_GET_DEVICE_CONFIG_PARAMS;

typedef struct _UWB_DEVICE_CONFIG_PARAMS {
    uint32_t size;
    UWB_STATUS status;
    uint32_t deviceConfigParamsCount;
    _Field_size_(deviceConfigParamsCount) UWB_DEVICE_CONFIG_PARAM deviceConfigParams[ANYSIZE_ARRAY];
} UWB_DEVICE_CONFIG_PARAMS, *PUWB_DEVICE_CONFIG_PARAMS;

// Device State Values
enum UWB_DEVICE_STATE {
    // UWBS is initialized and ready for UWB session
    UWB_DEVICE_STATE_READY = 0x01,
    // UWBS is busy with UWB session
    UWB_DEVICE_STATE_ACTIVE,
    // Error occurred within the UWBS
    UWB_DEVICE_STATE_ERROR = 0xFF
};

// Device Status Notification
typedef struct _UWB_DEVICE_STATUS {
    uint32_t size;
    // UWBS device state
    UWB_DEVICE_STATE deviceState;
} UWB_DEVICE_STATUS, * PUWB_DEVICE_STATUS;

// Device Reset
enum UWB_RESET_CONFIG {
    UWB_RESET_CONFIG_UWBS_RESET = 0x00
};

typedef struct _UWB_DEVICE_RESET {
    uint32_t size;
    UWB_RESET_CONFIG resetConfig;
} UWB_DEVICE_RESET, * PUWB_DEVICE_RESET;

#pragma endregion

#pragma region UwbSessionConfigGroup

enum UWB_SESSION_TYPE {
    UWB_SESSION_TYPE_RANGING_SESSION = 0x00,
    UWB_SESSION_TYPE_DEVICE_TEST_MODE = 0xD0
};

typedef struct _UWB_SESSION_INIT {
    uint32_t size;
    uint32_t sessionId;
    UWB_SESSION_TYPE sessionType;
} UWB_SESSION_INIT, *PUWB_SESSION_INIT;

typedef struct _UWB_SESSION_DEINIT {
    uint32_t size;
    uint32_t sessionId;
} UWB_SESSION_DEINIT, *PUWB_SESSION_DEINIT;

enum UWB_APP_CONFIG_PARAM_TYPE {
    UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE = 0x00,
    UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE,
    UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG,
    UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE,
    UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER,
    UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES,
    UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS,
    UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS,
    UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION,
    UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL,
    UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX,
    UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE,
    UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL,
    UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ,
    UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG,
    UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR,
    UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR,
    UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE,
    UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG,
    UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX = 0x14,
    UWB_APP_CONFIG_PARAM_TYPE_SFD_ID,
    UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE,
    UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION,
    UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT = 0x1A,
    UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR,
    UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER,
    UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX = 0x1E,
    UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE,
    UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE = 0x22,
    UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION,
    UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE,
    UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY,
    UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE,
    UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID,
    UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV,
    UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS,
    UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY,
    UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME,
    UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE,
    UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH,
    UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG,
    UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT,
    UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID,
    UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE,
    UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS,
    UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH = 0x35
};

typedef struct _UWB_APP_CONFIG_PARAM {
    uint32_t size;
    UWB_APP_CONFIG_PARAM_TYPE paramType;
    uint32_t paramLength;
    _Field_size_bytes_(paramLength) uint8_t paramValue[ANYSIZE_ARRAY];
} UWB_APP_CONFIG_PARAM, *PUWB_APP_CONFIG_PARAM;

typedef struct _UWB_SET_APP_CONFIG_PARAMS {
    uint32_t size;
    uint32_t sessionId;
    uint32_t appConfigParamsCount;
    _Field_size_(appConfigParamsCount) UWB_APP_CONFIG_PARAM appConfigParams[ANYSIZE_ARRAY];
} UWB_SET_APP_CONFIG_PARAMS, *PUWB_SET_APP_CONFIG_PARAMS;

typedef struct _UWB_APP_CONFIG_PARAM_STATUS {
    uint32_t size;
    UWB_APP_CONFIG_PARAM_TYPE paramType;
    UWB_STATUS status;
} UWB_APP_CONFIG_PARAM_STATUS, *PUWB_APP_CONFIG_PARAM_STATUS;

typedef struct _UWB_SET_APP_CONFIG_PARAMS_STATUS {
    uint32_t size;
    UWB_STATUS status;
    uint32_t appConfigParamsCount;
    _Field_size_(appConfigParamsCount) UWB_APP_CONFIG_PARAM_STATUS appConfigParamsStatus[ANYSIZE_ARRAY];
} UWB_SET_APP_CONFIG_PARAMS_STATUS, *PUWB_SET_APP_CONFIG_PARAMS_STATUS;

typedef struct _UWB_GET_APP_CONFIG_PARAMS {
    uint32_t size;
    uint32_t sessionId;
    uint32_t appConfigParamsCount;
    _Field_size_(appConfigParamsCount) UWB_APP_CONFIG_PARAM_TYPE appConfigParams[ANYSIZE_ARRAY];
} UWB_GET_APP_CONFIG_PARAMS, *PUWB_GET_APP_CONFIG_PARAMS;

typedef struct _UWB_APP_CONFIG_PARAMS {
    uint32_t size;
    UWB_STATUS status;
    uint32_t appConfigParamsCount;
    _Field_size_(appConfigParamsCount) UWB_APP_CONFIG_PARAM appConfigParams[ANYSIZE_ARRAY];
} UWB_APP_CONFIG_PARAMS, *PUWB_APP_CONFIG_PARAMS;

enum UWB_SESSION_STATE {
    UWB_SESSION_STATE_INIT = 0x00,
    UWB_SESSION_STATE_DEINIT,
    UWB_SESSION_STATE_ACTIVE,
    UWB_SESSION_STATE_IDLE
};

enum UWB_SESSION_REASON_CODE {
    UWB_SESSION_REASON_CODE_STATE_CHANGE_WITH_SESSION_MANAGEMENT_COMMANDS = 0x00,
    UWB_SESSION_REASON_CODE_MAX_RANGING_ROUND_RETRY_COUNT_REACHED,
    UWB_SESSION_REASON_CODE_MAX_NUMBER_OF_MEASUREMENTS_REACHED,
    UWB_SESSION_REASON_CODE_ERROR_SLOT_LENGTH_NOT_SUPPORTED = 0x20,
    UWB_SESSION_REASON_CODE_ERROR_INSUFFICIENT_SLOTS_PER_RR,
    UWB_SESSION_REASON_CODE_ERROR_MAC_ADDRESS_MODE_NOT_SUPPORTED,
    UWB_SESSION_REASON_CODE_ERROR_INVALID_RANGING_INTERVAL,
    UWB_SESSION_REASON_CODE_ERROR_INVALID_STS_CONFIG,
    UWB_SESSION_REASON_CODE_ERROR_INVALID_RFRAME_CONFIG
};

typedef struct _UWB_SESSION_COUNT {
    uint32_t size;
    UWB_STATUS status;
    uint32_t sessionCount;
} UWB_GET_SESSION_COUNT, * PUWB_GET_SESSION_COUNT;

typedef struct _UWB_GET_SESSION_STATE {
    uint32_t size;
    uint32_t sessionId;
} UWB_GET_SESSION_STATE, * PUWB_GET_SESSION_STATE;

typedef struct _UWB_SESSION_STATE_STATUS {
    uint32_t size;
    UWB_STATUS status;
    UWB_SESSION_STATE sessionState;
} UWB_SESSION_STATE_STATUS, * PUWB_SESSION_STATE_STATUS;

enum UWB_MULTICAST_ACTION {
    UWB_MULTICAST_ACTION_ADD_SHORT_ADDRESS,
    UWB_MULTICAST_ACTION_DELETE_SHORT_ADDRESS
};

typedef struct _UWB_MULTICAST_CONTROLEE_LIST_ENTRY {
    uint32_t size;
    uint16_t shortAddress;
    uint32_t subSessionId;
} UWB_MULTICAST_CONTROLEE_LIST_ENTRY, * PUWB_MULTICAST_CONTROLEE_LIST_ENTRY;

// Session Status Notification
typedef struct _UWB_SESSION_STATUS {
    uint32_t size;
    uint32_t sessionId;
    UWB_SESSION_STATE state;
    UWB_SESSION_REASON_CODE reasonCode;
} UWB_SESSION_STATUS, *PUWB_SESSION_STATUS;

typedef struct _UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST {
    uint32_t size;
    uint32_t sessionId;
    UWB_MULTICAST_ACTION action;
    uint32_t numberOfControlees;
    _Field_size_(numberOfControlees) UWB_MULTICAST_CONTROLEE_LIST_ENTRY controleeList[ANYSIZE_ARRAY];
} UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST, * PUWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST;

enum UWB_MULTICAST_STATUS {
    UWB_MULTICAST_STATUS_OK_MULTICAST_LIST_UPDATE,
    UWB_MULTICAST_STATUS_ERROR_MULTICAST_LIST_FULL,
    UWB_MULTICAST_STATUS_ERROR_KEY_FETCH_FAIL,
    UWB_MULTICAST_STATUS_ERROR_SUB_SESSION_ID_NOT_FOUND
};

typedef struct _UWB_MULTICAST_LIST_STATUS {
    uint32_t size;
    uint16_t controleeMacAddress;
    uint32_t subSessionId;
    UWB_MULTICAST_STATUS status;
} UWB_MULTICAST_LIST_STATUS, * PUWB_MULTICAST_LIST_STATUS;

// Session Update Controller Multicast List Notification
typedef struct _UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF {
    uint32_t size;
    uint32_t sessionId;
    uint32_t remainingMulticastListSize;
    uint32_t numberOfControlees;
    _Field_size_(numberOfControlees) UWB_MULTICAST_LIST_STATUS statusList[ANYSIZE_ARRAY];
} UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF, * PUWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF;

#pragma endregion

#pragma region UwbRangingSessionControlGroup

typedef struct _UWB_START_RANGING_SESSION {
    uint32_t size;
    uint32_t sessionId;
} UWB_START_RANGING_SESSION, *PUWB_START_RANGING_SESSION;

typedef struct _UWB_STOP_RANGING_SESSION {
    uint32_t size;
    uint32_t sessionId;
} UWB_STOP_RANGING_SESSION, *PUWB_STOP_RANGING_SESSION;

typedef struct _UWB_GET_RANGING_COUNT {
    uint32_t size;
    uint32_t sessionId;
} UWB_GET_RANGING_COUNT, * PUWB_GET_RANGING_COUNT;

typedef struct _UWB_RANGING_COUNT {
    uint32_t size;
    UWB_STATUS status;
    uint32_t count;
} UWB_RANGING_COUNT, * PUWB_RANGING_COUNT;

enum UWB_RANGING_MEASUREMENT_TYPE {
    UWB_RANGING_MEASUREMENT_TYPE_TWO_WAY = 0x01
};

enum UWB_MAC_ADDRESS_MODE_INDICATOR {
    UWB_MAC_ADDRESS_MODE_INDICATOR_SHORT,
    UWB_MAC_ADDRESS_MODE_INDICATOR_EXTENDED
};

#define UWB_MAC_ADDRESS_LENGTH_SHORT 2
#define UWB_MAC_ADDRESS_LENGTH_EXTENDED 8

typedef struct _UWB_MAC_ADDRESS
{
    uint32_t size;
    UWB_MAC_ADDRESS_MODE_INDICATOR addressType;
    union
    {
        uint8_t addressShort[UWB_MAC_ADDRESS_LENGTH_SHORT];
        uint8_t addressExtended[UWB_MAC_ADDRESS_LENGTH_EXTENDED];
    };
} UWB_MAC_ADDRESS, *PUWB_MAC_ADDRESS;

enum UWB_LINE_OF_SIGHT_INDICATOR {
    UWB_LINE_OF_SIGHT_INDICATOR_LINE_OF_SIGHT = 0x00,
    UWB_LINE_OF_SIGHT_INDICATOR_NON_LINE_OF_SIGHT = 0x01,
    UWB_LINE_OF_SIGHT_INDICATOR_UNABLE_TO_DETERMINE = 0xFF
};

typedef struct _UWB_RANGING_MEASUREMENT {
    uint32_t size;
    UWB_MAC_ADDRESS macAddrPeer;
    UWB_STATUS status;
    UWB_LINE_OF_SIGHT_INDICATOR lineOfSightIndicator;
    // Distance in centimeters
    uint16_t distance;
    // AoA Azimuth in degrees
    // Allowed values range from -180 to +180
    uint8_t aoaAzimuth[2];
    // IEEE 802.15.4z-2020, Section 6.9.1.7: Ranging FoM
    // Figure of Merit goes from 0 to 100
    uint8_t aoaAzimuthFigureOfMerit;
    // AoA Elevation in degrees
    // Allowed values range from -90 to +90
    uint8_t aoaElevation[2];
    // Figure of Merit goes from 0 to 100
    uint8_t aoaElevationFigureOfMerit;
    // AoA Destination Azimuth in degrees
    // Allowed values range from -180 to +180
    uint8_t aoaDestinationAzimuth[2];
    // Figure of Merit goes from 0 to 100
    uint8_t aoaDestinationAzimuthFigureOfMerit;
    // AoA Destination Elevation in degrees
    // Allowed values range from -90 to +90
    uint8_t aoaDestinationElevation[2];
    // Figure of Merit goes from 0 to 100
    uint8_t aoaDestinationElevationFigureOfMerit;
    // Slot number starts from 0
    uint8_t slotIndex;
} UWB_RANGING_MEASUREMENT, *PUWB_RANGING_MEASUREMENT;

// Ranging Data Notification
typedef struct _UWB_RANGING_DATA {
    uint32_t size;
    uint32_t sequenceNumber;
    uint32_t sessionId;
    uint32_t currentRangingInterval;
    UWB_RANGING_MEASUREMENT_TYPE rangingMeasurementType;
    uint32_t numberOfRangingMeasurements;
    _Field_size_(numberOfRangingMeasurements) UWB_RANGING_MEASUREMENT rangingMeasurements[ANYSIZE_ARRAY];
} UWB_RANGING_DATA, *PUWB_RANGING_DATA;

#pragma endregion

enum UWB_NOTIFICATION_TYPE {
    UWB_NOTIFICATION_TYPE_DEVICE_STATUS,
    UWB_NOTIFICATION_TYPE_GENERIC_ERROR,
    UWB_NOTIFICATION_TYPE_SESSION_STATUS,
    UWB_NOTIFICATION_TYPE_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST,
    UWB_NOTIFICATION_TYPE_RANGING_DATA
};

typedef struct _UWB_NOTIFICATION_DATA
{
    uint32_t size;
    UWB_NOTIFICATION_TYPE notificationType;
    union
    {
        UWB_DEVICE_STATUS deviceStatus;
        UWB_STATUS genericError;
        UWB_SESSION_STATUS sessionStatus;
        UWB_SESSION_UPDATE_CONTROLLER_MULTICAST_LIST_NTF sessionUpdateControllerMulticastList;
        UWB_RANGING_DATA rangingData;
    };
} UWB_NOTIFICATION_DATA, *PUWB_NOTIFICATION_DATA;

#endif // NTDDI_VERSION

#ifdef __cplusplus
}
#endif

// clang-format on
