
#ifndef __NEAR_OBJECT_DEVICE_HXX__
#define __NEAR_OBJECT_DEVICE_HXX__

#include <memory>
#include <optional>

#include <nearobject/NearObjectConnectionProfile.hxx>

namespace nearobject
{
class NearObjectSession;

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
     * @return StartSessionResult The result of the operation.
     */
    StartSessionResult
    StartSession(const NearObjectConnectionProfile& profile);

private:
    /**
     * @brief Concrete implementation of StartSession() API.
     * 
     * @param profile 
     * @return StartSessionResult 
     */
    virtual StartSessionResult
    StartSessionImpl(const NearObjectConnectionProfile& profile) = 0;
};

} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_HXX__
