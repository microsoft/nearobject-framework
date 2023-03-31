
#include <memory>

#include <magic_enum.hpp>

#include "UwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorDevice.hxx"
#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorTracelogging.hxx"

using windows::devices::uwb::simulator::UwbSimulatorDdiHandler;
using windows::devices::uwb::simulator::UwbSimulatorSession;

UwbSimulatorDevice::UwbSimulatorDevice(WDFDEVICE wdfDevice) :
    m_wdfDevice(wdfDevice),
    m_ddiHandler(std::make_shared<UwbSimulatorDdiHandler>(this))
{}

/* static */
NTSTATUS
UwbSimulatorDevice::OnWdfDeviceAdd(WDFDRIVER /* driver */, PWDFDEVICE_INIT deviceInit)
{
    // Configure power callbacks.
    WDF_PNPPOWER_EVENT_CALLBACKS powerCallbacks;
    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&powerCallbacks);
    powerCallbacks.EvtDeviceD0Entry = &UwbSimulatorDevice::OnWdfD0Entry;
    powerCallbacks.EvtDeviceD0Exit = &UwbSimulatorDevice::OnWdfD0Exit;
    WdfDeviceInitSetPnpPowerEventCallbacks(deviceInit, &powerCallbacks);

    // Configure device file association.
    WDF_FILEOBJECT_CONFIG fileConfiguration;
    WDF_FILEOBJECT_CONFIG_INIT(&fileConfiguration, &UwbSimulatorDevice::OnWdfFileCreate, &UwbSimulatorDevice::OnWdfFileClose, WDF_NO_EVENT_CALLBACK);
    WDF_OBJECT_ATTRIBUTES fileAttributes;
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&fileAttributes, UwbSimulatorDeviceFile);
    fileAttributes.EvtDestroyCallback = &UwbSimulatorDeviceFile::OnWdfDestroy;
    WdfDeviceInitSetFileObjectConfig(deviceInit, &fileConfiguration, &fileAttributes);

    // Configure device object attributes.
    WDF_OBJECT_ATTRIBUTES deviceAttributes;
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, UwbSimulatorDevice);
    deviceAttributes.EvtCleanupCallback = &UwbSimulatorDevice::OnWdfObjectContextCleanup;
    deviceAttributes.SynchronizationScope = WdfSynchronizationScopeNone;
    deviceAttributes.ExecutionLevel = WdfExecutionLevelPassive;

    // Create the device.
    WDFDEVICE device;
    NTSTATUS status = WdfDeviceCreate(&deviceInit, &deviceAttributes, &device);
    if (!NT_SUCCESS(status)) {
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "WdfDeviceCreate failed",
            TraceLoggingLevel(TRACE_LEVEL_CRITICAL),
            TraceLoggingNTStatus(status));
        return status;
    }

    // Initialize the device context from the WDF pre-allocated WDF buffer.
    auto uwbSimulatorDeviceBuffer = GetUwbSimulatorDevice(device);
    auto uwbSimulatorDevice = new (uwbSimulatorDeviceBuffer) UwbSimulatorDevice(device);

    status = uwbSimulatorDevice->Initialize();
    return status;
}

/* static */
VOID
UwbSimulatorDevice::OnWdfObjectContextCleanup(WDFOBJECT wdfDevice)
{
    auto instance = GetUwbSimulatorDevice(wdfDevice);
    if (instance->m_wdfDevice != wdfDevice) {
        return;
    }

    // Explicitly invoke the destructor since the object was created with placement new.
    instance->Uninitialize();
    instance->~UwbSimulatorDevice();
}

/* static */
NTSTATUS
UwbSimulatorDevice::OnWdfD0Entry(WDFDEVICE device, WDF_POWER_DEVICE_STATE previousState)
{
    auto instance = GetUwbSimulatorDevice(device);
    return instance->OnD0Entry(previousState);
}

/* static */
NTSTATUS
UwbSimulatorDevice::OnWdfD0Exit(WDFDEVICE device, WDF_POWER_DEVICE_STATE targetState)
{
    auto instance = GetUwbSimulatorDevice(device);
    return instance->OnD0Exit(targetState);
}

/* static */
void
UwbSimulatorDevice::OnWdfFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file)
{
    auto instance = GetUwbSimulatorDevice(device);
    instance->OnFileCreate(device, request, file);
}

/* static */
void
UwbSimulatorDevice::OnWdfFileClose(WDFFILEOBJECT file)
{
    auto instance = GetUwbSimulatorDevice(WdfFileObjectGetDevice(file));
    instance->OnFileClose(file);
}

NTSTATUS
UwbSimulatorDevice::Initialize()
{
    // Create a default queue for all requests that are not explicitly configured to go elsewhere.
    WDF_IO_QUEUE_CONFIG queueConfig;
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);
    queueConfig.PowerManaged = WdfFalse;
    queueConfig.EvtIoDeviceControl = &UwbSimulatorIoQueue::OnWdfIoDeviceControl;

    WDF_OBJECT_ATTRIBUTES queueAttributes;
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&queueAttributes, UwbSimulatorIoQueue);

    WDFQUEUE wdfQueue;
    NTSTATUS status = WdfIoQueueCreate(m_wdfDevice, &queueConfig, &queueAttributes, &wdfQueue);
    if (!NT_SUCCESS(status)) {
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "WdfIoQueueCreate failed",
            TraceLoggingLevel(TRACE_LEVEL_FATAL),
            TraceLoggingNTStatus(status));
        return status;
    }

    // Construct a new UwbSimulatorIoQueue instance using the WDF pre-allocated buffer.
    auto ioQueueBuffer = GetUwbSimulatorIoQueue(wdfQueue);
    auto ioQueue = new (ioQueueBuffer) UwbSimulatorIoQueue(wdfQueue);
    if (ioQueue == nullptr) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    status = ioQueue->Initialize();
    if (!NT_SUCCESS(status)) {
        ioQueue->~UwbSimulatorIoQueue();
        return status;
    }

    m_ioQueue = ioQueue;

    return STATUS_SUCCESS;
}

NTSTATUS
UwbSimulatorDevice::Uninitialize()
{
    if (m_ioQueue != nullptr) {
        m_ioQueue->Uninitialize();
        m_ioQueue->~UwbSimulatorIoQueue();
        m_ioQueue = nullptr;
    }

    return STATUS_SUCCESS;
}

void
UwbSimulatorDevice::OnFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "OnFileCreate",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingPointer(device, "Device"),
        TraceLoggingPointer(request, "Request"),
        TraceLoggingPointer(file, "File"));

    auto uwbSimulatorFileBuffer = GetUwbSimulatorFile(file);
    auto uwbSimulatorFile = new (uwbSimulatorFileBuffer) UwbSimulatorDeviceFile(file, this);
    auto uwbSimulatorFileStatus = uwbSimulatorFile->Initialize();
    if (uwbSimulatorFileStatus == STATUS_SUCCESS) {
        uwbSimulatorFile->RegisterHandler(m_ddiHandler);

        std::unique_lock deviceFilesLockExclusive{ m_deviceFilesGate };
        m_deviceFiles.push_back(uwbSimulatorFile);
        DbgPrint("%p added file object %p\n", m_wdfDevice, file);
    } else {
        uwbSimulatorFile->~UwbSimulatorDeviceFile();
    }

    WdfRequestComplete(request, uwbSimulatorFileStatus);
}

void
UwbSimulatorDevice::OnFileClose(WDFFILEOBJECT file)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "OnFileClose",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingPointer(file, "File"));

    std::unique_lock deviceFilesLockExclusive{ m_deviceFilesGate };
    const auto uwbSimulatorFileClosed = GetUwbSimulatorFile(file);
    const auto removed = std::erase_if(m_deviceFiles, [&](const auto &uwbSimulatorFile) {
        return (uwbSimulatorFile == uwbSimulatorFileClosed);
    });

    DbgPrint("%p %s file object %p\n", m_wdfDevice, (removed ? "removed" : "failed to remove"), file);
}

NTSTATUS
UwbSimulatorDevice::OnD0Entry(WDF_POWER_DEVICE_STATE previousState)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "OnD0Entry",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingHexUInt32(previousState, "previousState"));

    return STATUS_SUCCESS;
}

NTSTATUS
UwbSimulatorDevice::OnD0Exit(WDF_POWER_DEVICE_STATE targetState)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "OnD0Exit",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingHexUInt32(targetState, "targetState"));

    return STATUS_SUCCESS;
}

std::tuple<UwbStatus, std::shared_ptr<UwbSimulatorSession>>
UwbSimulatorDevice::SessionCreate(uint32_t sessionId, UwbSessionType sessionType)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionCreate",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"),
        TraceLoggingString(std::data(magic_enum::enum_name(sessionType)), "Session Type"));

    auto session = std::make_shared<UwbSimulatorSession>(sessionId, sessionType);

    std::unique_lock sessionsWriteLock{ m_sessionsGate };
    auto [sessionIt, inserted] = m_sessions.try_emplace(sessionId, session);
    if (!inserted) {
        return { UwbStatusSession::Duplicate, nullptr };
    }

    return { UwbStatusOk, session };
}

std::tuple<UwbStatus, std::shared_ptr<UwbSimulatorSession>>
UwbSimulatorDevice::SessionDestroy(uint32_t sessionId)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionDestroy",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"));

    decltype(m_sessions)::node_type nodeHandle;
    {
        std::unique_lock sessionsWriteLock{ m_sessionsGate };
        nodeHandle = m_sessions.extract(sessionId);
    }
    if (nodeHandle.empty()) {
        return { UwbStatusSession::NotExist, nullptr };
    }

    auto &session = nodeHandle.mapped();

    return { UwbStatusOk, session };
}

std::shared_ptr<windows::devices::uwb::simulator::UwbSimulatorSession>
UwbSimulatorDevice::SessionGet(uint32_t sessionId)
{
    std::shared_lock sessionsReadLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        return nullptr;
    }

    const auto &[_, session] = *sessionIt;
    return session;
}

std::size_t
UwbSimulatorDevice::GetSessionCount()
{
    std::shared_lock sessionsReadLock{ m_sessionsGate };
    return std::size(m_sessions);
}

void
UwbSimulatorDevice::PushUwbNotification(UwbNotificationData uwbNotificationData)
{
    // TODO: log
    // TODO: avoid copies by using std::shared_ptr<> instead
    std::shared_lock deviceFilesLockShared{ m_deviceFilesGate };

    // Distribute a copy of the notification data to each open file handle.
    for (auto &deviceFile : m_deviceFiles) {
        deviceFile->GetIoEventQueue()->PushNotification(uwbNotificationData);
    }
}
