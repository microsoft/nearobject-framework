
#ifndef NOTSTD_UNIQUE_PTR_OUT_HXX
#define NOTSTD_UNIQUE_PTR_OUT_HXX

#include <memory>

namespace notstd
{
/**
 * @brief Adapts using a std::unique_ptr as an output pointer parameter.
 * 
 * This allows clean transfer of pointer ownership with functions that return
 * raw pointers, such as:
 * 
 *     void createThing(Thing** theThing);
 * 
 * Without this adaption code, a temporary raw pointer must be used as below:
 * 
 *     Thing *theThingPtr
 *     createThing(&theThingPtr);
 *     std::unique_ptr<Thing> theThing(theThingPtr);
 * 
 * This adapter eliminates the temporary:
 * 
 *     std::unique_ptr<Thing> theThing;
 *     createThing(notstd::unique_ptr_out(theThing));
 * 
 * The idea of this code was heavily inspired by wil::out_param, however, it is
 * considerably simpler and less generic.
 * 
 * @tparam T The type the pointer references. 
 */
template <typename T>
struct unique_ptr_out
{
    std::unique_ptr<T>& wrapper;
    T* pointer_raw;

    explicit unique_ptr_out(std::unique_ptr<T>& output) : 
        wrapper(output),
        pointer_raw(nullptr)
    {}

    operator T**()
    {
        return &pointer_raw;
    } 

    ~unique_ptr_out()
    {
        wrapper.reset(pointer_raw);
    }

    unique_ptr_out(unique_ptr_out const& other) = delete;
    unique_ptr_out &operator=(unique_ptr_out const& other) = delete;
    unique_ptr_out(unique_ptr_out&& other) = delete;
};
} // namespace notstd

#endif // NOTSTD_UNIQUE_PTR_OUT_HXX
