
#include "Service.hxx"

int
main(int argc, char *argv[])
{
    windows::uwb::Service uwbService{};
    uwbService.Start();

    return 0;
}
