
#ifndef FIRA_UCI_SESSION_STATE_HXX
#define FIRA_UCI_SESSION_STATE_HXX

namespace uwb::protocol::fira::uci
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

} // namespace uwb::protocol::fira::uci

#endif // FIRA_UCI_SESSION_STATE_HXX
