
#ifndef DEVICE_STATE_HXX
#define DEVICE_STATE_HXX

namespace nearobject
{
namespace protocol
{
namespace fira
{
namespace uci
{
/**
 * @brief See FiRa Consortium - UCI Generic Specification v1.1.0, Section 5.
 */
enum class DeviceState {
    Ready,
    Active,
    Error,
};

} // namespace uci
} // namespace fira
} // namespace protocol
} // namespace nearobject

#endif // DEVICE_STATE_HXX
