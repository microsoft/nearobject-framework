/**
 * @file UwbSimulatorIoEventQueue.hxx
 * @brief This file contains the event queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2023
 */

#ifndef UWB_SIMULATOR_IO_EVENT_QUEUE_HXX
#define UWB_SIMULATOR_IO_EVENT_QUEUE_HXX

#include <condition_variable>
#include <cstddef>
#include <memory>
#include <optional>
#include <queue>
#include <stop_token>
#include <thread>

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

    NTSTATUS
    EnqueueRequest(WDFREQUEST request);

    /**
     * @brief Push new notification data onto the queue.
     *
     * @param notificationData The notification data.
     */
    void
    PushNotification(::uwb::protocol::fira::UwbNotificationData notificationData);

private:
    void
    ProcessNotificationQueue(std::stop_token stopToken);  

private:
    WDFQUEUE m_wdfQueue;
    WDFWAITLOCK m_wdfQueueLock{ nullptr };
    std::queue<::uwb::protocol::fira::UwbNotificationData> m_notificationQueue;

    // Notification processing. Note the jthread must come last to ensure it is
    // joined first upon destruction, which will signal stop and release any waits
    // on the condition variables.
    std::mutex m_notificationGate;
    std::condition_variable_any m_notificationRequestPending;
    std::condition_variable_any m_notificationDataAvailable;
    std::jthread m_notificationThread;

    const std::size_t m_maximumQueueSize{ MaximumQueueSizeDefault };
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorIoEventQueue, GetUwbSimulatorIoEventQueue)

#endif // UWB_SIMULATOR_IO_EVENT_QUEUE_HXX
