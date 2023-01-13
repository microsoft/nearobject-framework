/**
 * @file Driver.hxx
 * @brief This file contains the driver definitions.
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <windows.h>

#include <wdf.h>

EXTERN_C_START

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_OBJECT_CONTEXT_CLEANUP OnWdfDriverUnload;

EXTERN_C_END
