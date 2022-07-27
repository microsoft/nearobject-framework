
#include "NearObjectDiscoverySource.hxx"

using namespace nearobject::service;

std::unique_ptr<OobConnection>
NearObjectDiscoverySource::EstablishConnection()
{
    return EstablishConnectionImpl();
}

std::unique_ptr<OobConnection>
NearObjectDiscoverySource::EstablishConnectionImpl()
{
    return {};
}
