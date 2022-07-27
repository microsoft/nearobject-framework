
#include "NearObjectDiscoverySourceBluetooth.hxx"

#include <stdexcept>

using namespace nearobject::service;

std::unique_ptr<NearObjectOobConnection>
NearObjectDiscoverySourceBluetooth::EstablishConnectionImpl()
{
    throw std::logic_error("not implemented");
}
