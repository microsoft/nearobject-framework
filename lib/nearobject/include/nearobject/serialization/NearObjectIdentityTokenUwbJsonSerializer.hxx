
#ifndef NEAR_OBJECT_IDENTITY_TOKEN_UWB_JSON_SERIALIZER_HXX
#define NEAR_OBJECT_IDENTITY_TOKEN_UWB_JSON_SERIALIZER_HXX

#include <nlohmann/json.hpp>

namespace nearobject
{
struct NearObjectIdentityTokenUwb;

/**
 * @brief 
 * 
 * @param json 
 * @param identityTokenUwb 
 */
void
to_json(nlohmann::json& json, const NearObjectIdentityTokenUwb& identityTokenUwb);

/**
 * @brief 
 * 
 * @param json 
 * @param identityTokenUwb 
 */
void
from_json(const nlohmann::json& json, NearObjectIdentityTokenUwb& identityTokenUwb);

} // namespace nearobject

#endif // NEAR_OBJECT_IDENTITY_TOKEN_UWB_JSON_SERIALIZER_HXX
