
#include <utility>

#include <CLI/CLI.hpp>

#include <nearobject/service/NearObjectServiceConfiguration.hxx>

using namespace nearobject::service;

namespace details
{
CLI::App&
ConfigureCliAppOptions(CLI::App& app, NearObjectServiceConfiguration& configuration)
{
    app.name("Near Object Service");
    app.description("Service supporting interaction with near objects");

    // Configure supported command-line program options.
    app.add_flag(
        "-d",
        configuration.RunInBackground,
        "Run the service as a daemon in the background");
    app.add_flag(
        "-v",
        configuration.VerboseLogging,
        "Enable logging verbosity");

    return app;
}

template<typename ...Args>
NearObjectServiceConfiguration ParseCliAppOptions(bool throwOnParseError, Args&& ...args)
{
    CLI::App app{};
    NearObjectServiceConfiguration configuration{};

    ConfigureCliAppOptions(app, configuration);

    try {
        app.parse(std::forward<Args>(args)...);
    } catch (const CLI::ParseError &parseError) {
        if (throwOnParseError) {
            throw parseError; // NOLINT(cert-err60-cpp)
        }
    }

    return configuration;
}
} // namespace details

// NOLINTBEGIN(*-avoid-c-arrays)
/* static */
NearObjectServiceConfiguration
NearObjectServiceConfiguration::FromCommandLineArguments(int argc, char *argv[], bool throwOnParseError)
{
    return details::ParseCliAppOptions(throwOnParseError, argc, argv);
}
// NOLINTEND(*-avoid-c-arrays)

/* static */
NearObjectServiceConfiguration
NearObjectServiceConfiguration::FromCommandLineArguments(std::vector<std::string>& args, bool throwOnParseError)
{
    return details::ParseCliAppOptions(throwOnParseError, args);
}
