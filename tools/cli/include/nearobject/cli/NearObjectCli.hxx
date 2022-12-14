
#ifndef NEAR_OBJECT_CLI_HXX
#define NEAR_OBJECT_CLI_HXX

#include <functional>
#include <memory>

#include <CLI/CLI.hpp>
#include <nearobject/cli/NearObjectCliHandler.hxx>
#include <nearobject/cli/NearObjectCliData.hxx>
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
     * @brief Parse the specified command line argumenbts.
     *
     * @param argc
     * @param argv
     * @return int
     */
    int
    Parse(int argc, char* argv[]) noexcept;

    /**
     * @brief Get the app object associated with the "uwb" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetUwbApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb range" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetRangeApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb range start" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetRangeStartApp() noexcept;

    /**
     * @brief Get the app object associated with the "uwb range stop" sub-command.
     *
     * @return CLI::App&
     */
    CLI::App&
    GetRangeStopApp() noexcept;

private:
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
     * @brief Add the 'uwb range' sub-command.
     *
     * @param parent The parent app to add the command to.
     * @return CLI::App*
     */
    CLI::App*
    AddSubcommandUwbRange(CLI::App* parent);

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

private:
    std::shared_ptr<NearObjectCliData> m_cliData;
    std::shared_ptr<NearObjectCliHandler> m_cliHandler;

    // Resolved device instance, if applicable to the selected command.
    std::shared_ptr<uwb::UwbDevice> m_uwbDevice;

    std::unique_ptr<CLI::App> m_cliApp;
    // The following are helper references to the subcommands of m_cliApp, the memory is managed by CLI11.
    CLI::App* m_uwbApp;
    CLI::App* m_rangeApp;
    CLI::App* m_rangeStartApp;
    CLI::App* m_rangeStopApp;
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
