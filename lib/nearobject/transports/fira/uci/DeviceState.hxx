
#ifndef __DEVICE_STATE_HXX__
#define __DEVICE_STATE_HXX__

namespace uwb
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

}
} // namespace uwb

#endif // __DEVICE_STATE_HXX__
