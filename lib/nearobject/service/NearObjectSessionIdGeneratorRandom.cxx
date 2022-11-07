
#include <nearobject/service/NearObjectSessionIdGeneratorRandom.hxx>

using namespace nearobject::service;

uint32_t
NearObjectSessionIdGeneratorRandom::GetNext() noexcept
{
    return m_distribution(m_generator);
}
