
#include <nearobject/discovery/NearObjectDiscoverySourceBluetooth.hxx>

#include <stdexcept>

using namespace nearobject::discovery;

std::unique_ptr<NearObjectOobConnection>
NearObjectDiscoverySourceBluetooth::EstablishConnectionImpl()
{
    throw std::logic_error("not implemented");
}
