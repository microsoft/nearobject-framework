
#ifndef NEAR_OBJECT_CLI_HANDLER_HXX
#define NEAR_OBJECT_CLI_HANDLER_HXX

#include <cstdint>
#include <memory>

#include <nearobject/cli/NearObjectCliControlFlowContext.hxx>
#include <nearobject/cli/NearObjectCliData.hxx>
#include <nearobject/cli/NearObjectCliUwbSessionEventCallbacks.hxx>
#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>
#include <uwb/protocols/fira/UwbSessionData.hxx>

namespace nearobject::cli
{
class NearObjectCli;
/**
 * @brief Class which handles and executes resolved command-line requests. The
 * command line driver will invoke the function associated with the command line
 * request. Multiple distinct actions may be invoked simultaneously.
 */
struct NearObjectCliHandler
{
    NearObjectCliHandler() = default;

    virtual ~NearObjectCliHandler() = default;

    /**
     * @brief Assign the parent CLI object for this handler.
     *
     * @param parent The (owning) parent object.
     */
    void
    SetParent(NearObjectCli* parent);

    /**
     * @brief Invoked by the command line driver when a UWB device is required
     * for the selected nocli operation. This function must resolve and return
     * an instance of the appropriate UWB device to use for the operation.
     *
     * @param cliData The parsed command-line arguments.
     * @return std::shared_ptr<uwb::UwbDevice>
     */
    virtual std::shared_ptr<uwb::UwbDevice>
    ResolveUwbDevice(const nearobject::cli::NearObjectCliData& cliData) noexcept;

    /**
     * @brief Invoked by command-line driver when the request is to start a
     * ranging session.
     *
     * @param uwbDevice The resolved uwb device to start the ranging session on.
     * @param rangingParameters The parameters needed to configure and start a
     * ranging session.
     */
    virtual void
    HandleDriverStartRanging(std::shared_ptr<uwb::UwbDevice> uwbDevice, const UwbRangingParameters& rangingParameters) noexcept;

    /**
     * @brief Invoked by command-line driver when the request is to start a
     * ranging session.
     *
     * @param uwbDevice The resolved uwb device to start the ranging session on.
     * @param sessionData The data to configure the ranging session.
     */
    virtual void
    HandleStartRanging(std::shared_ptr<uwb::UwbDevice> uwbDevice, uwb::protocol::fira::UwbSessionData& sessionData) noexcept;

    /**
     * @brief Invoked by the command-line driver when the request is to stop an ongoing ranging session.
     *
     * TODO: this will need to change to accept a session id. Or, prior state
     * saved may allow this to remain argumentless.
     */
    virtual void
    HandleStopRanging() noexcept;

    /**
     * @brief Invoked by the command-line driver when monitor mode is requested to be enabled.
     */
    virtual void
    HandleMonitorMode() noexcept;

    /**
     * @brief Invoked by the command-line driver when the request is to reset the device.
     *
     * @param uwbDevice The resolved uwb device to reset.
     */
    virtual void
    HandleDeviceReset(std::shared_ptr<uwb::UwbDevice> uwbDevice) noexcept;

    /**
     * @brief Invoked by the command-line driver when the request is to get device info.
     *
     * @param uwbDevice The resolved uwb device to get device info from.
     */
    virtual void
    HandleGetDeviceInfo(std::shared_ptr<uwb::UwbDevice> uwbDevice) noexcept;

private:
    NearObjectCli* m_parent;
    std::shared_ptr<::uwb::UwbSession> m_activeSession;
    std::shared_ptr<::uwb::UwbSessionEventCallbacks> m_sessionEventCallbacks;
};

} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HANDLER_HXX
