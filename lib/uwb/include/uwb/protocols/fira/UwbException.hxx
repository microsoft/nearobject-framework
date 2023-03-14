
#ifndef UWB_EXCEPTION_HXX
#define UWB_EXCEPTION_HXX

#include <utility>

#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb::protocol::fira
{
struct UwbException
{
    explicit UwbException(UwbStatus status) :
        Status(std::move(status))
    {}

    UwbStatus Status{ UwbStatusGeneric::Ok };
};
} // namespace uwb::protocol::fira

#endif // UWB_EXCEPTION_HXX
