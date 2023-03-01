
#include <algorithm>
#include <iterator>

#include <magic_enum.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

#include "IUwbSimulator.hxx"
#include "UwbSimulatorDdiCallbacks.hxx"
#include "UwbSimulatorTracelogging.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbSimulatorDdiCallbacks::UwbSimulatorDdiCallbacks() :
    m_simulatorCapabilities({ IUwbSimulator::Version })
{}

NTSTATUS
UwbSimulatorDdiCallbacks::RaiseUwbNotification(UwbNotificationData uwbNotificationData)
{
    // Acquire the notification lock to ensure the notification proimise can be safely inspected and updated.
    std::unique_lock notificationLock{ m_notificationGate };

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "UwbNotification",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingString("EventRaised", "Action"),
        TraceLoggingBool(m_notificationPromise.has_value(), "WaitPending"),
        TraceLoggingString(std::data(ToString(uwbNotificationData)), "Data"));

    if (!m_notificationPromise.has_value()) {
        // No outstanding client waiting for a result, so nothing to do.
        return STATUS_INVALID_DEVICE_REQUEST;
    }

    m_notificationPromise->set_value(std::move(uwbNotificationData));
    m_notificationPromise.reset();

    return STATUS_SUCCESS;
}

void
UwbSimulatorDdiCallbacks::SessionUpdateState(UwbSimulatorSession &session, UwbSessionState sessionState, std::optional<UwbSessionReasonCode> reasonCode = std::nullopt)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionStateUpdate",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingString(std::data(magic_enum::enum_name(session.State)), "StatePrevious"),
        TraceLoggingString(std::data(magic_enum::enum_name(sessionState)), "StateNew"));

    session.State = sessionState;

    // Create and raise a notification with the state change.
    UwbNotificationData notificationData{
        UwbSessionStatus{
            .SessionId = session.Id,
            .State = sessionState,
            .ReasonCode = reasonCode }
    };

    RaiseUwbNotification(std::move(notificationData));
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceReset()
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceGetInformation(UwbDeviceInformation &deviceInformation)
{
    deviceInformation = m_deviceInformation;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceGetCapabilities(UwbCapability &deviceCapabilities)
{
    deviceCapabilities = m_deviceCapabilities;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> & /* deviceConfigurationParameterTypes */, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults)
{
    deviceConfigurationParameterResults = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> & /* deviceConfigurationParameters */, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>> &deviceConfigurationParameterResults)
{
    deviceConfigurationParameterResults = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionInitialize(uint32_t sessionId, UwbSessionType sessionType)
{
    std::unique_lock sessionsWriteLock{ m_sessionsGate };
    auto [sessionIt, inserted] = m_sessions.try_emplace(sessionId, sessionId, sessionType);
    if (!inserted) {
        return UwbStatusSession::Duplicate;
    }

    auto &[_, session] = *sessionIt;
    SessionUpdateState(session, UwbSessionState::Initialized);

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionDeninitialize(uint32_t sessionId)
{
    decltype(m_sessions)::node_type nodeHandle;
    {
        std::unique_lock sessionsWriteLock{ m_sessionsGate };
        nodeHandle = m_sessions.extract(sessionId);
    }
    if (nodeHandle.empty()) {
        return UwbStatusSession::NotExist;
    }

    auto &session = nodeHandle.mapped();
    SessionUpdateState(session, UwbSessionState::Deinitialized);

    // TODO: do whatever else is needed for deinitialization

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SetApplicationConfigurationParameters(uint32_t /*sessionId*/, const std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> & /* applicationConfigurationParameters */, std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> &applicationConfigurationParameterResults)
{
    std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> results{};
    applicationConfigurationParameterResults = std::move(results);
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> &applicationConfigurationParameters)
{
    std::shared_lock sessionsReadLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        applicationConfigurationParameters = {};
        return UwbStatusSession::NotExist;
    }

    const auto &[_, session] = *sessionIt;
    applicationConfigurationParameters = session.ApplicationConfigurationParameters;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::GetSessionCount(uint32_t &sessionCount)
{
    std::shared_lock sessionsReadLock{ m_sessionsGate };
    sessionCount = static_cast<uint32_t>(std::size(m_sessions));
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionGetState(uint32_t sessionId, UwbSessionState &sessionState)
{
    std::shared_lock sessionsReadLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        return UwbStatusSession::NotExist;
    }

    const auto &[_, session] = *sessionIt;
    sessionState = session.State;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionUpdateControllerMulticastList(uint32_t sessionId, std::vector<UwbMacAddress> controlees)
{
    std::unique_lock sessionsWriteLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        return UwbStatusSession::NotExist;
    }

    auto &[_, session] = *sessionIt;
    std::move(std::begin(controlees), std::end(controlees), std::inserter(session.Controlees, std::end(session.Controlees)));
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionStartRanging(uint32_t sessionId)
{
    std::unique_lock sessionsWriteLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        return UwbStatusSession::NotExist;
    }

    auto &[_, session] = *sessionIt;
    session.Sequence++;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionStopRanging(uint32_t sessionId)
{
    std::unique_lock sessionsWriteLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        return UwbStatusSession::NotExist;
    }

    auto &[_, session] = *sessionIt;

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionGetRangingCount(uint32_t /* sessionId */, uint32_t & /* rangingCount */)
{
    return UwbStatusOk;
}

NTSTATUS
UwbSimulatorDdiCallbacks::UwbNotification(UwbNotificationData &notificationData)
{
    // Acquire the notification lock to ensure the notification proimise can be safely inspected and updated.
    std::unique_lock notificationLock{ m_notificationGate };
    if (m_notificationPromise.has_value()) {
        // pre-existing promise, this should not happen.
        TraceLoggingWrite(
            UwbSimulatorTraceloggingProvider,
            "UwbNotification",
            TraceLoggingLevel(TRACE_LEVEL_WARNING),
            TraceLoggingString("Ignore", "Action"));
        return STATUS_ALREADY_REGISTERED;
    }

    // Create a new promise whose shared state will be updated when a notification is raised.
    auto notificationFuture = m_notificationPromise.emplace().get_future();

    // Release the lock since the shared data (promise) has been created and future obtained.
    notificationLock.unlock();

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "UwbNotification",
        TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
        TraceLoggingString("Wait", "Action"));

    // Lock is now released; synchronously wait indefinitely for the shared state to be updated.
    notificationFuture.wait();
    notificationData = notificationFuture.get();

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "UwbNotification",
        TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
        TraceLoggingString("WaitComplete", "Action"));

    return STATUS_SUCCESS;
}

UwbSimulatorCapabilities
UwbSimulatorDdiCallbacks::GetSimulatorCapabilities()
{
    return m_simulatorCapabilities;
}

void
UwbSimulatorDdiCallbacks::TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs &triggerSessionEventArgs)
{
    switch (triggerSessionEventArgs.Action) {
    case UwbSimulatorSessionEventAction::RandomRangingMeasurementGenerationStart: {
        SessionRandomMeasurementGenerationConfigure(triggerSessionEventArgs.SessionId, RandomMeasurementGeneration::Enable);
        break;
    }
    case UwbSimulatorSessionEventAction::RandomRangingMeasurementGenerationStop: {
        SessionRandomMeasurementGenerationConfigure(triggerSessionEventArgs.SessionId, RandomMeasurementGeneration::Disable);
        break;
    }
    case UwbSimulatorSessionEventAction::None: {
        break;
    }
    default: {
        break;
    }
    }
}

void
UwbSimulatorDdiCallbacks::SessionRandomMeasurementGenerationConfigure(uint32_t sessionId, RandomMeasurementGeneration action)
{
    std::unique_lock sessionsWriteLock{ m_sessionsGate };
    auto sessionIt = m_sessions.find(sessionId);
    if (sessionIt == std::cend(m_sessions)) {
        return;
    }

    auto &[_, session] = *sessionIt;

    switch (action) {
    case RandomMeasurementGeneration::Disable:
        session.RandomRangingMeasurementGenerationStop();
        break;
    case RandomMeasurementGeneration::Enable:
        session.RandomRangingMeasurementGenerationStart([&](UwbRangingData rangingData) {
            RaiseUwbNotification(std::move(rangingData));
        });
        break;
    }
}
