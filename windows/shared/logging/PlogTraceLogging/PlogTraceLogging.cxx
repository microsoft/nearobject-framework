
#include <chrono>
#include <iomanip>
#include <sstream>

#include <PlogTraceLogging/PlogTraceLogging.hxx>

#include <plog/Appenders/IAppender.h>

// Define a handle to a TraceLogging provider
TRACELOGGING_DEFINE_PROVIDER(
    g_hplogTraceLoggingProvider,
    "PlogTraceLoggingProvider",
    // {EB735AF1-3A63-4650-B4B0-47A4CEE75468}
    (0xeb735af1, 0x3a63, 0x4650, 0xb4, 0xb0, 0x47, 0xa4, 0xce, 0xe7, 0x54, 0x68));

std::string
logging::GetLogName2(const std::string& executableName)
{
    auto now = std::chrono::system_clock::now();

    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&t_c), "%Y%m%d")
       << "-LogNearObject-"
       << executableName
       << ".txt";

    return ss.str();
}
