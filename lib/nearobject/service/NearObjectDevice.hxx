
#ifndef __NEAR_OBJECT_DEVICE_HXX__
#define __NEAR_OBJECT_DEVICE_HXX__

#include <memory>
#include <optional>

#include <nearobject/NearObjectConnectionProfile.hxx>

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
    NearObjectDevice& operator=(NearObjectDevice&) = delete;
    NearObjectDevice& operator=(NearObjectDevice&&) = delete;

    NearObjectDevice() = default;
    virtual ~NearObjectDevice() = default;

    /**
     * @brief Holds the result of the StartSession() function. 
     */
    struct StartSessionResult
    {
        std::optional<std::shared_ptr<NearObjectSession>> Session;
        // TODO: add error details when !Session
    };

    /**
     * @brief Attempt to start a session using the specified connection profile.
     * 
     * @param profile The profile to use to start a session.
     * @param eventCallbacks The callback object to receive event signals.
     * @return StartSessionResult 
     */
    StartSessionResult
    StartSession(const NearObjectConnectionProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks);

private:
    /**
     * @brief Concrete implementation of StartSession() API.
     * 
     * @param profile 
     * @param eventCallbacks 
     * @return StartSessionResult 
     */
    virtual StartSessionResult
    StartSessionImpl(const NearObjectConnectionProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks) = 0;
};

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_HXX__
