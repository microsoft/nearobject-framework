
#ifndef __SESSION_STATE_HXX__
#define __SESSION_STATE_HXX__

namespace nearobject
{
namespace protocol
{
namespace fira
{
namespace uci
{
/**
 * @brief See FiRa Consortium - UCI Generic Specification v1.1.0, Section 7.
 */
enum class SessionState {
    Deinit,
    Init,
    Idle,
    Active,
};

} // namespace uci
} // namespace fira
} // namespace transport
} // namespace nearobject

#endif // __SESSION_STATE_HXX__
