
#ifndef __NEAR_OBJECT_HXX__
#define __NEAR_OBJECT_HXX__

namespace nearobject
{
/**
 * @brief Represents a near object.
 */
class NearObject
{
public:
    /**
     * @brief Determines if the specified near object refers to the same entity
     * as this near object. 
     * 
     * @param other
     * @return true 
     * @return false 
     */
    bool
    IsSame(const NearObject& other) const noexcept;
};

bool
operator==(const NearObject&, const NearObject&) noexcept;

} // namespace nearobject

#endif // __NEAR_OBJECT_HXX__
