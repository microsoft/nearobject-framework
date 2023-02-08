
#include <algorithm>
#include <iterator>
#include <mutex>

#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

#include "UwbSimulatorDdiCallbacksLrpNoop.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

void
UwbSimulatorDdiCallbacksLrpNoop::SessionUpdateState(UwbSimulatorSession &session, UwbSessionState sessionState)
{
    session.State = sessionState;

    UWB_NOTIFICATION_DATA notificationData{};
    notificationData.notificationType = UWB_NOTIFICATION_TYPE_SESSION_STATUS;
    auto &sessionStatus = notificationData.sessionStatus;
    sessionStatus.size = sizeof sessionStatus;
    sessionStatus.sessionId = session.Id;
    sessionStatus.state = UwbCxDdi::From(sessionState);
    sessionStatus.reasonCode = UWB_SESSION_REASON_CODE_STATE_CHANGE_WITH_SESSION_MANAGEMENT_COMMANDS;
    // TODO: raise this notification
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceReset()
{
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacksLrpNoop::DeviceGetInformation(UwbDeviceInfoInformation &deviceInformation)
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
UwbSimulatorDdiCallbacksLrpNoop::UwbNotification(UwbNotificationData notificationData)
{
}
