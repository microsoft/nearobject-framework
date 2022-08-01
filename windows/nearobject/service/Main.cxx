
#include <nearobject/service/ServiceRuntime.hxx>

int
main(int argc, char *argv[])
{
    nearobject::service::ServiceRuntime nearObjectServiceRuntime{};
    nearObjectServiceRuntime.Start();
    return 0;
}
