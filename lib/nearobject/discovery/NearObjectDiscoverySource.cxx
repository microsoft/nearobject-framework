
#include <nearobject/discovery/NearObjectDiscoverySource.hxx>

using namespace nearobject::discovery;

std::unique_ptr<NearObjectOobConnection>
NearObjectDiscoverySource::EstablishConnection()
{
    return EstablishConnectionImpl();
}

std::unique_ptr<NearObjectOobConnection>
NearObjectDiscoverySource::EstablishConnectionImpl()
{
    return {};
}
