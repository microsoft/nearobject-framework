
#include "OobChannel.hxx"

using namespace nearobject::service;

std::unique_ptr<OobConnection>
OobChannel::EstablishConnection()
{
    return EstablishConnectionImpl();
}

std::unique_ptr<OobConnection>
OobChannel::EstablishConnectionImpl()
{
    return {};
}
