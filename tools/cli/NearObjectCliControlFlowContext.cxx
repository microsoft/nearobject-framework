
#include <nearobject/cli/NearObjectCliControlFlowContext.hxx>

using namespace nearobject::cli;

 NearObjectCliControlFlowContext::NearObjectCliControlFlowContext(std::ptrdiff_t numOperations) :
    m_operationCompleteLatch(numOperations)
{}

std::latch&
NearObjectCliControlFlowContext::GetOperationCompleteLatch()
{
    return m_operationCompleteLatch;
}

bool
NearObjectCliControlFlowContext::SignalOperationComplete()
{
    const bool allOperationsComplete = m_operationCompleteLatch.try_wait();
    if (!allOperationsComplete) {
        m_operationCompleteLatch.count_down(1);
    }

    return allOperationsComplete;
}
