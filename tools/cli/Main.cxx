
#include <nearobject/service/ServiceRunner.hxx>

int
main(int argc, char *argv[])
{
    nearobject::service::Service nearObjectService{};
    nearObjectService.Start();
    return 0;
}
