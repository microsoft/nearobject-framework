
#include <bitset>
#include <stdexcept>
#include <unordered_map>

#include <UwbCxLrpDeviceGlue.h>

#include <windows/uwb/UwbDevice.hxx>
#include <windows/uwb/UwbSession.hxx>

using namespace windows::devices;

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

    for (const auto capability : wil::make_range(&uwbDeviceCapabilities.capabilityParams[0], uwbDeviceCapabilities.numberOfCapabilityParams)) {
        switch (capability.paramType) {
        case UWB_CAPABILITY_PARAM_TYPE_RANGING_METHOD: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<4> rangingConfigurations{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::RangingConfigurationBit, rangingConfigurations, uwbCapability.RangingConfigurations);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_DEVICE_ROLES: {
            const auto value = *reinterpret_cast<const uint8_t*>(&capability.paramValue);
            std::bitset<2> deviceRoles{ value };
            detail::ProcessSupportFromBitset(fira::UwbCapability::DeviceRoleBit, deviceRoles, uwbCapability.DeviceRoles);
            break;
        }
        case UWB_CAPABILITY_PARAM_TYPE_PHY_VERSION_RANGE: {
            const auto  value = *reinterpret_cast<const uint32_t*>(&capability.paramValue);
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
            break;
        }        
    }

    return uwbCapability;
}
} // namespace detail

UwbDevice::UwbDevice(std::wstring deviceName) :
    m_deviceName(std::move(deviceName))
{}

const std::wstring&
UwbDevice::DeviceName() const noexcept
{
    return m_deviceName;
}

void
UwbDevice::Initialize()
{
    wil::unique_hfile handleDriver(CreateFile(
        m_deviceName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        nullptr));

    // TODO: wil::unique_hfile handleDriver(CreateFile(m_deviceName))
    // TODO: call CM_Register_Notification(handleDriver, filterType=CM_NOTIFY_FILTER_TYPE_DEVICEHANDLE)
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVE -> close handleDriver since device removal is requested
    //   - handle CM_NOTIFY_ACTION_DEVICEQUERYREMOVEFAILED -> query removal failed

    m_handleDriver = std::move(handleDriver);
}

std::unique_ptr<uwb::UwbSession>
UwbDevice::CreateSession(uint32_t sessionId)
{
    // Create a duplicate handle to the driver for use by the session.
    wil::unique_hfile handleDriverForSession;
    if (!DuplicateHandle(GetCurrentProcess(), m_handleDriver.get(), GetCurrentProcess(), &handleDriverForSession, 0, FALSE, DUPLICATE_SAME_ACCESS)) {
        return nullptr;
    }

    // Populate the session initialization command argument.
    UWB_SESSION_INIT sessionInit;
    sessionInit.sessionId = sessionId;
    sessionInit.sessionType = UWB_SESSION_TYPE_RANGING_SESSION;

    // Request a new session from the driver.
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SESSION_INIT, &sessionInit, sizeof sessionInit, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        return nullptr;
    }

    return std::make_unique<UwbSession>(sessionId, std::move(handleDriverForSession));
}

uwb::protocol::fira::UwbCapability
UwbDevice::GetCapabilities() const 
{
    // Determine the amount of memory required for the UWB_DEVICE_CAPABILITIES from the driver.
    DWORD bytesRequired = 0;
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, nullptr, 0, &bytesRequired, nullptr);
    if (FAILED(hr)) {
        // TODO: need to do something different here
        return {};
    }

    // Allocate memory for the UWB_DEVICE_CAPABILITIES structure, and pass this to the driver request.
    DWORD uwbCapabilitiesSize = bytesRequired;
    auto uwbDeviceCapabilitiesBuffer = std::make_unique<uint8_t[]>(uwbCapabilitiesSize);
    hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_GET_DEVICE_CAPABILITIES, nullptr, 0, uwbDeviceCapabilitiesBuffer.get(), uwbCapabilitiesSize, &bytesRequired, nullptr);
    if (FAILED(hr)) {
        // TODO: need to do something different here
        return {};
    }

    const UWB_DEVICE_CAPABILITIES& uwbDeviceCapabilities = *reinterpret_cast<UWB_DEVICE_CAPABILITIES*>(uwbDeviceCapabilitiesBuffer.get());
    return detail::FromUwbCx(uwbDeviceCapabilities);
}

bool
UwbDevice::IsEqual(const uwb::UwbDevice& other) const noexcept
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
    const auto& rhs = static_cast<const windows::devices::UwbDevice&>(other);
    return (this->DeviceName() == rhs.DeviceName());
}
