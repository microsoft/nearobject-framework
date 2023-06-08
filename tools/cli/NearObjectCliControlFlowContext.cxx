
#include <stdexcept>
#include <string>

#include <nearobject/cli/NearObjectCliControlFlowContext.hxx>

using namespace nearobject::cli;

NearObjectCliControlFlowContext::NearObjectCliControlFlowContext(std::ptrdiff_t numOperations) :
    m_operationsCompleteLatchCount(numOperations),
    m_operationCompleteLatch(numOperations),
    m_stopCallback(m_stopSource.get_token(), [&] {
        OnStop();
    })
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
    OnStop();
}

void
NearObjectCliControlFlowContext::RegisterStopCallback(std::function<void()> stopCallback)
{
    std::unique_lock stopCallbacksLock{ m_stopCallbacksGate };
    m_stopCallbacks.push_back(std::move(stopCallback));
}

void
NearObjectCliControlFlowContext::OnStop()
{
    // Move the registered stop callbacks into a local variable. This ensures
    // the stop callbacks are only ever invoked once per program execution
    // cycle.
    decltype(m_stopCallbacks) stopCallbacks;
    {
        std::unique_lock stopCallbacksLock{ m_stopCallbacksGate };
        stopCallbacks = std::move(m_stopCallbacks);
    }

    // Execute the removed stop callbacks.
    for (auto &stopCallback : stopCallbacks) {
        stopCallback();
    }
}
