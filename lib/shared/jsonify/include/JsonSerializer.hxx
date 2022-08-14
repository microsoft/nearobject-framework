
#ifndef JSON_SERIALIZER_HXX
#define JSON_SERIALIZER_HXX

#include <string>

#include <nlohmann/json.hpp>

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
    ToJson(const T& object) const
    {
        nlohmann::json json = object;
        return json.dump();
    }

    virtual T 
    FromJson(const std::string& jsonString) const
    {
        nlohmann::json json = nlohmann::json::parse(jsonString);
        return json.get<T>();
    }
};
} // namespace persist

#endif // JSON_SERIALIZER_HXX
