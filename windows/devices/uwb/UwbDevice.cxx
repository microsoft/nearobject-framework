
#include <future>
#include <ios>
#include <stdexcept>
#include <tuple>

#include <notstd/memory.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>
#include <uwb/protocols/fira/UwbException.hxx>
#include <windows/devices/uwb/UwbConnector.hxx>
#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
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

/* static */
std::shared_ptr<UwbDevice>
UwbDevice::Create(std::string deviceName)
{
    return std::make_shared<notstd::enable_make_protected<UwbDevice>>(std::move(deviceName));
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
UwbDevice::ResolveSessionImpl(uint32_t sessionId)
{
    std::vector<UwbApplicationConfigurationParameterType> applicationConfigurationParameterTypes({ UwbApplicationConfigurationParameterType::DeviceType });

    auto resultFuture = m_uwbSessionConnector->GetApplicationConfigurationParameters(sessionId, applicationConfigurationParameterTypes);
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to obtain application configuration parameters for session id " << sessionId;
        throw UwbException(UwbStatusGeneric::Failed);
    }

    UwbStatus uwbStatus;
    std::vector<UwbApplicationConfigurationParameter> applicationConfigurationParameters;
    try {
        std::tie(uwbStatus, applicationConfigurationParameters) = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            PLOG_ERROR << "failed to obtain device type for session id " << sessionId << " (" << ToString(uwbStatus) << ")";
            throw UwbException(uwbStatus);
        }
    } catch (UwbException& uwbException) {
        PLOG_ERROR << "caught exception attempting to obtain application configuration parameters for session id " << sessionId << " (" << ToString(uwbException.Status) << ")";
        throw uwbException;
    } catch (std::exception& e) {
        PLOG_ERROR << "caught unexpected exception attempting to obtain application configuration parameters for session id " << sessionId << " (" << e.what() << ")";
        throw e;
    }

    // Validate the call provided the expected DeviceType parameter.
    if (std::size(applicationConfigurationParameters) < 1 || !std::holds_alternative<DeviceType>(applicationConfigurationParameters.front().Value)) {
        PLOG_FATAL << "invalid application configuration parameters returned";
        throw std::runtime_error("GetApplicationConfigurationParameters() returned bad data; this is a bug!");
    }

    // Create an instance of the session.
    auto deviceType = std::get<DeviceType>(applicationConfigurationParameters.front().Value);
    auto session = std::make_shared<UwbSession>(sessionId, this, deviceType);

    return session;
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
        PLOG_ERROR << "failed to obtain device information from driver";
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

uint32_t
UwbDevice::GetSessionCountImpl()
{
    auto resultFuture = m_uwbDeviceConnector->GetSessionCount();
    if (!resultFuture.valid()) {
        PLOG_ERROR << "failed to obtain session count from driver";
        throw std::make_exception_ptr(UwbException(UwbStatusGeneric::Rejected));
    }

    try {
        auto [uwbStatus, sessionCount] = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            PLOG_ERROR << "uwb device reported an error obtaining session count, status =" << ::ToString(uwbStatus);
            return {};
        }
        return sessionCount;
    } catch (const UwbException& e) {
        PLOG_ERROR << "caught exception obtaining session count (" << ::ToString(e.Status) << ")";
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
    auto uwbConnector = std::make_shared<UwbConnector>(m_deviceName);
    m_uwbDeviceConnector = uwbConnector;
    m_uwbSessionConnector = uwbConnector;
    m_callbacksToken = m_uwbDeviceConnector->RegisterDeviceEventCallbacks(m_callbacks);
    return true;
}

bool
UwbDevice::IsEqual(const ::uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const windows::devices::uwb::UwbDevice&>(other);
    return (this->DeviceName() == rhs.DeviceName());
}
