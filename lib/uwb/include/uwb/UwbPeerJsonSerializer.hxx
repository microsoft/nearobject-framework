
#ifndef UWB_PEER_JSON_SERIALIZER_HXX
#define UWB_PEER_JSON_SERIALIZER_HXX

#include <nlohmann/json.hpp>

namespace uwb
{
class UwbPeer;
struct UwbPeerSpatialProperties;

/**
 * @brief 
 * 
 * @param json 
 * @param uwbPeer 
 */
void
to_json(nlohmann::json& json, const UwbPeer& uwbPeer);

/**
 * @brief 
 * 
 * @param json 
 * @param uwbPeer 
 */
void
from_json(const nlohmann::json& json, UwbPeer& uwbPeer);

/**
 * @brief 
 * 
 * @param json 
 * @param uwbPeerSpatialProperties 
 */
void
to_json(nlohmann::json& json, const UwbPeerSpatialProperties& uwbPeerSpatialProperties);

/**
 * @brief 
 * 
 * @param json 
 * @param uwbPeerSpatialProperties 
 */
void
from_json(const nlohmann::json& json, UwbPeerSpatialProperties& uwbPeerSpatialProperties);

} // namespace uwb

#endif // UWB_PEER_JSON_SERIALIZER_HXX
