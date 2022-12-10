
#ifndef NEAR_OBJECT_CLI_HXX
#define NEAR_OBJECT_CLI_HXX

#include <memory>

#include <CLI/CLI.hpp>

#include "NearObjectCliData.hxx"

namespace nearobject::cli
{
struct NearObjectCliData;

class NearObjectCli
{
public:
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
    Parse(int argc, char* argv[]);

private:
    /**
     * @brief Create an initialize the command line parser. This will create a
     * parser and configure it with all common options and flags.
     * 
     * @return std::unique_ptr<CLI::App> 
     */
    std::unique_ptr<CLI::App>
    CreateParser();

private:
    std::unique_ptr<CLI::App> m_cliApp;
    std::shared_ptr<NearObjectCliData> m_cliData;
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_HXX
