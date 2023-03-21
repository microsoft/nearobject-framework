
#ifndef UWB_SIMULATOR_SESSION_HXX
#define UWB_SIMULATOR_SESSION_HXX

#include <chrono>
#include <functional>
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

    /**
     * @brief Start generation of random uwb ranging data for this session.
     *
     * @param onMeasurementEvent The callback to invoke with the random uwb ranging data.
     */
    void
    RandomRangingMeasurementGenerationStart(std::function<void(UwbRangingData)> onMeasurementEvent);

    /**
     *@brief Stop generation of random uwb ranging measurements for this session.
     */
    void
    RandomRangingMeasurementGenerationStop();

private:
    /**
     * @brief Thread function which generates randomg uwb ranging measurements.
     *
     * The thread generates 1 UwbRangingData (encoded within)
     *
     * @param onMeasurementEvent The callback to invoke with each ranging data set.
     * @param stopToken
     */
    void
    RandomRangingMeasurementGenerator(std::function<void(UwbRangingData)> onMeasurementEvent, std::stop_token stopToken);

    /**
     * @brief Generate a random UwbRangingMeasurement.
     *
     * @return UwbRangingMeasurement
     */
    UwbRangingMeasurement
    GenerateRandomRangingMeasurement();

    /**
     * @brief Generate the next set of uwb ranging data.
     *
     * @return UwbRangingData
     */
    UwbRangingData
    GenerateNextRangingData();

private:
    static constexpr auto DefaultRandomRangingMeasurementsDuration = 1000ms;

    bool m_randomRangingMeasurementsEnabled{ false };
    std::jthread m_randomRangingMeasurementsThread;
    std::chrono::milliseconds m_randomRangingMeasurementsDuration{ DefaultRandomRangingMeasurementsDuration };
    uint32_t m_sequenceNumber{ 0 };
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_SESSION_HXX
