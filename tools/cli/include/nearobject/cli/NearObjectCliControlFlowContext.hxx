
#ifndef NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX
#define NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX

#include <latch>
#include <stop_token>

namespace nearobject::cli
{
/**
 * @brief Class tracking control flow context for the CLI. 
 */
class NearObjectCliControlFlowContext
{
public:
    /**
     * @brief Construct a new Near Object Cli Control Flow Context object
     * 
     * @param numOperations 
     */
    explicit NearObjectCliControlFlowContext(std::ptrdiff_t numOperations);

    /**
     * @brief Get the whole-program exection stop token. 
     * 
     * A request to stop will be signaled on this token when the program should
     * halt execution. This is a cooperative, best-effort based request so may
     * not actually result in halting execution depending on the operation(s)
     * being carried out.
     * 
     * @return std::stop_token 
     */
    std::stop_token
    GetExecutionStopToken();

    /**
     * @brief Get the latch tracking operation completion.
     * 
     * @return std::latch& 
     */
    std::latch&
    GetOperationCompleteLatch();

    /**
     * @brief Signal that execution of an operation is now complete.
     * 
     * @return true If all operations have completed.
     * @return false If at least one operations is incomplete.
     */
    bool
    SignalOperationComplete();

private:
    std::latch m_operationCompleteLatch;
    std::stop_source m_stopSource;
    std::stop_token m_stopToken;
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX
