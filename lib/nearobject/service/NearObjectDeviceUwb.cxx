
#include "NearObjectDeviceUwb.hxx"

using namespace nearobject::service;


NearObjectDeviceUwb::NearObjectDeviceUwb(std::unique_ptr<uwb::UwbDevice> uwbDevice) :
    m_uwbDevice(std::move(uwbDevice))
{}

NearObjectDevice::StartSessionResult
NearObjectDeviceUwb::StartSessionImpl(const NearObjectConnectionProfile& /* profile */)
{
    return { std::nullopt };
}
