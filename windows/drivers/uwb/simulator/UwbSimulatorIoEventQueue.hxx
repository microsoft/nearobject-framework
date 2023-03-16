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
#include <optional>
#include <queue>

#include <windows.h>

#include <wdf.h>

#include "IUwbSimulatorDdiCallbacksLrp.hxx"
#include <uwb/protocols/fira/FiraDevice.hxx>

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

    WDFQUEUE
    GetWdfQueue() const noexcept;

    NTSTATUS
    PendRequest(WDFREQUEST request, std::size_t outputBufferSize);

    NTSTATUS
    GetNextQueuedRequest(std::optional<::uwb::protocol::fira::UwbNotificationData> notificationData, std::size_t &outputBufferSize);

private:
    WDFQUEUE m_wdfQueue;
    WDFWAITLOCK m_wdfQueueLock{ nullptr };
    std::optional<std::size_t> m_pendingRequestOutputBufferSize;
    std::queue<::uwb::protocol::fira::UwbNotificationData> m_notificationQueue;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorIoEventQueue, GetUwbSimulatorIoEventQueue)

#endif // UWB_SIMULATOR_IO_EVENT_QUEUE_HXX
