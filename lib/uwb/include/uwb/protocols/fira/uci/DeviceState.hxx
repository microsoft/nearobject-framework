
#ifndef FIRA_UCI_DEVICE_STATE_HXX
#define FIRA_UCI_DEVICE_STATE_HXX

namespace uwb::protocol::fira::uci
{
/**
 * @brief See FiRa Consortium - UCI Generic Specification v1.1.0, Section 5.
 */
enum class DeviceState {
    Ready,
    Active,
    Error,
};

} // namespace uwb::protocol::fira::uci

#endif // FIRA_UCI_DEVICE_STATE_HXX
