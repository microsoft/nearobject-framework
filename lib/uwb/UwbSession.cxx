
#include <cstdint>
#include <stdexcept>

#include <plog/Log.h>

#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

using namespace uwb;
using namespace uwb::protocol::fira;
/**
 * @brief map of uci tag to a function of how to generate that uci tag given UwbConfiguration
 *
 */
const std::unordered_map<UwbApplicationConfigurationParameterType, std::function<std::optional<UwbApplicationConfigurationParameterValue>(const UwbConfiguration&)>> uciGenerators{
    // params that directly transfer
    { UwbApplicationConfigurationParameterType::DeviceType, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         //  return config.GetDeviceType();
         // TODO how do we know whether the device is controller or controlee?
         return std::nullopt;
     } },
    { UwbApplicationConfigurationParameterType::DeviceRole, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetDeviceRole();
     } },
    { UwbApplicationConfigurationParameterType::StsConfiguration, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetStsConfiguration();
     } },
    { UwbApplicationConfigurationParameterType::MultiNodeMode, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMultiNodeMode();
     } },
    { UwbApplicationConfigurationParameterType::ChannelNumber, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetChannel();
     } },
    { UwbApplicationConfigurationParameterType::SlotDuration, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetSlotDuration();
     } },
    { UwbApplicationConfigurationParameterType::RangingInterval, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetRangingInterval();
     } },
    { UwbApplicationConfigurationParameterType::MacFcsType, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMacAddressFcsType();
     } },
    { UwbApplicationConfigurationParameterType::RFrameConfiguration, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetRFrameConfig();
     } },
    { UwbApplicationConfigurationParameterType::PreambleCodeIndex, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetPreambleCodeIndex();
     } },
    { UwbApplicationConfigurationParameterType::RangingTimeStruct, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetRangingTimeStruct();
     } },
    { UwbApplicationConfigurationParameterType::SlotsPerRangingRound, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetSlotsPerRangingRound();
     } },
    { UwbApplicationConfigurationParameterType::PrfMode, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetPrfMode();
     } },
    { UwbApplicationConfigurationParameterType::ScheduledMode, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetSchedulingMode();
     } },
    { UwbApplicationConfigurationParameterType::KeyRotationRate, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetKeyRotationRate();
     } },
    { UwbApplicationConfigurationParameterType::MacAddressMode, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMacAddressMode();
     } },
    { UwbApplicationConfigurationParameterType::MaxRangingRoundRetry, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetMaxRangingRoundRetry();
     } },
    { UwbApplicationConfigurationParameterType::UwbInitiationTime, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetUwbInitiationTime();
     } },
    { UwbApplicationConfigurationParameterType::HoppingMode, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         return config.GetHoppingMode();
     } },
    // {  UwbApplicationConfigurationParameterType::ResultReportConfig, []( const UwbConfiguration & config) -> std::optional< UwbApplicationConfigurationParameterValue> {
    //      return config.GetResultReportConfigurations();
    //  } },

    // params with different names
    // {  UwbApplicationConfigurationParameterType::RangingRoundUsage, []( const UwbConfiguration & config) -> std::optional< UwbApplicationConfigurationParameterValue> {
    //      return config.GetRangingMethod();
    //  } },
    { UwbApplicationConfigurationParameterType::DeviceMacAddress, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         auto mode = config.GetMacAddressMode();
         auto role = config.GetDeviceRole();
         if (not mode or not role) {
             return std::nullopt;
         }
         // TODO how do we know if we are controller or controlee?
         return std::nullopt;
     } },
    { UwbApplicationConfigurationParameterType::DestinationMacAddresses, [](const UwbConfiguration& config) -> std::optional<UwbApplicationConfigurationParameterValue> {
         auto mode = config.GetMacAddressMode();
         auto role = config.GetDeviceRole();
         if (not mode or not role) {
             return std::nullopt;
         }
         // TODO how do we know if we are controller or controlee?
         return std::nullopt;
     } },
    // TODO figure out the rest of the uci params
};

std::vector<UwbApplicationConfigurationParameter>
protocol::fira::GetUciConfigParams(const UwbConfiguration& uwbConfiguration)
{
    std::vector<UwbApplicationConfigurationParameter> result;
    for (const auto& [uciTag, generator] : uciGenerators) {
        const auto uciValue = generator(uwbConfiguration);
        if (not uciValue) {
            continue;
        }
        result.push_back(UwbApplicationConfigurationParameter{
            .Type = uciTag,
            .Value = uciValue.value() });
    }
    return result;
}

UwbSession::UwbSession(std::weak_ptr<UwbSessionEventCallbacks> callbacks) :
    m_uwbMacAddressSelf(UwbMacAddress::Random<UwbMacAddressType::Extended>()),
    m_callbacks(std::move(callbacks))
{}

uint32_t
UwbSession::GetId() const noexcept
{
    return m_sessionId;
}

void
UwbSession::SetMacAddressType(UwbMacAddressType uwbMacAddressType) noexcept
{
    m_uwbMacAddressType = uwbMacAddressType;
    // TODO: update driver with new mac address type (aka "mode" in FiRa-speak).
}

void
UwbSession::AddPeer(UwbMacAddress peerMacAddress)
{
    std::scoped_lock peersLock{ m_peerGate };
    PLOG_VERBOSE << "Session with id " << m_sessionId << " requesting to add peer via DDI with mac address " << peerMacAddress.ToString();
    AddPeerImpl(std::move(peerMacAddress));
}

void
UwbSession::Configure(const uint32_t sessionId, const std::vector<protocol::fira::UwbApplicationConfigurationParameter> configParams)
{
    PLOG_VERBOSE << "configure";
    ConfigureImpl(sessionId, configParams);
}

void
UwbSession::StartRanging()
{
    PLOG_VERBOSE << "start ranging";
    bool rangingActiveExpected = false;
    const bool wasRangingInactive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, true);
    if (wasRangingInactive) {
        StartRangingImpl();
    }
}

void
UwbSession::StopRanging()
{
    PLOG_VERBOSE << "stop ranging";
    bool rangingActiveExpected = true;
    const bool wasRangingActive = m_rangingActive.compare_exchange_weak(rangingActiveExpected, false);
    if (wasRangingActive) {
        // StopRangingImpl();
    }
}

void
UwbSession::SetSessionStatus(const uwb::protocol::fira::UwbSessionStatus& status)
{
    PLOG_VERBOSE << "session changed state: " << m_sessionStatus.ToString() << " --> " << status.ToString();
    m_sessionStatus = status;
}

void
UwbSession::InsertPeerImpl(const uwb::UwbMacAddress& peerAddress)
{
    m_peers.insert(peerAddress);
    PLOG_VERBOSE << "Session with id " << m_sessionId << " added peer via DDI with mac address " << peerAddress.ToString();
}