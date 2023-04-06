
#ifndef NEAR_OBJECT_SERVICE_CONFIGURATION
#define NEAR_OBJECT_SERVICE_CONFIGURATION

#include <string>
#include <vector>

namespace nearobject::service
{
/**
 * @brief Class to collect common service configuration.
 */
struct NearObjectServiceConfiguration
{
    /**
     * @brief Create a NearObjectServiceConfiguration from program command-line
     * arugments.
     * 
     * @param argc 
     * @param argv 
     * @param throwOnParseError Throw an exception if a parsing error is
     * encountered.
     * @return NearObjectServiceConfiguration 
     */
    static NearObjectServiceConfiguration
    FromCommandLineArguments(int argc, char* argv[], bool throwOnParseError = false);

    /**
     * @brief Create a NearObjectServiceConfiguration from program command-line
     * arugments.
     * 
     * @param args 
     * @param OnParseError 
     * @return NearObjectServiceConfiguration 
     */
    static NearObjectServiceConfiguration
    FromCommandLineArguments(std::vector<std::string>& args, bool throwOnParseError = false);

    /**
     * @brief Run the service in the background.
     */
    bool RunInBackground{ false };

    /**
     * @brief Enable logging verbosity.
     */
    bool VerboseLogging{ false };
};
} // namespace nearobject::service

#endif // NEAR_OBJECT_SERVICE_CONFIGURATION
