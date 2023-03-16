/**
 * @file UwbSimulatorIoEventQueue.hxx
 * @brief This file contains the event queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2023
 */

#ifndef UWB_SIMULATOR_IO_EVENT_QUEUE_HXX
#define UWB_SIMULATOR_IO_EVENT_QUEUE_HXX

#include <cstddef>
#include <memory>

#include <windows.h>

#include <wdf.h>

#include "IUwbSimulatorDdiCallbacksLrp.hxx"

/**
 * @brief Per-event-queue context object.
 */
class UwbSimulatorIoEventQueue
{
public:
    /**
     * @brief Construct a new UwbSimulatorIoEventQueue object.
     *
     * @param wdfQueue
     */
    explicit UwbSimulatorIoEventQueue(WDFQUEUE wdfQueue);

    ~UwbSimulatorIoEventQueue();

    /**
     * @brief
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Initialize();

    /**
     * @brief
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Uninitialize();

private:
    WDFQUEUE m_wdfQueue;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorIoEventQueue, GetUwbSimulatorIoEventQueue)

#endif // UWB_SIMULATOR_IO_EVENT_QUEUE_HXX
