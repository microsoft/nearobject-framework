
#include <algorithm>
#include <cstring>
#include <iterator>
#include <optional>
#include <string>
#include <utility>

#include <rpc.h>
#include <guiddef.h>

#include <wil/win32_helpers.h>

namespace notstd
{
/**
 * @brief Returns a string-representation of a GUID, without enclosing braces.
 * Eg. "FB49FD78-D7EA-4E3E-B3D0-5A19FE5C7EC3"
 * 
 * @tparam TString 
 * @param guid 
 * @return TString 
 */
template <typename TString>
inline TString
GuidToString(const GUID& guid);

/**
 * @brief Specialization for converting a std::wstring to a GUID.
 * 
 * @tparam std::wstring 
 * @param guid 
 * @return std::wstring 
 */
template <>
inline std::wstring
GuidToString(const GUID& guid)
{
    // Note: std::wstring allocates size + 1 to account for
    // adding a '\0', so using `guid_string_length` which doesn't
    // include space for null-terminator is correct.
    std::wstring guidString(wil::guid_string_length, L'\0');
    if (StringFromGUID2(guid, std::data(guidString), static_cast<int>(guidString.size()) + 1) == 0) {
        return {};
    }

    // Remove enclosing {}
    guidString.pop_back();
    guidString.erase(std::begin(guidString));

    return guidString;
}

/**
 * @brief Specialization for converting a std::string to a GUID.
 * 
 * @tparam std::string
 * @param guid 
 * @return std::string 
 */
template <>
inline std::string
GuidToString(const GUID& guid)
{
    const auto wide = GuidToString<std::wstring>(guid);

    // Do a 1-1 transformation using the wide-character values directly for the
    // ASCII string. This is safe because GUIDs only contain alphanumeric
    // characters and the dash (-), all of which are contained in the ASCII
    // charcter set, and thus have 8-bit values.
    std::string ascii(wide.length(), '\0');
    std::transform(std::cbegin(wide), std::cend(wide), std::begin(ascii), [](const auto& wc) {
        return static_cast<std::string::value_type>(wc);
    });

    return ascii;
}

/**
 * @brief Parses a string for a GUID and returns it, if valid.
 * 
 * @tparam TString The type of input string to parse.
 * @param input The string to parse.
 * @return std::optional<GUID> An optional containing a GUID if the input was valid, std::nullopt otherwise.
 */
template <typename TString>
inline std::optional<GUID>
GuidFromString(const TString& input) noexcept;

/**
 * @brief Specialization for parsing a GUID from std::string.
 * 
 * @tparam TString std::string
 * @param input 
 * @return std::optional<GUID> 
 */
template <>
inline std::optional<GUID>
GuidFromString(const std::string& input) noexcept
{
    GUID guid{};
    auto result = UuidFromStringA(reinterpret_cast<RPC_CSTR>(const_cast<std::string::value_type*>(std::data(input))), &guid);
    if (result != NOERROR) {
        return std::nullopt;
    }

    return guid;
}

/**
 * @brief Specialization for parsing a GUID from std::wstring.
 * 
 * @tparam TString std::wstring
 * @param input 
 * @return std::optional<GUID> 
 */
template <>
inline std::optional<GUID>
GuidFromString(const std::wstring& input) noexcept
{
    GUID guid{};
    auto result = UuidFromStringW(reinterpret_cast<RPC_WSTR>(const_cast<std::wstring::value_type*>(std::data(input))), &guid);
    if (result != NOERROR) {
        return std::nullopt;
    }

    return guid;
}
} // namespace notstd 

namespace std
{
template <>
struct equal_to<GUID>
{
    bool
    operator()(const GUID& lhs, const GUID& rhs) const noexcept
    {
        return std::memcmp(&lhs, &rhs, sizeof lhs) == 0;
    }
};

template <>
struct less<GUID>
{
    bool
    operator()(const GUID& lhs, const GUID& rhs) const noexcept
    {
        return std::memcmp(&lhs, &rhs, sizeof lhs) < 0;
    }
};

template <>
struct hash<GUID>
{
    size_t
    operator()(const GUID& guid) const noexcept
    {
        RPC_STATUS status = RPC_S_OK;
        return ::UuidHash(const_cast<GUID*>(&guid), &status);
    }
};
} // namespace std
