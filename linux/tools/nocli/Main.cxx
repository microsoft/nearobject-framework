
#include <iostream>
#include <memory>

#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include <CLI/CLI.hpp>

#include "NearObjectCliDataLinux.hxx"
#include <nearobject/cli/NearObjectCli.hxx>

#include <LogUtils/LogUtils.hxx>

int
main(int argc, char* argv[])
{
    plog::init(plog::verbose, logging::GetLogName("nocli").c_str());

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
