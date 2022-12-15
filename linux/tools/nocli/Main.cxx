
#include <iostream>
#include <memory>

#include <CLI/CLI.hpp>

#include <nearobject/cli/NearObjectCli.hxx>
#include "NearObjectCliDataLinux.hxx"

int
main(int argc, char* argv[])
{
    auto cliData = std::make_shared<nearobject::cli::NearObjectCliDataLinux>();
    auto cliHandler = std::make_shared<nearobject::cli::NearObjectCliHandler>();
    nearobject::cli::NearObjectCli cli{ cliData, cliHandler };

    // Parse the arguments.
    int result = cli.Parse(argc, argv);
    if (result != 0) {
        return result;
    }

    return 0;
}
