#include "pch.h"
#include "winrt/Windows.Devices.NearObject.h"
#include "winrt/Windows.Devices.NearObject.Service.h"

#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::NearObject;
using namespace Windows::Devices::NearObject::Service;

int main()
{
    init_apartment();
    NearObject nearObject;
    NearObjectService nearObjectService;
    std::cout << "Near Object Service Application started" << std::endl;

    return 0;
}
