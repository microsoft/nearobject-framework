
#ifndef SESSION_STATE_HXX
#define SESSION_STATE_HXX

namespace nearobject::protocol::fira::uci
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

} // namespace nearobject::protocol::fira::uci

#endif // SESSION_STATE_HXX
