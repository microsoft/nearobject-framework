
#include <algorithm>
#include <iterator>

#include <magic_enum.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

#include "UwbSimulatorDdiCallbacksLrpNoop.hxx"
#include "UwbSimulatorTracelogging.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

void
UwbSimulatorDdiCallbacksLrpNoop::RaiseUwbNotification(UwbNotificationData uwbNotificationData)
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
        return;
    }

    m_notificationPromise->set_value(std::move(uwbNotificationData));
    m_notificationPromise.reset();
}

void
UwbSimulatorDdiCallbacksLrpNoop::SessionUpdateState(UwbSimulatorSession &session, UwbSessionState sessionState, std::optional<UwbSessionReasonCode> reasonCode = std::nullopt)
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
            .ReasonCode = reasonCode
        }
    };

    RaiseUwbNotification(std::move(notificationData));
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceReset()
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetInformation(UwbDeviceInformation &deviceInformation)
{
    deviceInformation = m_deviceInformation;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetCapabilities(UwbCapability &deviceCapabilities)
{
    deviceCapabilities = m_deviceCapabilities;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetConfigurationParameters(std::vector<UwbDeviceConfigurationParameterType> & /* deviceConfigurationParameterTypes */, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus, std::optional<UwbDeviceConfigurationParameter>>> &deviceConfigurationParameterResults)
{
    deviceConfigurationParameterResults = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceSetConfigurationParameters(const std::vector<UwbDeviceConfigurationParameter> & /* deviceConfigurationParameters */, std::vector<std::tuple<UwbDeviceConfigurationParameterType, UwbStatus>> &deviceConfigurationParameterResults)
{
    deviceConfigurationParameterResults = {};
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionInitialize(uint32_t sessionId, UwbSessionType sessionType)
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
UwbSimulatorDdiCallbacksLrpNoop::SessionDeninitialize(uint32_t sessionId)
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
UwbSimulatorDdiCallbacksLrpNoop::SetApplicationConfigurationParameters(uint32_t /*sessionId*/, const std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> & /* applicationConfigurationParameters */, std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> &applicationConfigurationParameterResults)
{
    std::vector<std::tuple<UwbApplicationConfigurationParameterType, UwbStatus, std::shared_ptr<IUwbAppConfigurationParameter>>> results{};
    applicationConfigurationParameterResults = std::move(results);
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<std::shared_ptr<IUwbAppConfigurationParameter>> &applicationConfigurationParameters)
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
UwbSimulatorDdiCallbacksLrpNoop::GetSessionCount(uint32_t &sessionCount)
{
    std::shared_lock sessionsReadLock{ m_sessionsGate };
    sessionCount = static_cast<uint32_t>(std::size(m_sessions));
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::SessionGetState(uint32_t sessionId, UwbSessionState &sessionState)
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
UwbSimulatorDdiCallbacksLrpNoop::SessionUpdateControllerMulticastList(uint32_t sessionId, std::vector<UwbMacAddress> controlees)
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
UwbSimulatorDdiCallbacksLrpNoop::SessionStartRanging(uint32_t sessionId)
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
UwbSimulatorDdiCallbacksLrpNoop::SessionStopRanging(uint32_t sessionId)
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
UwbSimulatorDdiCallbacksLrpNoop::SessionGetRangingCount(uint32_t /* sessionId */, uint32_t & /* rangingCount */)
{
    return UwbStatusOk;
}

void
UwbSimulatorDdiCallbacksLrpNoop::UwbNotification(UwbNotificationData &notificationData)
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
        return;
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
}
