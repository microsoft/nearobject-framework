
#ifndef __SESSION_STATE_HXX__
#define __SESSION_STATE_HXX__

namespace uwb
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

}
} // namespace uwb

#endif // __SESSION_STATE_HXX__