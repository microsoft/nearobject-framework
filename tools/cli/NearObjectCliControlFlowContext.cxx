
#include <nearobject/cli/NearObjectCliControlFlowContext.hxx>

using namespace nearobject::cli;

 NearObjectCliControlFlowContext::NearObjectCliControlFlowContext(std::ptrdiff_t numOperations) :
    m_operationCompleteLatch(numOperations)
{}

std::stop_token
NearObjectCliControlFlowContext::GetExecutionStopToken()
{
    return m_stopSource.get_token();
}

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
