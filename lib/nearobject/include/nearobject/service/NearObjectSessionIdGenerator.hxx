
#ifndef NEAR_OBJECT_SESSION_ID_GENERATOR_HXX
#define NEAR_OBJECT_SESSION_ID_GENERATOR_HXX

#include <cstdint>

namespace nearobject::service
{
/**
 * @brief Interface for generating session identifiers.
 */
struct NearObjectSessionIdGenerator
{
    /**
     * @brief Provide the next generated session identifier.
     * 
     * @return uint32_t 
     */
    virtual uint32_t
    GetNext() noexcept = 0;

    /**
     * @brief Destroy the NearObjectSessionIdGenerator object.
     */
    virtual ~NearObjectSessionIdGenerator() = default;
};
} // namespace nearobject::service

#endif // NEAR_OBJECT_SESSION_ID_GENERATOR_HXX
