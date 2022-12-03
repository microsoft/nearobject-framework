
#include <stdexcept>

#include <nearobject/cli/NearObjectCli.hxx>

using namespace nearobject::cli;

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
    app->name("near object framework command cline interface tool");
    app->description("a command line tool to assist with all things nearobject");
    // TODO shaneguan_microsoft: add common options+flags here from your PR
    return app;
}
