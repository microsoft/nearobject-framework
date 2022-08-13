
#ifndef JSON_SERIALIZER_HXX
#define JSON_SERIALIZER_HXX

#include <string>

namespace persist
{
template <typename T>
struct JsonSerializer
{
    virtual ~JsonSerializer() = default;
    JsonSerializer() = default;
    JsonSerializer(const JsonSerializer&) = default;
    JsonSerializer(JsonSerializer&&) = default;
    JsonSerializer& operator=(const JsonSerializer&) = default;
    JsonSerializer& operator=(JsonSerializer&&) = default;

    virtual std::string
    ToJson(const T& object) const = 0;

    virtual T 
    FromJson(const std::string& json) const = 0;
};
} // namespace persist

#endif // JSON_SERIALIZER_HXX
