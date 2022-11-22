
#ifndef UWBCX_LRP_DEVICE_GLUE_H
#define UWBCX_LRP_DEVICE_GLUE_H

// clang-format off

/**
 * Definitions below copied from the Windows WDK; these must eventually be
 * removed and the project targeted to use the proper headers from the installed
 * WDK.
 */

#define FILE_DEVICE_UNKNOWN 34
#define METHOD_BUFFERED 0
#define FILE_ANY_ACCESS 0
#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

// NB: This must come before any other Windows include
#include <windows.h>

#include <UwbCxLrpDevice.h>

// clang-format on

#endif // UWBCX_LRP_DEVICE_GLUE_H
