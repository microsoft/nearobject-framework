
#include <future>
#include <ios>
#include <stdexcept>

#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

#include <plog/Log.h>
#include <wil/result.h>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

UwbDevice::UwbDevice(std::string deviceName) :
    m_deviceName(std::move(deviceName))
{}

const std::string&
UwbDevice::DeviceName() const noexcept
{
    return m_deviceName;
}

void
UwbDevice::Initialize()
{
    m_uwbDeviceConnector = std::make_shared<UwbDeviceConnector>(m_deviceName);
    m_uwbDeviceConnector->NotificationListenerStart([this](auto&& uwbNotificationData) {
        // Invoke base class notification handler which takes care of threading.
        ::UwbDevice::OnUwbNotification(std::move(uwbNotificationData));
    });
}

std::shared_ptr<uwb::UwbSession>
UwbDevice::CreateSessionImpl(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks)
{
    return std::make_shared<UwbSession>(std::move(callbacks), m_uwbDeviceConnector);
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
        // TODO: need to do something different than just return a default-constructed object here
        PLOG_ERROR << "failed to obtain capabilities from driver";
        return {};
    }

    try {
        auto [uwbStatus, uwbDeviceInformation] = resultFuture.get();
        if (!IsUwbStatusOk(uwbStatus)) {
            PLOG_ERROR << "uwb device reported an error obtaining uwb device information, status =" << ::ToString(uwbStatus);
            return {};
        }
        return std::move(uwbDeviceInformation);
    } catch (std::exception& e) {
        PLOG_ERROR << "caught exception obtaining uwb device information (" << e.what() << ")";
        return {};
    }
}

bool
UwbDevice::IsEqual(const ::uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const windows::devices::uwb::UwbDevice&>(other);
    return (this->DeviceName() == rhs.DeviceName());
}
