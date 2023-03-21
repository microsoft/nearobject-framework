
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
