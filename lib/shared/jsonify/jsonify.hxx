#ifndef JSONIFY_HXX
#define JSONIFY_HXX
#include "rapidjson/document.h"     // rapidjson's DOM-style API

using namespace rapidjson;
using namespace std;

namespace persist {

class Serializable {
public:
    virtual rapidjson::Value to_serial(rapidjson::Document::AllocatorType&) const = 0;
};

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

};

#endif // JSONIFY_HXX