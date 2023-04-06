#include "pch.h"
#include "winrt/Windows.Devices.NearObject.Service.h"

#include <iostream>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::NearObject::Service;

int main()
{
    init_apartment();
    NearObjectService nearObjectService(nullptr);
    std::cout << "Near Object Service Application started" << std::endl;

    return 0;
}
