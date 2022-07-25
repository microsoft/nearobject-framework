
#ifndef __DEVICE_STATE_HXX__
#define __DEVICE_STATE_HXX__

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
} // namespace transport
} // namespace nearobject

#endif // __DEVICE_STATE_HXX__
