
#ifndef NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX
#define NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX

#include <latch>

namespace nearobject::cli
{
class NearObjectCliControlFlowContext
{
public:
    explicit NearObjectCliControlFlowContext(std::ptrdiff_t numOperations);

    std::latch&
    GetOperationCompleteLatch();

private:
    std::latch m_operationCompleteLatch;
};
} // namespace nearobject::cli

#endif // NEAR_OBJECT_CLI_CONTROL_FLOW_CONTEXT_HXX
