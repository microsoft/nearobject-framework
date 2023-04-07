
#ifndef NEAR_OBJECT_CLI_HXX
#define NEAR_OBJECT_CLI_HXX

#include <functional>
#include <memory>
#include <unordered_set>

#include <CLI/CLI.hpp>
#include <nearobject/cli/NearObjectCliControlFlowContext.hxx>
#include <nearobject/cli/NearObjectCliData.hxx>
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <uwb/UwbDevice.hxx>

namespace nearobject::cli
{
/**
 * @brief Near object command-line (nocli) top-level object.
 *
 * This collects command-line interface functionality, including parsing options
 * and driving execution of the requested operation(s).
 */
class NearObjectCli
{
public:
    /**
     * @brief Construct a new NearObjectCli object.
     *
     * @param cliData An instance to store the parsed data.
     * @param cliHandler An instance to handle parsed command requests.
     */
    NearObjectCli(std::shared_ptr<NearObjectCliData> cliData, std::shared_ptr<NearObjectCliHandler> cliHandler);

    /**
     * @brief Get a reference to the parser. The parser will be configured with
     * all common command line interface options and flags, and may be extended
     * further for the specific application using it.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetParser();

    /**
     * @brief Get the Data object
     *
     * @return std::shared_ptr<NearObjectCliData>
     */
    std::shared_ptr<NearObjectCliData>
    GetData() const noexcept;

    /**
     * @brief Get a shared reference to the the control flow object.
     *
     * @return std::shared_ptr<NearObjectCliControlFlowContext>
     */
    std::shared_ptr<NearObjectCliControlFlowContext>
    GetControlFlowContext() const noexcept;

    /**
     * @brief Parse the specified command line argumenbts.
     *
     * @param argc
     * @param argv
     * @return int
     */
    int
    Parse(int argc, char* argv[]) noexcept;

    /**
     * @brief Wait for all queued operations to complete execution.
     */
    void
    WaitForExecutionComplete();

    /**
     * @brief Cancel all in-progress execution.
     */
    void
    CancelExecution();

    /**
     * @brief Get the app object associated with the "uwb" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetUwbApp() noexcept;

    /**
     * @brief Get the app object associated with the "service" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetServiceApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb range" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetUwbRangeApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb raw" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetUwbRawApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb range start" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetUwbRangeStartApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb range stop" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetUwbRangeStopApp() noexcept;

    /**
     * @brief Get the app object associated with the "service range" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetServiceRangeApp() noexcept;

    /**
     * @brief Get the app object associated with the "service range start" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetServiceRangeStartApp() noexcept;

    /**
     * @brief Get the app object associated with the "service range stop" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetServiceRangeStopApp() noexcept;

private:
    /**
     * @brief Register a CLI::App command that has an operation pending on it.
     *
     * This function should be called once parsing is complete for the CLI::App
     * instance.
     *
     * @param app The CLI::App to register.
     */
    void
    RegisterCliAppWithOperation(CLI::App* app);

    /**
     * @brief Signal that an operation owned by a CLI::App has completed.
     *
     * @param app The app for which the operation completed.
     */
    void
    SignalCliAppOperationCompleted(CLI::App* app);

    /**
     * @brief Obtain a reference to the resolved uwb device.
     *
     * This is only valid if the selected command requires a uwb device. The
     * device is resolved using the names in the following order:
     *
     *  1. Device name as selected from a device probe request via command line.
     *  2. Device name as specified on the command line.
     *  3. Default device name (first device found).
     *
     * @return std::shared_ptr<uwb::UwbDevice>
     */
    std::shared_ptr<uwb::UwbDevice>
    GetUwbDevice() noexcept;

    /**
     * @brief Create an initialize the command line parser. This will create a
     * parser and configure it with all common options and flags.
     *
     * @return std::unique_ptr<CLI::App>
     */
    std::unique_ptr<CLI::App>
    CreateParser() noexcept;

    /**
     * @brief Add the 'uwb' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwb(CLI::App* parent);

    /**
     * @brief Add the 'service' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandService(CLI::App* parent);

    /**
     * @brief Add the 'uwb monitor' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbMonitor(CLI::App* parent);

    /**
     * @brief Add the 'uwb raw' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRaw(CLI::App* parent);

    /**
     * @brief Add the 'uwb range' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRange(CLI::App* parent);

    /**
     * @brief Add the 'service range' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandServiceRange(CLI::App* parent);

    /**
     * @brief Add the 'uwb raw devicereset' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRawDeviceReset(CLI::App* parent);

    /**
     * @brief Add the 'uwb raw getdeviceinfo' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRawGetDeviceInfo(CLI::App* parent);

    /**
     * @brief Add the 'uwb raw sessiondeinit' sub-command.
     *
     * @param parent The parent app to add the command.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRawSessionDeinitialize(CLI::App* parent);

    /**
     * @brief Add the 'uwb raw getsessioncount' sub-command.
     *
     * @param parent The parent app to add the command.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRawGetSessionCount(CLI::App* parent);

    /**
     * @brief Add the 'uwb raw getsessionstate' sub-command.
     *
     * @param parent The parent app to add the command.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRawGetSessionState(CLI::App* parent);

    /**
     * @brief Add the 'uwb range start' sub-command.
     *
     * @param parent
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRangeStart(CLI::App* parent);

    /**
     * @brief Add the 'uwb range stop' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRangeStop(CLI::App* parent);

    /**
     * @brief Add the 'service range start' sub-command.
     *
     * @param parent
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandServiceRangeStart(CLI::App* parent);

    /**
     * @brief Add the 'service range stop' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandServiceRangeStop(CLI::App* parent);

private:
    std::shared_ptr<NearObjectCliData> m_cliData;
    std::shared_ptr<NearObjectCliHandler> m_cliHandler;
    std::shared_ptr<NearObjectCliControlFlowContext> m_cliControlFlowContext;
    std::unordered_set<CLI::App*> m_cliAppOperations;

    std::unique_ptr<CLI::App> m_cliApp;
    // The following are helper references to the subcommands of m_cliApp, the memory is managed by CLI11.
    CLI::App* m_uwbApp;
    CLI::App* m_serviceApp;
    CLI::App* m_monitorApp;
    CLI::App* m_uwbRawApp;
    CLI::App* m_uwbRangeApp;
    CLI::App* m_serviceRangeApp;
    CLI::App* m_uwbRangeStartApp;
    CLI::App* m_uwbRangeStopApp;
    CLI::App* m_serviceRangeStartApp;
    CLI::App* m_serviceRangeStopApp;
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
