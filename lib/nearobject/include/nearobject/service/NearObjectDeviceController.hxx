
#ifndef NEAR_OBJECT_DEVICE_CONTROLLER_HXX
#define NEAR_OBJECT_DEVICE_CONTROLLER_HXX

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
class NearObjectDeviceController
{
public:
    virtual ~NearObjectDeviceController() = default;
    NearObjectDeviceController(NearObjectDeviceController&) = delete;
    NearObjectDeviceController(NearObjectDeviceController&&) = delete;
    NearObjectDeviceController&
    operator=(NearObjectDeviceController&) = delete;
    NearObjectDeviceController&
    operator=(NearObjectDeviceController&&) = delete;

    /**
     * @brief Construct a new NearObjectDeviceController object.
     */
    NearObjectDeviceController() = default;

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
     * @brief Determines if this controller is equal to another.
     * 
     * @param other 
     * @return true 
     * @return false 
     */
    virtual bool
    IsEqual(const NearObjectDeviceController& other) const noexcept = 0;

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
    std::mutex m_sessionsGate;
    std::vector<std::weak_ptr<NearObjectSession>> m_sessions{};
};

bool
operator==(const NearObjectDeviceController&, const NearObjectDeviceController&) noexcept;

bool
operator!=(const NearObjectDeviceController&, const NearObjectDeviceController&) noexcept;

} // namespace service
} // namespace nearobject

#endif // NEAR_OBJECT_DEVICE_CONTROLLER_HXX
