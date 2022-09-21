
#ifndef SESSION_STATE_HXX
#define SESSION_STATE_HXX

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
} // namespace protocol
} // namespace nearobject

#endif // SESSION_STATE_HXX
