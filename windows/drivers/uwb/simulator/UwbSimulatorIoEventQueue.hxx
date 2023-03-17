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
    static constexpr std::size_t MaximumQueueSizeDefault = 1024;

    /**
     * @brief Construct a new UwbSimulatorIoEventQueue object.
     *
     * @param wdfQueue
     * @param maximumQueueSize The maximum size of the notification queue before
     * data is dropped.
     */
    explicit UwbSimulatorIoEventQueue(WDFQUEUE wdfQueue, std::size_t maximumQueueSize = MaximumQueueSizeDefault);

    /**
     * @brief Destroy the UwbSimulatorIoEventQueue object.
     */
    ~UwbSimulatorIoEventQueue() = default;

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
     * @brief Handle a WDF request for notification data.
     *
     * @param request The WDF request handle.
     * @param notificationData The output argument to hold notification request data, if available.
     * @param outputBufferSize The size of the DDI output buffer that will eventually be populated.
     * @return NTSTATUS
     */
    NTSTATUS
    HandleNotificationRequest(WDFREQUEST request, std::optional<::uwb::protocol::fira::UwbNotificationData> &notificationData, std::size_t &outputBufferSize);

    /**
     * @brief Push new notification request data onto the queue.
     *
     * @param notificationData The notification data.
     * @return NTSTATUS
     */
    NTSTATUS
    PushNotificationData(::uwb::protocol::fira::UwbNotificationData notificationData);

private:
    WDFQUEUE m_wdfQueue;
    WDFWAITLOCK m_wdfQueueLock{ nullptr };
    std::optional<std::size_t> m_pendingRequestOutputBufferSize;
    std::queue<::uwb::protocol::fira::UwbNotificationData> m_notificationQueue;
    const std::size_t m_maximumQueueSize{ MaximumQueueSizeDefault };
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorIoEventQueue, GetUwbSimulatorIoEventQueue)

#endif // UWB_SIMULATOR_IO_EVENT_QUEUE_HXX
