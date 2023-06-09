
#ifndef UWB_JSON_SERIALIZERS_HXX
#define UWB_JSON_SERIALIZERS_HXX

#include <nlohmann/json.hpp>
#include <uwb/UwbSessionEventCallbacks.hxx>

namespace uwb
{
/**
 * @brief Specialize enum serialization to avoid any potential issues with
 * changes or re-ordering of the underlying values.
 */
// clang-format off
NLOHMANN_JSON_SERIALIZE_ENUM(UwbSessionEndReason, { // NOLINT(*-avoid-c-arrays)
    { UwbSessionEndReason::Stopped, "Stopped" },
    { UwbSessionEndReason::Canceled, "Canceled" },
    { UwbSessionEndReason::Timeout, "Timeout" },
    { UwbSessionEndReason::Unspecified, "Unspecified" },
})
// clang-format on
} // namespace uwb

#endif // UWB_JSON_SERIALIZERS_HXX
