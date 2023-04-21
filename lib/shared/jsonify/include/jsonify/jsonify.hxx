
#ifndef JSONIFY_HXX
#define JSONIFY_HXX

namespace persist
{
/**
 * @brief Describes the result of parsing a serializable object
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
 * @brief Describes the result of persisting a serializable object
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

}; // namespace persist

#endif // JSONIFY_HXX
