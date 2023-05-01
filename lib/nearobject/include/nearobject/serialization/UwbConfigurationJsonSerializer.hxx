
#ifndef UWB_CONFIGURATION_JSON_SERIALIZER_HXX
#define UWB_CONFIGURATION_JSON_SERIALIZER_HXX

#include <nlohmann/json.hpp>

namespace uwb::protocol::fira
{
struct UwbConfiguration;

/**
 * @brief
 *
 * @param json
 * @param uwbConfiguration
 */
void
to_json(nlohmann::json& json, const UwbConfiguration& uwbConfiguration);

/**
 * @brief
 *
 * @param json
 * @param uwbConfiguration
 */
void
from_json(const nlohmann::json& json, UwbConfiguration& uwbConfiguration);

} // namespace uwb::protocol::fira

#endif // UWB_CONFIGURATION_JSON_SERIALIZER_HXX