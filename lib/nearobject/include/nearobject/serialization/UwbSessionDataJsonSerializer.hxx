
#ifndef UWB_SESSION_DATA_JSON_SERIALIZER_HXX
#define UWB_SESSION_DATA_JSON_SERIALIZER_HXX

#include <nlohmann/json.hpp>

namespace uwb::protocol::fira
{
struct UwbSessionData;

/**
 * @brief 
 * 
 * @param json 
 * @param uwbSessionData 
 */
void
to_json(nlohmann::json& json, const UwbSessionData& uwbSessionData);

/**
 * @brief 
 * 
 * @param json 
 * @param uwbSessionData 
 */
void
from_json(const nlohmann::json& json, UwbSessionData& uwbSessionData);

} // namespace uwb::protocol::fira

#endif // UWB_SESSION_DATA_JSON_SERIALIZER_HXX