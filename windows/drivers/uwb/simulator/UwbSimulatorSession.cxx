
#include "UwbSimulatorSession.hxx"

using namespace windows::devices::uwb::simulator;
using namespace uwb::protocol::fira;

UwbSimulatorSession::UwbSimulatorSession(uint32_t sessionId, UwbSessionType sessionType) :
    IUwbSimulatorSession{
        .Id = sessionId,
        .Type = sessionType,
    }
{}

void
UwbSimulatorSession::RandomRangingMeasurementGenerationStart(std::function<void(UwbNotificationData)> onMeasurementEvent)
{
    if (m_randomRangingMeasurementsEnabled) {
        return;
    }

    m_randomRangingMeasurementsThread = std::jthread([this, onMeasurementEvent = std::move(onMeasurementEvent)](std::stop_token stopToken) {
        RandomRangingMeasurementGenerator(std::move(onMeasurementEvent), stopToken);
    });
}

void
UwbSimulatorSession::RandomRangingMeasurementGenerationStop()
{
    m_randomRangingMeasurementsThread.request_stop();
}

UwbRangingMeasurement
UwbSimulatorSession::GenerateRandomRangingMeasurement()
{
    // TODO: some of the below values need to be randomized, others need to be sampled from a fixed set of values
    UwbRangingMeasurement rangingMeasurement{
        .SlotIndex = 1,
        .Distance = 0x0123,
        .Status = UwbStatusGeneric::Ok,
        .PeerMacAddress = ::uwb::UwbMacAddress::Random<::uwb::UwbMacAddressType::Short>(),
        .LineOfSightIndicator = UwbLineOfSightIndicator::LineOfSight,
        .AoAAzimuth = {
            .Result = 0xAAAAU,
        },
        .AoAElevation = {
            .Result = 0xBBBBU,
        },
        .AoaDestinationAzimuth = {
            .Result = 0xCCCCU,
        },
        .AoaDestinationElevation = {
            .Result = 0xDDDDU,
        },
    };

    return rangingMeasurement;
}

UwbRangingData
UwbSimulatorSession::GenerateNextRangingData()
{
    // TODO: number of random ranging measurements need to be bounded by the number of peers involved in the session.
    UwbRangingData rangingData{
        .SequenceNumber = m_sequenceNumber++,
        .SessionId = Id,
        .CurrentRangingInterval = static_cast<uint32_t>(m_randomRangingMeasurementsDuration.count()),
        .RangingMeasurementType = UwbRangingMeasurementType::TwoWay,
        .RangingMeasurements = {
            GenerateRandomRangingMeasurement(),
            GenerateRandomRangingMeasurement(),
            GenerateRandomRangingMeasurement(),
        },
    };

    return rangingData;
}

void
UwbSimulatorSession::RandomRangingMeasurementGenerator(std::function<void(UwbNotificationData)> onMeasurementEvent, std::stop_token stopToken)
{
    while (!stopToken.stop_requested()) {
        std::this_thread::sleep_for(m_randomRangingMeasurementsDuration);
        auto rangingData = GenerateNextRangingData();
        onMeasurementEvent(std::move(rangingData));
    }
}
