
#ifndef __NEAR_OBJECT_DEVICE_HXX__
#define __NEAR_OBJECT_DEVICE_HXX__

namespace nearobject
{
namespace service
{
class NearObjectDevice
{
public:
    // Disable copy and move operations
    NearObjectDevice(NearObjectDevice&) = delete;
    NearObjectDevice(NearObjectDevice&&) = delete;
    NearObjectDevice& operator=(NearObjectDevice&) = delete;
    NearObjectDevice& operator=(NearObjectDevice&&) = delete;

    bool
    operator==(const NearObjectDevice& other) const noexcept;

    bool
    IsSame(const NearObjectDevice& other) const noexcept;

private:
    virtual bool
    IsSameImpl(const NearObjectDevice& other) const noexcept;
};
} // namespace service
} // namespace nearobject

#endif // _NEAR_OBJECT_DEVICE_HXX__
