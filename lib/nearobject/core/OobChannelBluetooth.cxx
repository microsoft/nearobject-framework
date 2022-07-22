
#include "OobChannelBluetooth.hxx"

#include <stdexcept>

using namespace nearobject;

std::unique_ptr<OobConnection>
OobChannelBluetooth::EstablishConnection()
{
    throw std::logic_error("not implemented");
}