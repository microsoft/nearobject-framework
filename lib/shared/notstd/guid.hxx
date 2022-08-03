
#include <cstring>
#include <utility>

#include <rpc.h>
#include <guiddef.h>

// Note: This is Windows-specific and requires linking to rpcrt4.lib

namespace std
{
    template <>
    struct equal_to<GUID>
    {
        bool operator()(const GUID& lhs, const GUID& rhs) const noexcept
        {
            return std::memcmp(&lhs, &rhs, sizeof lhs) == 0;
        }
    };

    template <>
    struct less<GUID>
    {
        bool operator()(const GUID& lhs, const GUID& rhs) const noexcept
        {
            return std::memcmp(&lhs, &rhs, sizeof lhs) < 0;
        }
    };

    template <>
    struct hash<GUID>
    {
        size_t operator()(const GUID& guid) const noexcept
        {
            RPC_STATUS status = RPC_S_OK;
            return ::UuidHash(const_cast<GUID*>(&guid), &status);
        }
    };
}
