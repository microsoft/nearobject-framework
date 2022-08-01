
#ifndef NEAR_OBJECT_DEVICE_HXX
#define NEAR_OBJECT_DEVICE_HXX

#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include <nearobject/NearObjectProfile.hxx>

namespace nearobject
{
class NearObjectSession;
struct NearObjectSessionEventCallbacks;

namespace service
{
/**
 * @brief A device providing Near Object services.
 */
class NearObjectDevice
{
public:
    // Disable copy and move operations
    NearObjectDevice(NearObjectDevice&) = delete;
    NearObjectDevice(NearObjectDevice&&) = delete;
    NearObjectDevice&
    operator=(NearObjectDevice&) = delete;
    NearObjectDevice&
    operator=(NearObjectDevice&&) = delete;

    NearObjectDevice() = default;
    virtual ~NearObjectDevice() = default;

    /**
     * @brief Construct a new Near Object Device object
     *
     * @param deviceId
     */
    explicit NearObjectDevice(uint64_t deviceId);

    /**
     * @brief Holds the result of the StartSession() function.
     */
    struct StartSessionResult
    {
        std::optional<std::shared_ptr<NearObjectSession>> Session;
        // TODO: add error details when !Session
    };

    /**
     * @brief Attempt to start a session using the specified profile.
     *
     * @param profile The profile to use to start a session.
     * @param eventCallbacks The callback object to receive event signals.
     * @return StartSessionResult
     */
    StartSessionResult
    StartSession(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

    /**
     * @brief Returns a value uniquely identifying this device.
     *
     * @return uint64_t
     */
    uint64_t
    Id() const noexcept;

private:
    /**
     * @brief Concrete implementation of StartSession() API.
     *
     * @param profile
     * @param eventCallbacks
     * @return StartSessionResult
     */
    virtual StartSessionResult
    StartSessionImpl(const NearObjectProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) = 0;

private:
    const uint64_t m_deviceId;

protected:
    std::mutex m_sessionsGate;
    std::vector<std::weak_ptr<NearObjectSession>> m_sessions{};
};

bool
operator==(const NearObjectDevice&, const NearObjectDevice&) noexcept;

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_HXX
