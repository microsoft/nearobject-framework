
#ifndef NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX
#define NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX

#include <latch>

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
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX
