/**
 * @file UwbSimulatorIoEventQueue.cxx
 * @brief This file contains the queue entry points and callbacks.
 *
 * @copyright Copyright (c) 2023
 */

#include <memory>

#include <windows.h>

#include "UwbSimulatorIoEventQueue.hxx"
#include "UwbSimulatorTracelogging.hxx"
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

using ::uwb::protocol::fira::UwbNotificationData;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbSimulatorIoEventQueue::UwbSimulatorIoEventQueue(WDFQUEUE wdfQueue) :
    m_wdfQueue(wdfQueue)
{}

UwbSimulatorIoEventQueue::~UwbSimulatorIoEventQueue()
{
}

WDFQUEUE
UwbSimulatorIoEventQueue::GetWdfQueue() const noexcept
{
    return m_wdfQueue;
}

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
UwbSimulatorIoEventQueue::PendRequest(WDFREQUEST request, std::size_t outputBufferSize)
{
    NTSTATUS status;
    status = WdfWaitLockAcquire(m_wdfQueueLock, nullptr);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = WdfRequestForwardToIoQueue(request, m_wdfQueue);
    if (NT_SUCCESS(status)) {
        m_pendingRequestOutputBufferSize = outputBufferSize;
    }

    WdfWaitLockRelease(m_wdfQueueLock);
    return status;
}

NTSTATUS
UwbSimulatorIoEventQueue::GetNextQueuedRequest(std::optional<UwbNotificationData> notificationDataOpt, std::size_t &outputBufferSize)
{
    NTSTATUS status = WdfWaitLockAcquire(m_wdfQueueLock, nullptr);
    if (NT_SUCCESS(status)) {
        if (!m_notificationQueue.empty()) {
            auto &notificationData = m_notificationQueue.front();
            auto converted = UwbCxDdi::From(notificationData);
            auto outputBufferSizeRequired = converted.size();
            if (outputBufferSize < outputBufferSizeRequired) {
                status = STATUS_BUFFER_TOO_SMALL;
            } else {
                notificationData = std::move(notificationData);
                m_notificationQueue.pop();
            }
        } else {
            status = STATUS_NO_MORE_ENTRIES;
            // Forward request onto queue
        }
    }

    WdfWaitLockRelease(m_wdfQueueLock);
    
    return status;
}
