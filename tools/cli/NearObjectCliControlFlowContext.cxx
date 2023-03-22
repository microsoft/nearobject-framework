
#include <stdexcept>
#include <string>

#include <nearobject/cli/NearObjectCliControlFlowContext.hxx>

using namespace nearobject::cli;

NearObjectCliControlFlowContext::NearObjectCliControlFlowContext(std::ptrdiff_t numOperations) :
    m_operationsCompleteLatchCount(numOperations),
    m_operationCompleteLatch(numOperations)
{}

std::stop_token
NearObjectCliControlFlowContext::GetExecutionStopToken()
{
    return m_stopSource.get_token();
}

bool
NearObjectCliControlFlowContext::RequestStopExecution()
{
    return m_stopSource.request_stop();
}

bool
NearObjectCliControlFlowContext::OperationSignalComplete(uint16_t numOperationsCompleted)
{
    const bool allOperationsComplete = m_operationCompleteLatch.try_wait();
    if (!allOperationsComplete) {
        if (m_operationsCompleteLatchCount < numOperationsCompleted) {
            const std::string message = "BUG: too many operations were signaled complete (" + std::to_string(numOperationsCompleted) + " requested, " + std::to_string(m_operationsCompleteLatchCount) + "remaining)";
            throw std::runtime_error(message);
        }
        m_operationCompleteLatch.count_down(numOperationsCompleted);
        m_operationsCompleteLatchCount -= numOperationsCompleted;
    }

    return allOperationsComplete;
}

void
NearObjectCliControlFlowContext::OperationsWaitForComplete()
{
    m_operationCompleteLatch.wait();
}
