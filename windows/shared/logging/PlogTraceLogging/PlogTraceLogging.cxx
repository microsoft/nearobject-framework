
#include <chrono>
#include <iomanip>
#include <sstream>

#include <PlogTraceLogging/PlogTraceLogging.hxx>


TRACELOGGING_DEFINE_PROVIDER(
    g_hplogTraceLoggingProvider,
    "PlogTraceLoggingProvider",
    // {EB735AF1-3A63-4650-B4B0-47A4CEE75468}
    (0xeb735af1, 0x3a63, 0x4650, 0xb4, 0xb0, 0x47, 0xa4, 0xce, 0xe7, 0x54, 0x68));