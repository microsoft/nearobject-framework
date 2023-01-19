/**
 * @file UwbSimulatorIoQueue.hxx
 * @brief This file contains the queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2022
 */

#ifndef UWB_SIMULATOR_IO_QUEUE_HXX
#define UWB_SIMULATOR_IO_QUEUE_HXX

#include <cstddef>

#include <windows.h>

#include <wdf.h>

/**
 * @brief Per-queue context object.
 */
class UwbSimulatorIoQueue
{
public:
    /**
     * @brief Construct a new UwbSimulatorIoQueue object.
     *
     * @param wdfQueue
     */
    explicit UwbSimulatorIoQueue(WDFQUEUE wdfQueue);

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

    /**
     * @brief
     *
     * @param request
     * @param outputBufferLength
     * @param inputBufferLength
     * @param ioControlCode
     */
    void
    OnIoDeviceControl(WDFREQUEST request, size_t outputBufferLength, size_t inputBufferLength, ULONG ioControlCode);

    /**
     * @brief This event is invoked for a power-managed queue before the device
     * leaves the working state (D0).
     *
     * @param request
     * @param actionFlags
     */
    void
    OnIoStop(WDFREQUEST request, ULONG actionFlags);

public:
    static EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL OnWdfIoDeviceControl;
    static EVT_WDF_IO_QUEUE_IO_STOP OnWdfIoStop;

private:
    WDFQUEUE m_wdfQueue;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorIoQueue, GetUwbSimulatorIoQueue)

#endif // UWB_SIMULATOR_IO_QUEUE_HXX
