
#include "NearObjectDevice.hxx"

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

using namespace nearobject::service;

NearObjectDevice::StartSessionResult
NearObjectDevice::StartSession(const NearObjectConnectionProfile& profile, std::weak_ptr<NearObjectSessionEventCallbacks> eventCallbacks)
{
    return StartSessionImpl(profile, eventCallbacks);
}
