
#include <type_traits>
#include <variant>

#include <uwb/UwbDevice.hxx>

using namespace uwb;
using namespace uwb::protocol::fira;

void
UwbDevice::OnStatusChanged([[maybe_unused]] UwbStatus /* status */)
{
    // TODO: implement this
}

void
UwbDevice::OnDeviceStatusChanged([[maybe_unused]] UwbStatusDevice statusDevice)
{
    // TODO: implement this
}

void
UwbDevice::OnSessionStatusChanged([[maybe_unused]] UwbSessionStatus statusSession)
{
    // TODO: implement this
}

void
UwbDevice::OnSessionMulticastListStatus([[maybe_unused]] UwbSessionUpdateMulicastListStatus /* statusMulticastList */)
{
    // TODO: implement this
}

void
UwbDevice::OnSessionRangingData([[maybe_unused]] UwbRangingData rangingData)
{
    // TODO: implement this
}

void
UwbDevice::OnUwbNotification(UwbNotificationData uwbNotificationData)
{
    std::visit([this](auto&& arg) {
        using ValueType = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<ValueType, UwbStatus>) {
            OnStatusChanged(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbStatusDevice>) {
            OnDeviceStatusChanged(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionStatus>) {
            OnSessionStatusChanged(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbSessionUpdateMulicastListStatus>) {
            OnSessionMulticastListStatus(arg);
        } else if constexpr (std::is_same_v<ValueType, UwbRangingData>) {
            OnSessionRangingData(arg);
        }
    },
        uwbNotificationData);
}

bool
uwb::operator==(const UwbDevice& lhs, const UwbDevice& rhs) noexcept
{
    return (typeid(lhs) == typeid(rhs)) && lhs.IsEqual(rhs);
}

bool
uwb::operator!=(const UwbDevice& lhs, const UwbDevice& rhs) noexcept
{
    return !(lhs == rhs);
}
