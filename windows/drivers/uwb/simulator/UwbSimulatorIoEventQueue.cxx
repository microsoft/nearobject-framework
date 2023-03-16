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

UwbSimulatorIoEventQueue::UwbSimulatorIoEventQueue(WDFQUEUE wdfQueue) :
    m_wdfQueue(wdfQueue)
{}

UwbSimulatorIoEventQueue::~UwbSimulatorIoEventQueue()
{
}

NTSTATUS
UwbSimulatorIoEventQueue::Initialize()
{
    return STATUS_SUCCESS;
}

NTSTATUS
UwbSimulatorIoEventQueue::Uninitialize()
{
    return STATUS_SUCCESS;
}
