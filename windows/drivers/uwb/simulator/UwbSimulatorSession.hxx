
#ifndef UWB_SIMULATOR_SESSION_HXX
#define UWB_SIMULATOR_SESSION_HXX

#include <chrono>
#include <thread>

#include <uwb/protocols/fira/FiraDevice.hxx>

#include "IUwbSimulatorSession.hxx"

namespace windows::devices::uwb::simulator
{
enum class RandomMeasurementGeneration {
    Disable,
    Enable,
};

using namespace std::chrono_literals;

using ::uwb::protocol::fira::UwbNotificationData;
using ::uwb::protocol::fira::UwbRangingData;
using ::uwb::protocol::fira::UwbRangingMeasurement;

struct UwbSimulatorSession :
    public IUwbSimulatorSession
{
    UwbSimulatorSession(uint32_t sessionId, UwbSessionType sessionType);

    void
    RandomRangingMeasurementGenerationStart(std::function<void(UwbNotificationData)> onMeasurementEvent);

    void
    RandomRangingMeasurementGenerationStop();

private:
    void
    RandomRangingMeasurementGenerator(std::function<void(UwbNotificationData)> onMeasurementEvent, std::stop_token stopToken);
    
    UwbRangingMeasurement
    GenerateRandomRangingMeasurement();

    UwbRangingData
    GenerateNextRangingData();

private:
    bool m_randomRangingMeasurementsEnabled{ false };
    std::jthread m_randomRangingMeasurementsThread;
    std::chrono::milliseconds m_randomRangingMeasurementsDuration{ 1000ms };
    uint32_t m_sequenceNumber{ 0 };
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_SESSION_HXX
