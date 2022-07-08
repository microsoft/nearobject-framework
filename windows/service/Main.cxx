
#include "UwbService.hxx"

int
main(int argc, char *argv[])
{
    windows::uwb::UwbService uwbService{};
    uwbService.Start();
    return 0;
}
