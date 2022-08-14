
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
};
} // namespace persistence
} // namespace nearobject

#endif // NEAR_OBJECT_PROFILE_JSON_SERIALIZER
