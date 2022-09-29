
#ifndef UWB_MAC_ADDRESS_JSON_SERIALIZER_HXX
#define UWB_MAC_ADDRESS_JSON_SERIALIZER_HXX

#include <nlohmann/json.hpp>

namespace uwb
{
class UwbMacAddress;

/**
 * @brief 
 * 
 * @param json 
 * @param uwbMacAddress 
 */
void
to_json(nlohmann::json& json, const UwbMacAddress& uwbMacAddress);

/**
 * @brief 
 * 
 * @param json 
 * @param uwbMacAddress 
 */
void
from_json(const nlohmann::json& json, UwbMacAddress& uwbMacAddress);

} // namespace nearobject

#endif // UWB_MAC_ADDRESS_JSON_SERIALIZER_HXX
