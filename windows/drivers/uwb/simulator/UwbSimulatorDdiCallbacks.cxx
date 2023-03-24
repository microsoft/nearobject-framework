
#include <algorithm>
#include <iterator>
#include <ranges>

#include <magic_enum.hpp>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>

#include "IUwbSimulator.hxx"
#include "UwbSimulatorDdiCallbacks.hxx"
#include "UwbSimulatorDevice.hxx"
#include "UwbSimulatorTracelogging.hxx"

using namespace windows::devices::uwb;
using namespace windows::devices::uwb::simulator;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbSimulatorDdiCallbacks::UwbSimulatorDdiCallbacks(UwbSimulatorDeviceFile *deviceFile) :
    m_simulatorCapabilities({ IUwbSimulator::Version }),
    m_deviceFile(deviceFile)
{}

NTSTATUS
UwbSimulatorDdiCallbacks::RaiseUwbNotification(UwbNotificationData uwbNotificationData)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "UwbNotification",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingString("EventRaised", "Action"),
        TraceLoggingString(std::data(ToString(uwbNotificationData)), "Data"));

    auto ioEventQueue = m_deviceFile->GetIoEventQueue();
    ioEventQueue->PushNotificationData(std::move(uwbNotificationData));

    return STATUS_SUCCESS;
}

std::tuple<UwbStatus, std::shared_ptr<UwbSimulatorSession>>
UwbSimulatorDdiCallbacks::SessionGet(uint32_t sessionId)
{
    auto device = m_deviceFile->GetDevice();
    if (!device) {
        return { UwbStatusGeneric::Rejected, nullptr };
    }

    auto session = device->SessionGet(sessionId);
    if (session == nullptr) {
        return { UwbStatusSession::NotExist, nullptr };
    }

    return {
        UwbStatusGeneric::Ok, session
    };
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
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "DeviceReset",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION));

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceGetInformation(UwbDeviceInformation &deviceInformation)
{
    deviceInformation = m_deviceInformation;

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "DeviceGetInformation",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingString(deviceInformation.ToString().c_str(), "DeviceInformation"));

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::DeviceGetCapabilities(UwbCapability &deviceCapabilities)
{
    deviceCapabilities = m_deviceCapabilities;

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "DeviceGetCapabilities",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingString(deviceCapabilities.ToString().c_str(), "DeviceCapabilities"));

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
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionInitialize",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"),
        TraceLoggingString(std::data(magic_enum::enum_name(sessionType)), "Session Type"));

    auto device = m_deviceFile->GetDevice();
    if (!device) {
        return UwbStatusGeneric::Rejected;
    }

    // Create a new session.
    auto [uwbStatus, session] = device->SessionCreate(sessionId, sessionType);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    SessionUpdateState(*session, UwbSessionState::Initialized);

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionDeninitialize(uint32_t sessionId)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionDeninitialize",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"));

    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    SessionUpdateState(*session, UwbSessionState::Deinitialized);

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameter> applicationConfigurationParameters, std::vector<UwbSetApplicationConfigurationParameterStatus> &applicationConfigurationParameterResults)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SetApplicationConfigurationParameters",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"));

    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    } else if (session->State == UwbSessionState::Deinitialized) {
        return UwbStatusSession::NotExist;
    }

    std::vector<UwbSetApplicationConfigurationParameterStatus> results{};
    // TODO: session exclusive mutex

    // Partition the parameters into those that are expressly disallowed and those that require further checking.
    auto disallowed = std::ranges::partition(applicationConfigurationParameters, [&](const auto &applicationConfigurationParameter) {
        return (session->State == UwbSessionState::Active)
            ? ::uwb::protocol::fira::IsApplicationConfigurationChangeableWhileActive(applicationConfigurationParameter)
            : false;
    });

    // Update result container with all disallowed parameters.
    std::ranges::transform(disallowed, std::back_inserter(results), [&](const auto &applicationConfigurationParameter) {
        return { UwbStatusSession::Active, applicationConfigurationParameter.Type };
    });

    // Process remaining entries.
    std::ranges::transform(std::ranges::begin(applicationConfigurationParameters), std::ranges::begin(disallowed), std::back_inserter(results), [&](auto &applicationConfigurationParameter) {
        // Copy the type for later use in the result tuple.
        auto type = applicationConfigurationParameter.Type;
        
        // Extract the existing entry. If there is no entry, the node will be empty.
        auto node = session->ApplicationConfigurationParameters.extract(applicationConfigurationParameter);
        if (!node.empty()) {
            TraceLoggingWrite(
                UwbSimulatorTraceloggingProvider,
                "SetApplicationConfigurationParameters",
                TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
                TraceLoggingUInt32(sessionId, "Session Id"),
                TraceLoggingString("ParameterUpdate"),
                TraceLoggingString(node.value().ToString().c_str(), "ValueOld"),
                TraceLoggingString(applicationConfigurationParameter.ToString().c_str(), "Value"));
        } else {
            TraceLoggingWrite(
                UwbSimulatorTraceloggingProvider,
                "SetApplicationConfigurationParameters",
                TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
                TraceLoggingUInt32(sessionId, "Session Id"),
                TraceLoggingString("ParameterSet"),
                TraceLoggingString(applicationConfigurationParameter.ToString().c_str(), "Value"));
        }
        
        // Update the node with the current parameter value.
        node.value() = std::move(applicationConfigurationParameter);
        session->ApplicationConfigurationParameters.insert(std::move(node));

        // Update result container indicating setting the parameter was successful.
        return { UwbStatusGeneric::Ok, type };
    });

    applicationConfigurationParameterResults = std::move(results);
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::GetApplicationConfigurationParameters(uint32_t sessionId, std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes, std::vector<UwbApplicationConfigurationParameter> &applicationConfigurationParameters)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "GetApplicationConfigurationParameters",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"));

    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }
    
    // TODO: session shared mutex
    std::ranges::copy_if(session->ApplicationConfigurationParameters, std::back_inserter(applicationConfigurationParameters), [&](const auto &entry) {
        return std::ranges::any_of(applicationConfigurationParameterTypes, [&](const auto &type) {
            return (entry.Type == type);
        });
    });

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::GetSessionCount(uint32_t &sessionCount)
{
    auto device = m_deviceFile->GetDevice();
    if (!device) {
        return UwbStatusGeneric::Rejected;
    }

    sessionCount = static_cast<uint32_t>(device->GetSessionCount());

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "GetSessionCount",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionCount, "Session Count"));

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionGetState(uint32_t sessionId, UwbSessionState &sessionState)
{
    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    sessionState = session->State;

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionGetState",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"),
        TraceLoggingString(std::data(magic_enum::enum_name(sessionState)), "Session State"));

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionUpdateControllerMulticastList(uint32_t sessionId, UwbMulticastAction action, std::vector<UwbSessionUpdateMulticastListEntry> updateMulticastListEntries)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionUpdateControllerMulticastList",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"),
        TraceLoggingString(std::data(magic_enum::enum_name(action)), "Multicast Action"));

    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    // TODO: session exclusive mutex
    const auto getControlee = [](const UwbSessionUpdateMulticastListEntry &entry) {
        return entry.ControleeMacAddress;
    };

    switch (action) {
    case UwbMulticastAction::AddShortAddress: {
        // TODO: updateMulticastListEntry.SubSessionId needs to be handled in future.
        std::ranges::move(updateMulticastListEntries | std::views::transform(getControlee), std::inserter(session->Controlees, std::end(session->Controlees)));
        break;
    }
    case UwbMulticastAction::DeleteShortAddress: {
        // TODO: updateMulticastListEntry.SubSessionId needs to be handled in future.
        std::erase_if(session->Controlees, [&](const auto &controleeToRemove) {
            return std::ranges::any_of(updateMulticastListEntries | std::views::transform(getControlee), [&](const auto &controlee) {
                return controleeToRemove == controlee;
            });
        });
        break;
    }
    default:
        return UwbStatusGeneric::InvalidParameter;
    }

    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionStartRanging(uint32_t sessionId)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionStartRanging",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"));

    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    // TODO: session exclusive mutex
    session->RangingCount++;
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionStopRanging(uint32_t sessionId)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionStopRanging",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"));

    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    // TODO: session exclusive mutex
    SessionUpdateState(*session, UwbSessionState::Idle, UwbSessionReasonCode::StateChangeWithSessionManagementCommands);
    
    return UwbStatusOk;
}

UwbStatus
UwbSimulatorDdiCallbacks::SessionGetRangingCount(uint32_t sessionId, uint32_t &rangingCount)
{
    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return uwbStatus;
    }

    rangingCount = session->RangingCount;

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "SessionGetRangingCount",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(sessionId, "Session Id"),
        TraceLoggingUInt32(rangingCount, "Session Ranging Count"));

    return UwbStatusOk;
}

NTSTATUS
UwbSimulatorDdiCallbacks::UwbNotification(UwbNotificationData & /*notificationData*/)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "UwbNotification",
        TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
        TraceLoggingString("Wait", "Action"));

    // This callback should not be invoked directly.
    NTSTATUS status = STATUS_NOT_IMPLEMENTED;

    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "UwbNotification",
        TraceLoggingLevel(TRACE_LEVEL_VERBOSE),
        TraceLoggingString("WaitComplete", "Action"));

    return status;
}

UwbSimulatorCapabilities
UwbSimulatorDdiCallbacks::GetSimulatorCapabilities()
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "GetSimulatorCapabilities",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingHexUInt32(m_simulatorCapabilities.Version, "Version"));
    return m_simulatorCapabilities;
}

void
UwbSimulatorDdiCallbacks::TriggerSessionEvent(const UwbSimulatorTriggerSessionEventArgs &triggerSessionEventArgs)
{
    TraceLoggingWrite(
        UwbSimulatorTraceloggingProvider,
        "TriggerSessionEvent",
        TraceLoggingLevel(TRACE_LEVEL_INFORMATION),
        TraceLoggingUInt32(triggerSessionEventArgs.SessionId, "Session Id"),
        TraceLoggingString(std::data(magic_enum::enum_name(triggerSessionEventArgs.Action)), "Action"));

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
    auto [uwbStatus, session] = SessionGet(sessionId);
    if (!IsUwbStatusOk(uwbStatus)) {
        return;
    }

    switch (action) {
    case RandomMeasurementGeneration::Disable:
        session->RandomRangingMeasurementGenerationStop();
        break;
    case RandomMeasurementGeneration::Enable:
        session->RandomRangingMeasurementGenerationStart([&](UwbRangingData rangingData) {
            RaiseUwbNotification(std::move(rangingData));
        });
        break;
    }
}
