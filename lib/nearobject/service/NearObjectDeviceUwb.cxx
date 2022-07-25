
#include "NearObjectDeviceUwb.hxx"

using namespace nearobject::service;

NearObjectDevice::StartSessionResult
NearObjectDeviceUwb::StartSessionImpl(const NearObjectConnectionProfile& /* profile */)
{
    return { std::nullopt };
}
