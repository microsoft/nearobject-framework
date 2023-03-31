/**
 * @file UwbSimulatorIoEventQueue.cxx
 * @brief This file contains the queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2023
 */

#include <memory>
#include <span>

#include <windows.h>

#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorIoEventQueue.hxx"
#include "UwbSimulatorTracelogging.hxx"
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

using ::uwb::protocol::fira::UwbNotificationData;

UwbSimulatorIoEventQueue::UwbSimulatorIoEventQueue(WDFQUEUE wdfQueue, std::size_t maximumQueueSize) :
    m_wdfQueue(wdfQueue),
    m_maximumQueueSize(maximumQueueSize)
{}

NTSTATUS
UwbSimulatorIoEventQueue::Initialize()
{
    WDF_OBJECT_ATTRIBUTES lockAttributes{};
    WDF_OBJECT_ATTRIBUTES_INIT(&lockAttributes);
    lockAttributes.ParentObject = m_wdfQueue;

    NTSTATUS status = WdfWaitLockCreate(&lockAttributes, &m_wdfQueueLock);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // Start the notification processing thread.
    m_notificationThread = std::jthread([this](std::stop_token stopToken) {
        ProcessNotificationQueue(std::move(stopToken));
    });

    return status;
}

NTSTATUS
UwbSimulatorIoEventQueue::Uninitialize()
{
    return STATUS_SUCCESS;
}

NTSTATUS
UwbSimulatorIoEventQueue::EnqueueRequest(WDFREQUEST request)
{
    WDFREQUEST requestExisting = nullptr;
    auto wdfFile = WdfRequestGetFileObject(request);
    std::scoped_lock notificationLockExclusive{ m_notificationGate };

    // Check if a request is already pending.
    NTSTATUS status = WdfIoQueueFindRequest(m_wdfQueue, nullptr, wdfFile, nullptr, &requestExisting);
    if (requestExisting != nullptr) {
        DbgPrint("%p an existing request already pending", m_wdfQueue);
        WdfObjectDereference(requestExisting);
        return STATUS_INVALID_DEVICE_STATE;
    }

    // Forward the request to the queue and indicate the i/o is pending.
    DbgPrint("%p pending request %p\n", m_wdfQueue, request);
    status = WdfRequestForwardToIoQueue(request, m_wdfQueue);
    if (!NT_SUCCESS(status)) {
        DbgPrint("%p failed to pend request %p with status 0x%08x\n", m_wdfQueue, status);
        return status;
    }

    // Notify the processing thread that a new request is pending.
    m_notificationRequestPending.notify_one();

    // Return pending status so dispatch code won't complete the request.
    return STATUS_PENDING;
}

void
UwbSimulatorIoEventQueue::PushNotification(UwbNotificationData notificationData)
{
    // Push the notification data into the queue.
    {
        std::scoped_lock notificationLockExclusive{ m_notificationGate };
        DbgPrint("%p pushing notification data with payload %s\n", m_wdfQueue, std::data(ToString(notificationData)));
        m_notificationQueue.push(std::move(notificationData));
    }

    // Signal the processing thread that notification data is available.
    m_notificationDataAvailable.notify_one();
}

void
UwbSimulatorIoEventQueue::ProcessNotificationQueue(std::stop_token stopToken)
{
    for (;;) {
        // Wait for a notification request to be pended.
        std::unique_lock notificationLock{ m_notificationGate };
        DbgPrint("%p waiting for notification queue items\n", m_wdfQueue);
        const auto isNotificationRequestPending = m_notificationRequestPending.wait(notificationLock, stopToken, [&] {
            auto ioRequestQueueState = WdfIoQueueGetState(m_wdfQueue, nullptr, nullptr);
            return !WDF_IO_QUEUE_IDLE(ioRequestQueueState);
        });

        // If predicate returned false, wait() unblocked due to stop request.
        if (!isNotificationRequestPending) {
            assert(stopToken.stop_requested());
            DbgPrint("%p exiting notification processing thread due to stop request\n", m_wdfQueue);
            break;
        }

        // Wait for notification data to become available.
        const auto isNotificationDataAvailable = m_notificationDataAvailable.wait(notificationLock, stopToken, [&] {
            return !m_notificationQueue.empty();
        });

        if (!isNotificationDataAvailable) {
            assert(stopToken.stop_requested());
            DbgPrint("%p exiting notification processing thread due to stop request\n", m_wdfQueue);
        }

        // Obtain the pended request from the queue.
        WDFREQUEST request = nullptr;
        NTSTATUS status = WdfIoQueueRetrieveNextRequest(m_wdfQueue, &request);
        if (!NT_SUCCESS(status)) {
            DbgPrint("%p unable to retrieve pending i/o request from queue, status=0x%08x\n", m_wdfQueue, status);
            continue;
        }

        // Remove the notification data from the top of the queue.
        UwbNotificationData notification = std::move(m_notificationQueue.front());
        m_notificationQueue.pop();

        // Release the lock now that we've removed the queue item.
        notificationLock.unlock();

        // Convert the neutral type notification data to the DDI type.
        auto notificationDdi = UwbCxDdi::From(notification);
        auto notificationDdiBuffer = std::data(notificationDdi);
        auto notificationDdiSize = std::size(notificationDdiBuffer);

        // Attempt to copy the notification data to the output buffer. Note that
        // WdfRequestRetrieveOutBuffer() will return STATUS_BUFFER_TOO_SMALL if
        // the request buffer is not large enough to contain available data.
        void *outputBuffer = nullptr;
        std::size_t outputBufferSize = 0;
        status = WdfRequestRetrieveOutputBuffer(request, notificationDdiSize, &outputBuffer, &outputBufferSize);
        if (NT_SUCCESS(status)) {
            DbgPrint("%p completing request %p with %llu byte payload %s\n", m_wdfQueue, request, notificationDdiSize, ToString(notification).c_str());
            std::memcpy(outputBuffer, std::data(notificationDdiBuffer), notificationDdiSize);
        }

        // Complete the request.
        WdfRequestCompleteWithInformation(request, status, notificationDdiSize);
    } // for (;;)
}
