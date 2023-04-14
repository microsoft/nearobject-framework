
#include <mutex>

#include <plog/Appenders/DebugOutputAppender.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <logging/LogUtils.hxx>

#include "UwbSimulatorLogging.hxx"

struct PlogLoggingContext
{
    PlogLoggingContext(const char *logIdentity) :
        RollingFileAppender(logging::GetLogName(logIdentity).c_str())
    {}

    plog::RollingFileAppender<plog::TxtFormatter> RollingFileAppender;
    plog::DebugOutputAppender<plog::TxtFormatter> DebugAppender;
};

PlogLoggingContext &
GetLoggingContext()
{
    // Use magic static to guarantee thread-safe initialization of the logging context.
    static PlogLoggingContext plogLoggingContext("UwbSimulatorDriver");
    return plogLoggingContext;
}

std::once_flag LoggingIsInitializedFlag;

void
UwbSimulatorLoggingInitialize()
{
    std::call_once(LoggingIsInitializedFlag, [] {
        auto &plogLoggingContext = GetLoggingContext();
        plog::init(plog::verbose, &plogLoggingContext.RollingFileAppender)
            .addAppender(&plogLoggingContext.DebugAppender);
        LOG_INFO << "logging initialized";
    });
}
