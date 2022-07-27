
#include "NearObjectDiscoverySource.hxx"

using namespace nearobject::service;

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
