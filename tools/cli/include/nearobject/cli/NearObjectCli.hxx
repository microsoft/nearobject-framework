
#ifndef NEAR_OBJECT_CLI_HXX
#define NEAR_OBJECT_CLI_HXX

#include <memory>

#include <CLI/CLI.hpp>

#include "NearObjectCliData.hxx"

namespace nearobject::cli
{
struct NearObjectCliData;

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
     */
    explicit NearObjectCli(std::shared_ptr<NearObjectCliData> cliData);

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
     * @brief Create an initialize the command line parser. This will create a
     * parser and configure it with all common options and flags.
     *
     * @return std::unique_ptr<CLI::App>
     */
    std::unique_ptr<CLI::App>
    CreateParser() noexcept;

private:
    std::unique_ptr<CLI::App> m_cliApp;
    std::shared_ptr<NearObjectCliData> m_cliData;

    // the following are helper references to the subcommands of m_cliApp, the memory is managed by CLI11
    CLI::App* m_uwbApp;
    CLI::App* m_rangeApp;
    CLI::App* m_rangeStartApp;
    CLI::App* m_rangeStopApp;
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
