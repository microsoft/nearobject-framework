
#include <future>
#include <ios>
#include <stdexcept>

#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbException.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbDeviceConnector.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

#include <plog/Log.h>
#include <wil/result.h>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

UwbDevice::UwbDevice(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{
    m_callbacks = std::make_shared<::uwb::UwbRegisteredDeviceEventCallbacks>(
        [this](::uwb::protocol::fira::UwbStatus status) {
            return OnStatusChanged(status);
        },
        [this](::uwb::protocol::fira::UwbStatusDevice status) {
            return OnDeviceStatusChanged(status);
        },
        [this](::uwb::protocol::fira::UwbSessionStatus status) {
            return OnSessionStatusChanged(status);
        });
}

const std::string&
UwbDevice::DeviceName() const noexcept
{
    return m_deviceName;
}

std::shared_ptr<::uwb::UwbSession>
UwbDevice::CreateSessionImpl(uint32_t sessionId, std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks)
{
    return std::make_shared<UwbSession>(sessionId, std::move(callbacks), this);
}

std::shared_ptr<::uwb::UwbSession>
UwbDevice::ResolveSessionImpl([[maybe_unused]] uint32_t sessionId)
{
    // TODO: implement this
    // One problem is that m_uwbDeviceConnector is of type
    // IUwbDeviceDdiConnector, which prevents use of the session-based connector
    // functionality. That is needed here to obtain information about the
    // session in question. For example, we need to call
    // IUwbSessionDdi::[SessionGetState, GetApplicationConfigurationParameters,
    // SessionGetRangingCount]. So, we may need to change the type of
    // m_uwbDeviceConnector to UwbConnector instead.
    return nullptr;
}

UwbCapability
UwbDevice::GetCapabilitiesImpl()
{
    auto resultFuture = m_uwbDeviceConnector->GetCapabilities();
    if (!resultFuture.valid()) {
        // TODO: need to do something different than just return a default-constructed object here
        PLOG_ERROR << "failed to obtain capabilities from driver";
        return {};
    }

    try {
        auto [uwbStatus, uwbCapability] = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            PLOG_ERROR << "uwb device reported an error obtaining uwb capabilities, status =" << ::ToString(uwbStatus);
            return {};
        }
        return std::move(uwbCapability);
    } catch (std::exception& e) {
        PLOG_ERROR << "caught exception obtaining uwb capabilities (" << e.what() << ")";
        return {};
    }
}

UwbDeviceInformation
UwbDevice::GetDeviceInformationImpl()
{
    auto resultFuture = m_uwbDeviceConnector->GetDeviceInformation();
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to obtain capabilities from driver";
        throw std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected));
    }

    try {
        auto uwbDeviceInformation = resultFuture.get();
        return std::move(uwbDeviceInformation);
    } catch (const UwbException& e) {
        PLOG_ERROR << "caught exception obtaining uwb device information (" << ::ToString(e.Status) << ")";
        throw e;
    }
}

void
UwbDevice::ResetImpl()
{
    auto resultFuture = m_uwbDeviceConnector->Reset();
    if (!resultFuture.valid()) {
        // TODO: need to do something different than just return a default-constructed object here
        PLOG_ERROR << "failed to reset the uwb device";
        throw UwbException(UwbStatusGeneric::Failed);
    }

    try {
        resultFuture.get();
    } catch (const UwbException& e) {
        PLOG_ERROR << "caught exception resetting the uwb device (" << ::ToString(e.Status) << ")";
        throw e;
    }
}

bool
UwbDevice::InitializeImpl()
{
    m_uwbDeviceConnector = std::make_shared<UwbConnector>(m_deviceName);
    m_callbacksToken = m_uwbDeviceConnector->RegisterDeviceEventCallbacks(m_callbacks);
    m_uwbDeviceConnector->NotificationListenerStart();
    return true;
}

bool
UwbDevice::IsEqual(const ::uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const windows::devices::uwb::UwbDevice&>(other);
    return (this->DeviceName() == rhs.DeviceName());
}
