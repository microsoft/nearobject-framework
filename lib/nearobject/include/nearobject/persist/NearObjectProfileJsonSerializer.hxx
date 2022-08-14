
#ifndef NEAR_OBJECT_PROFILE_JSON_SERIALIZER
#define NEAR_OBJECT_PROFILE_JSON_SERIALIZER

#include <JsonSerializer.hxx>
#include <nearobject/NearObjectProfile.hxx>

#include <string>

namespace nearobject
{
namespace persistence
{
struct NearObjectProfileJsonSerializer :
    public persist::JsonSerializer<nearobject::NearObjectProfile>
{
    std::string
    ToJson(const NearObjectProfile& profile) const override;

    NearObjectProfile
    FromJson(const std::string& json) const override;
};
} // namespace persistence
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_JSON_SERIALIZER
