
#ifndef JSONIFY_HXX
#define JSONIFY_HXX

#include "rapidjson/document.h" // rapidjson's DOM-style API

using namespace rapidjson;
using namespace std;

namespace persist
{
/**
 * @brief Describes the result of parsing a Serializable object
 */
enum class ParseResult {
    /**
     * @brief Parsing succeeded and a valid object produced.
     */
    Succeeded,

    /**
     * @brief Parsing failed and no valid object was produced.
     */
    Failed,

    /**
     * @brief An unknown error occurred. Nothing may be assumed about the
     * content of the input data.
     */
    UnknownError,
};

/**
 * @brief Describes the result of persisting a Serializable object
 */
enum class PersistResult {
    /**
     * @brief Persisting succeeded and a valid object produced.
     */
    Succeeded,

    /**
     * @brief Persisting failed and no valid object was produced.
     */
    Failed,

    /**
     * @brief Could not open the file for writing or reading
     */
    FailedToOpenFile,

    /**
     * @brief The saved profiles were not saved in the proper format
     */
    FailedToParseFile,

    /**
     * @brief An unknown error occurred. Nothing may be assumed about the
     * content of the input data.
     */
    UnknownError,
};

class Serializable
{
public:
    virtual ~Serializable() = default;
    Serializable() = default;
    Serializable(const Serializable&) = default;
    Serializable(Serializable&&) = default;
    Serializable& operator=(const Serializable&) = default;
    Serializable& operator=(Serializable&&) = default;

    /**
     * @brief Turns this object into a json value
     *
     * @param allocator this is the allocator corresponding to the document for which the lifespan of this object will be bound by
     */
    virtual rapidjson::Value
    ToJson(rapidjson::Document::AllocatorType&) const = 0;

    /**
     * @brief Parse a json value and try to set the members of this object
     *
     * @param value the json value to parse
     * @return the status of the parsing. If anything other than Succeeded is returned, this object doesn't have its members set properly by the json value
     */
    virtual ParseResult
    ParseAndSet(const rapidjson::Value&) = 0;
};

}; // namespace persist

#endif // JSONIFY_HXX
