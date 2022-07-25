
#include "NearObjectDevice.hxx"

#include <nearobject/NearObjectSession.hxx>

using namespace nearobject::service;

NearObjectDevice::StartSessionResult
NearObjectDevice::StartSession(const NearObjectConnectionProfile& profile)
{
    return StartSessionImpl(profile);
}
