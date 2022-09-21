
#ifndef DEVICE_STATE_HXX
#define DEVICE_STATE_HXX

namespace nearobject::protocol::fira::uci
{
/**
 * @brief See FiRa Consortium - UCI Generic Specification v1.1.0, Section 5.
 */
enum class DeviceState {
    Ready,
    Active,
    Error,
};

} // namespace nearobject::protocol::fira::uci

#endif // DEVICE_STATE_HXX
