
#include <iostream>
#include <memory>

#include <CLI/CLI.hpp>

#include <nearobject/cli/NearObjectCli.hxx>
#include "NearObjectCliDataLinux.hxx"

int
main(int argc, char* argv[])
{
    auto cliData = std::make_shared<nearobject::cli::NearObjectCliDataLinux>();
    nearobject::cli::NearObjectCli cli{ cliData };
    CLI::App& app = cli.GetParser();

    // Parse the arguments.
    int result = cli.Parse(argc, argv);
    if (result != 0) {
        return result;
    }

    return 0;
}
