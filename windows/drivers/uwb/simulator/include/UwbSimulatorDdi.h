/**
 * @file UwbSimulatorDdi.h
 * @brief The file contains the private simulator driver device interface (DDI) definitions.
 *
 * @copyright Copyright (c) 2023
 */
#ifndef UWB_SIMULATOR_DDI_H
#define UWB_SIMULATOR_DDI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Device GUID for the Uwb Simulator driver.
 *
 * 21663d8d-2dd6-45c7-a54a-d902202124d7
 */
const GUID GUID_DEVINTERFACE_UWB_SIMULATOR = { 0x21663d8d, 0x2dd6, 0x45c7, { 0xa5, 0x4a, 0xd9, 0x02, 0x20, 0x21, 0x24, 0xd7 } };

/**
 * @brief Obtains the capabilities of the simulator driver.
 *
 * Input: None
 * Output: UwbSimulatorCapabilities
 */
#define IOCTL_UWB_DEVICE_SIM_GET_CAPABILITIES CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1000, METHOD_BUFFERED, FILE_ANY_ACCESS)

/**
 * @brief
 *
 * Input: UwbSimulatorTriggerSessionEventArgs
 * Output: ???
 */
#define IOCTL_UWB_DEVICE_SIM_TRIGGER_SESSION_EVENT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1001, METHOD_BUFFERED, FILE_ANY_ACCESS)

/**
 * @brief Describes the capabilities of the simulator driver, including
 * versioning and protocol information.
 */
struct UwbSimulatorCapabilities
{
    uint32_t Version;
};

enum UwbSimulatorSessionEventAction {
    None = 0x00,
    RandomRangingMeasurementGenerationStart = 0x01,
    RandomRangingMeasurementGenerationStop = 0x02,
};

struct UwbSimulatorTriggerSessionEventArgs
{
    uint32_t SessionId;
    UwbSimulatorSessionEventAction Action;
};

struct UwbSimulatorTriggerSessionEventResult
{
    // TODO
};

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // UWB_SIMULATOR_DDI_H
