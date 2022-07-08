
#include "OobChannelBluetooth.hxx"

#include <stdexcept>

using namespace uwb;

std::unique_ptr<OobConnection>
OobChannelBluetooth::EstablishConnection()
{
    throw std::logic_error("not implemented");
}