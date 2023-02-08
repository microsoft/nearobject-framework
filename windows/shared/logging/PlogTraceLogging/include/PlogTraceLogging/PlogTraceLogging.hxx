
#ifndef PLOG_TRACE_LOGGING_HXX
#define PLOG_TRACE_LOGGING_HXX

#include <windows.h>

#include <TraceLoggingProvider.h>

#include <plog/Appenders/IAppender.h>
#include <plog/Log.h>

#include <string>

TRACELOGGING_DECLARE_PROVIDER(g_hplogTraceLoggingProvider);

namespace plog
{

template <class Formatter>
class TraceLoggingAppender : public IAppender
{
public:
    TraceLoggingAppender()
    {
        TraceLoggingRegister(g_hplogTraceLoggingProvider);
    }

    ~TraceLoggingAppender()
    {
        TraceLoggingUnregister(g_hplogTraceLoggingProvider);
    }

    virtual void
    write(const Record& record) PLOG_OVERRIDE
    {
        std::wstring str = Formatter::format(record);

        TraceLoggingWrite(g_hplogTraceLoggingProvider,
            "PlogTraceLoggingWrapper",
            TraceLoggingValue(str.c_str(), "Message"));
    }
};

} // namespace plog

#endif // PLOG_TRACE_LOGGING_HXX
