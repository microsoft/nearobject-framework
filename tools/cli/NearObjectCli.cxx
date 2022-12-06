
#include <stdexcept>

#include <nearobject/cli/NearObjectCli.hxx>

using namespace nearobject::cli;

NearObjectCli::NearObjectCli(std::shared_ptr<NearObjectCliData> cliData) :
    m_cliData(cliData)
{}

CLI::App&
NearObjectCli::GetParser()
{
    if (!m_cliApp) {
        m_cliApp = CreateParser();
        if (!m_cliApp) {
            throw std::runtime_error("failed to create command line parser");
        }
    }

    return *m_cliApp;
}

std::shared_ptr<NearObjectCliData>
NearObjectCli::GetData() const noexcept
{
    return m_cliData;
}

int
NearObjectCli::Parse(int argc, char *argv[])
{
    try {
        m_cliApp->parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return m_cliApp->exit(e);
    }

    return 0;
}

std::unique_ptr<CLI::App>
NearObjectCli::CreateParser()
{
    auto app = std::make_unique<CLI::App>();
    app->name("near object framework command line interface tool");
    app->description("A command line tool to assist with all things nearobject");
    // TODO shaneguan_microsoft: add common options+flags here from your PR
    app->require_subcommand();
    auto uwbApp = app->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

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
    
    return app;
}