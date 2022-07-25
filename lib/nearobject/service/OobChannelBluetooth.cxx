
#include "OobChannelBluetooth.hxx"

#include <stdexcept>

using namespace nearobject::service;

std::unique_ptr<OobConnection>
OobChannelBluetooth::EstablishConnectionImpl()
{
    throw std::logic_error("not implemented");
}
