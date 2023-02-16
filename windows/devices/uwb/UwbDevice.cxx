
#include <bitset>
#include <future>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <variant>

#include <windows/devices/uwb/UwbCxAdapterDdiLrp.hxx>
#include <windows/devices/uwb/UwbCxDdiLrp.hxx>
#include <windows/devices/uwb/UwbDevice.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

#include <plog/Log.h>

using namespace windows::devices::uwb;
using namespace ::uwb::protocol::fira;

namespace detail
{
using namespace uwb::protocol;

namespace detail
{
/**
 * @brief Helper to process supported parameters from a given map which
 * associates a value with a bit position. If the specified bitmap contains
 * support for the value, the value is added to the result container.
 *
 * @tparam N The size of the bitset.
 * @tparam T The type of the value.
 * @param map The map associating values with bit positions in the bitset.
 * @param support The bitset defining parameter support.
 * @param result The vector to hold supported values that are present in the bitset.
 */
template <std::size_t N, typename T>
void
ProcessSupportFromBitset(std::unordered_map<T, std::size_t> map, const std::bitset<N>& support, std::vector<T>& result)
{
    for (const auto& [value, bit] : map) {
        if (support.test(bit)) {
            result.push_back(value);
        }
    }
}
} // namespace detail

/**
 * @brief Converts a UwbCx UWB_DEVICE_CAPABILTIIES structure to the service C++ FiRa equivalent.
 *
 * @param uwbDeviceCapabilities The device capabilities obtained from the UwbCx driver.
 * @return fira::UwbCapability
 */
fira::UwbCapability
FromUwbCx(const UWB_DEVICE_CAPABILITIES& uwbDeviceCapabilities)
{
    fira::UwbCapability uwbCapability;

    for (const auto capability : wil::make_range(&uwbDeviceCapabilities.capabilityParams[0], uwbDeviceCapabilities.capabilityParamsCount)) {
        switch (capability.paramType) {
        case UWB_CAPABILITY_PARAM_TYPE_RANGING_METHOD: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<4> rangingMethods{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::RangingMethodBit, rangingMethods, uwbCapability.RangingMethods);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_DEVICE_ROLES: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<2> deviceRoles{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::DeviceRoleBit, deviceRoles, uwbCapability.DeviceRoles);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_PHY_VERSION_RANGE: {
            const auto value = *reinterpret_cast<const uint32_t*>(&capability.paramValue);
            uwbCapability.FiraPhyVersionRange = value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_MAC_VERSION_RANGE: {
            const auto value = *reinterpret_cast<const uint32_t*>(&capability.paramValue);
            uwbCapability.FiraMacVersionRange = value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_BLOCK_STRIDING: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<1> blockStriding{ value };
            uwbCapability.BlockStriding = blockStriding.test(fira::UwbCapability::BlockStridingBit);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_HOPPING_MODE: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<1> hoppingMode{ value };
            uwbCapability.HoppingMode = hoppingMode.test(fira::UwbCapability::HoppingModeBit);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_SCHEDULED_MODE: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<2> schedulingModes{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::SchedulingModeBit, schedulingModes, uwbCapability.SchedulingModes);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_RANGING_TIME_STRUCT: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<2> rangingTimeStructs{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::RangingModeBit, rangingTimeStructs, uwbCapability.RangingTimeStructs);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_MULTI_NODE_MODE: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<3> modes{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::MultiNodeModeBit, modes, uwbCapability.MultiNodeModes);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_UWB_INITIATION_TIME: {
            const uint8_t value = capability.paramValue[0];
            uwbCapability.UwbInitiationTime = !!value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_STS_CONFIG: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<3> stsConfigurations{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::StsConfigurationBit, stsConfigurations, uwbCapability.StsConfigurations);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_RFRAME_CONFIG: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<4> rframeConfigurations{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::RFrameConfigurationBit, rframeConfigurations, uwbCapability.RFrameConfigurations);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_AOA_SUPPORT: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<4> aoaTypes{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::AngleOfArrivalBit, aoaTypes, uwbCapability.AngleOfArrivalTypes);
            uwbCapability.AngleOfArrivalFom = aoaTypes.test(fira::UwbCapability::AngleOfArrivalFomBit);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_EXTENDED_MAC_ADDRESS: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            uwbCapability.ExtendedMacAddress = !!value;
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_CC_CONSTRAINT_LENGTH: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<2> convolutionalCodeConstraintLengths{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::ConvolutionalCodeConstraintLengthsBit, convolutionalCodeConstraintLengths, uwbCapability.ConvolutionalCodeConstraintLengths);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_CHANNELS: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<8> channels{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::ChannelsBit, channels, uwbCapability.Channels);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_BPRF_PARAMETER_SETS: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<6> bprfParameterSets{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::BprfParameterSetsBit, bprfParameterSets, uwbCapability.BprfParameterSets);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_HPRF_PARAMETER_SETS: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<35> hprfParameterSets{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::HprfParameterSetsBit, hprfParameterSets, uwbCapability.HprfParameterSets);
            break;
        }
        default:
            // ignore unknown parameter tags
            PLOG_DEBUG << "ignoring unknown UwbCapability parameter tag " << notstd::to_underlying(capability.paramType);
            break;
        }
    }

    return uwbCapability;
}
} // namespace detail

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
    wil::unique_hfile handleDriver(CreateFileA(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr));

    wil::unique_hfile handleDriverNotifications(CreateFileA(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr));

    // TODO: call CM_Register_Notification(handleDriver, filterType=CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE)
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVE -> close handleDriver since device removal is requested
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED -> query removal failed

    m_handleDriver = std::move(handleDriver);
    m_handleDriverNotifications = std::move(handleDriverNotifications);
    m_notificationThread = std::jthread([this] {
        HandleNotifications();
    });
}

/**
 * @brief Namespace alias to reduce typing but preserve clarity regarding DDI
 * conversion.
 */
namespace UwbCxDdi = windows::devices::uwb::ddi::lrp;

void
UwbDevice::HandleNotifications()
{
    for (;;) {
        // TODO: check exit condition

        // Determine the amount of memory required for the UWB_NOTIFICATION_DATA from the driver.
        DWORD bytesRequired = 0;
        HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
        if (FAILED(hr)) {
            PLOG_ERROR << "error determining output buffer size for UWB notification, hr=0x" << std::hex << hr << std::endl;
            continue;
        }

        // Allocate memory for the UWB_NOTIFICATION_DATA structure, and pass this to the driver request.
        DWORD uwbNotificationDataSize = bytesRequired;
        std::vector<uint8_t> uwbNotificationDataBuffer(uwbNotificationDataSize);
        hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_NOTIFICATION, nullptr, 0, std::data(uwbNotificationDataBuffer), uwbNotificationDataSize, &bytesRequired, nullptr);
        if (FAILED(hr)) {
            PLOG_ERROR << "error obtaining UWB notification buffer, hr=0x" << std::hex << hr << std::endl;
            continue;
        }

        // Log a message if the output buffer is not aligned to UWB_NOTIFICATION_DATA. Ignore it for now as this should not occur often.
        if (reinterpret_cast<uintptr_t>(std::data(uwbNotificationDataBuffer)) % alignof(UWB_NOTIFICATION_DATA) != 0) {
            PLOG_ERROR << "driver output buffer is unaligned! undefined behavior may result" << std::endl;
        }

        // Convert to neutral type and process the notification.
        UWB_NOTIFICATION_DATA& notificationData = *reinterpret_cast<UWB_NOTIFICATION_DATA*>(std::data(uwbNotificationDataBuffer));
        auto uwbNotificationData = UwbCxDdi::To(notificationData);

        // Handle the notification in a fire-and-forget fashion. This may change
        // later. Since std::async returns a future, and the future's
        // destructor waits for it to complete, we cannot just ignore the
        // returned future. To work around this, we move the returned future
        // into a shared_ptr, then pass this by value to the std::async's
        // lambda, increasing its reference count. This will ensure the future
        // is automatically destructed once the async lambda has completed.
        auto notificationHandlerFuture = std::make_shared<std::future<void>>();
        *notificationHandlerFuture = std::async(std::launch::async, [this, notificationHandlerFuture, uwbNotificationData = std::move(uwbNotificationData)]() {
            ::UwbDevice::OnUwbNotification(std::move(uwbNotificationData));
        });
    }
}

std::unique_ptr<uwb::UwbSession>
UwbDevice::CreateSession(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks)
{
    // Create a duplicate handle to the driver for use by the session.
    wil::unique_hfile handleDriverForSession;
    if (!DuplicateHandle(GetCurrentProcess(), m_handleDriver.get(), GetCurrentProcess(), &handleDriverForSession, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        return nullptr;
    }

    return std::make_unique<UwbSession>(std::move(callbacks), std::move(handleDriverForSession));
}

uwb::protocol::fira::UwbCapability
UwbDevice::GetCapabilities() const
{
    // Determine the amount of memory required for the UWB_DEVICE_CAPABILITIES from the driver.
    DWORD bytesRequired = 0;
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
    if (FAILED(hr)) {
        // TODO: need to do something different here
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=0x" << std::hex << hr;
        return {};
    }

    // Allocate memory for the UWB_DEVICE_CAPABILITIES structure, and pass this to the driver request.
    DWORD uwbCapabilitiesSize = bytesRequired;
    auto uwbDeviceCapabilitiesBuffer = std::make_unique<uint8_t[]>(uwbCapabilitiesSize);
    hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, uwbDeviceCapabilitiesBuffer.get(), uwbCapabilitiesSize, &bytesRequired, nullptr);
    if (FAILED(hr)) {
        // TODO: need to do something different here
        PLOG_ERROR << "error when sending IOCTL_UWB_GET_DEVICE_CAPABILITIES, hr=0x" << std::hex << hr;
        return {};
    }

    const UWB_DEVICE_CAPABILITIES& uwbDeviceCapabilities = *reinterpret_cast<UWB_DEVICE_CAPABILITIES*>(uwbDeviceCapabilitiesBuffer.get());
    return detail::FromUwbCx(uwbDeviceCapabilities);
}

bool
UwbDevice::IsEqual(const ::uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const windows::devices::uwb::UwbDevice&>(other);
    return (this->DeviceName() == rhs.DeviceName());
}
