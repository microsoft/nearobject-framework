#include <CLI/CLI.hpp>

#include <iostream>

#include <nearobject/service/ServiceRuntime.hxx>
#include <uwb/protocols/fira/UwbConfiguration.hxx>



int
main(int argc, char **argv)
{
    CLI::App top{ "nocli, a comand line tool to assist with all things nearobject" };
    top.require_subcommand();
    auto uwbApp = top.add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

    auto rangeApp = uwbApp->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    auto startRangingApp = rangeApp->add_subcommand("start", "start ranging")->fallthrough();

    // option to specify file
    std::string defaultFile = "default.conf";
    {
        rangeApp->add_option("--file", defaultFile, "file to read as the default values, default is " + defaultFile);
        rangeApp->callback([&]() {
            if (defaultFile.size()) {
                std::cout << "reading stuff from this file: " << defaultFile << "\n";
            }
        });
    }

    uwb::protocol::fira::UwbConfiguration defaultConfiguration; // TODO make this parsed from the defaultFile;

    // std::string deviceRole;
    startRangingApp->add_option("--deviceRole", defaultConfiguration.DeviceRole, "responder/initiator"); // TODO is this really necessary? responder/initiator is strictly for OOB purposes
    
    bool controller{false};
    startRangingApp->add_flag("--controller", controller, "presence of this flag indicates controller, absence means controlee");

    std::string controleeMac;
    startRangingApp->add_option("--controleeMac", controleeMac, "assigned mac addres of the controlee");

    startRangingApp->callback([&]{
        std::cout << defaultConfiguration.DeviceRole;
    });

    CLI11_PARSE(top, argc, argv);

    return 0;
}