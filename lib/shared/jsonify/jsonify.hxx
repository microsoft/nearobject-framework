// Hello World example
// This example shows basic usage of DOM-style API.

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>

using namespace rapidjson;
using namespace std;

namespace persist {

class Serializable {
public:
    virtual Value to_serial() = 0;
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
