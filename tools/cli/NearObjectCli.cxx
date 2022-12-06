
#include <stdexcept>

#include <nearobject/cli/NearObjectCli.hxx>
#include <magic_enum.hpp>

using namespace nearobject::cli;

// bool
// uwb::protocol::fira::lexical_cast(const std::string& input, DeviceRole& dr)
// {
//     std::cout << "called correct lexical_cast function ! val: " << input << std::endl;
//     if (input == magic_enum::enum_name(DeviceRole::Responder)) {
//         dr = DeviceRole::Responder;
//         return true;
//     } else if (input == "Initiator") {
//         dr = DeviceRole::Initiator;
//         return true;
//     }
//     return false;
// }

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
    app->name("nocli");
    app->description("A command line tool to assist with all things nearobject");
    
    app->require_subcommand();
    auto uwbApp = app->add_subcommand("uwb", "commands related to uwb")->require_subcommand()->fallthrough();

    auto rangeApp = uwbApp->add_subcommand("range", "commands related to ranging")->require_subcommand()->fallthrough();

    auto startRangingApp = rangeApp->add_subcommand("start", "start ranging")->fallthrough();

    // option to specify file
    {
        rangeApp->add_option("--file", m_cliData->defaultFile, "file to read as the default values, default is " + m_cliData->defaultFile);
        rangeApp->callback([&]() {
            std::cout << "reading stuff from this file: " << m_cliData->defaultFile << "\n";
        });
    }

    // startRangingApp->add_option("--test", m_cliData->defaultConfiguration, "test");

    

    startRangingApp->add_option("--deviceRole", m_cliData->defaultConfiguration.DeviceRole, "Responder/Initiator");
    
    startRangingApp->add_flag("--controller", m_cliData->hostIsController, "presence of this flag indicates controller, absence means controlee");

    // std::string controleeMac;
    // startRangingApp->add_option("--controleeMac", controleeMac, "assigned mac addres of the controlee");

    startRangingApp->callback([&]{
        std::cout << m_cliData->defaultConfiguration.DeviceRole << "\n";
    });
    
    return app;
}