/**
 * @file UwbSimulatorIoEventQueue.cxx
 * @brief This file contains the queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2023
 */

#include <memory>
#include <span>

#include <windows.h>

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
    return status;
}

NTSTATUS
UwbSimulatorIoEventQueue::Uninitialize()
{
    return STATUS_SUCCESS;
}

NTSTATUS
UwbSimulatorIoEventQueue::HandleNotificationRequest(WDFREQUEST request, std::optional<UwbNotificationData> &notificationDataOpt, std::size_t &outputBufferSize)
{
    NTSTATUS status = WdfWaitLockAcquire(m_wdfQueueLock, nullptr);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    if (!m_notificationQueue.empty()) {
        auto &notificationData = m_notificationQueue.front();
        auto converted = UwbCxDdi::From(notificationData);
        auto outputBufferSizeRequired = converted.size();
        if (outputBufferSize < outputBufferSizeRequired) {
            outputBufferSize = outputBufferSizeRequired;
            status = STATUS_BUFFER_TOO_SMALL;
        } else {
            notificationDataOpt = std::move(notificationData);
            m_notificationQueue.pop();
            status = STATUS_SUCCESS;
        }
    } else {
        status = WdfRequestForwardToIoQueue(request, m_wdfQueue);
        if (!NT_SUCCESS(status)) {
            // TODO: log
        }
        status = STATUS_PENDING;
    }

    WdfWaitLockRelease(m_wdfQueueLock);

    return status;
}

NTSTATUS
UwbSimulatorIoEventQueue::PushNotificationData(UwbNotificationData notificationData)
{
    // Convert the neutral notification type to the DDI type. This needed, at
    // minimum, to determine the required buffer size since it will eventually
    // contain the DDI type.
    auto notificationDataDdi = UwbCxDdi::From(notificationData);
    auto notificationDataDdiBuffer = std::data(notificationDataDdi);
    auto notificationDataDdiBufferSize = std::size(notificationDataDdi);

    NTSTATUS status = WdfWaitLockAcquire(m_wdfQueueLock, nullptr);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    // TODO: could we easily support > 1 pending requests by draining the queue
    // here in a loop instead of taking the top queue entry only?

    // Attempt to retrieve the most recent pended request.
    WDFREQUEST request = nullptr;
    status = WdfIoQueueRetrieveNextRequest(m_wdfQueue, &request);

    // A request was pended, complete it with this notification data.
    if (NT_SUCCESS(status)) {
        void *outputBuffer = nullptr;
        status = WdfRequestRetrieveOutputBuffer(request, notificationDataDdiBufferSize, &outputBuffer, nullptr);
        if (NT_SUCCESS(status)) {
            std::memcpy(outputBuffer, std::data(notificationDataDdiBuffer), notificationDataDdiBufferSize);
        }

        // Complete the request.
        WdfRequestCompleteWithInformation(request, status, notificationDataDdiBufferSize);
    } else {
        // No request is pended, so queue this notification data for future requests.
        // If the queue is full, drop requests until there is space for this entry.
        while (std::size(m_notificationQueue) >= m_maximumQueueSize) {
            m_notificationQueue.pop();
        }

        // Add this notification data to the queue.
        m_notificationQueue.push(std::move(notificationData));
        status = STATUS_SUCCESS;
    }

    return status;
}
