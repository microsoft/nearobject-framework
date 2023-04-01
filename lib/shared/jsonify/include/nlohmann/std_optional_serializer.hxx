
#ifndef STD_OPTIONAL_SERIALIZER
#define STD_OPTIONAL_SERIALIZER

#include <optional>

#include <nlohmann/json.hpp>

NLOHMANN_JSON_NAMESPACE_BEGIN

template <typename T>
struct adl_serializer<std::optional<T>>
{
    static void
    to_json(json& j, const std::optional<T>& opt)
    {
        if (!opt.has_value()) {
            j = nullptr;
        } else {
            j = opt.value();
        }
    }

    static void
    from_json(const json& j, std::optional<T>& opt)
    {
        if (j.is_null()) {
            opt = std::nullopt;
        } else {
            opt = j.get<T>();
        }
    }
};

NLOHMANN_JSON_NAMESPACE_END

#endif // STD_OPTIONAL_SERIALIZER
