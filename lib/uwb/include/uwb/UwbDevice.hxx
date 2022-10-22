
#ifndef UWB_DEVICE_HXX
#define UWB_DEVICE_HXX

#include <memory>

namespace uwb
{
class UwbSession;

/**
 * @brief Interface for interacting with a UWB device.
 */
class UwbDevice
{
public:
    /**
     * @brief Creates a new UWB session with no configuration nor peers.
     * 
     * @param sessionId The unique session identifier to use.
     * @return std::unique_ptr<uwb::UwbSession> 
     */
    virtual std::unique_ptr<UwbSession>
    CreateSession(uint32_t sessionId) = 0;
};

} // namespace uwb

#endif // UWB_DEVICE_HXX
