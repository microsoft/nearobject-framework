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

class Serializable
{
public:
    /**
     * @brief Turns this object into a json value
     *
     * @param allocator this is the allocator corresponding to the document for which the lifespan of this object will be bound by
     */
    virtual rapidjson::Value
    to_serial(rapidjson::Document::AllocatorType&) const = 0;

    /**
     * @brief Parse a json value and try to set the members of this object
     *
     * @param value the json value to parse
     * @return the status of the parsing. If anything other than Succeeded is returned, this object doesn't have its members set properly by the json value
     */
    virtual ParseResult
    parse_and_set(const rapidjson::Value&) = 0;
};

}; // namespace persist

#endif // JSONIFY_HXX