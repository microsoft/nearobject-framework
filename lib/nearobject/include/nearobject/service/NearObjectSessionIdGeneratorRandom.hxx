
#ifndef NEAR_OBJECT_SESSION_ID_GENERATOR_RANDOM_HXX
#define NEAR_OBJECT_SESSION_ID_GENERATOR_RANDOM_HXX

#include <random>

#include <nearobject/service/NearObjectSessionIdGenerator.hxx>

namespace nearobject::service
{
/**
 * @brief Session identifier generator which uses a uniform integer
 * distribution.
 */
struct NearObjectSessionIdGeneratorRandom :
    public NearObjectSessionIdGenerator
{
    uint32_t
    GetNext() noexcept override;

private:
    std::mt19937 m_generator{ std::random_device{}() };
    std::uniform_int_distribution<uint32_t> m_distribution{};
};
} //namespace nearobject::service

#endif // NEAR_OBJECT_SESSION_ID_GENERATOR_RANDOM_HXX
