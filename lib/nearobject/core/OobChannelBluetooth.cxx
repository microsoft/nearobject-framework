
#include "OobChannelBluetooth.hxx"

#include <stdexcept>

using namespace nearobject;

std::unique_ptr<OobConnection>
OobChannelBluetooth::EstablishConnectionImpl()
{
    throw std::logic_error("not implemented");
}