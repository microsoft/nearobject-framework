
#include "OobChannel.hxx"

using namespace nearobject;

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
