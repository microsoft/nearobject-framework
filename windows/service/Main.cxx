
#include <nearobject/service/Service.hxx>

int
main(int argc, char *argv[])
{
    nearobject::service::Service nearObjectService{};
    nearObjectService.Start();
    return 0;
}
