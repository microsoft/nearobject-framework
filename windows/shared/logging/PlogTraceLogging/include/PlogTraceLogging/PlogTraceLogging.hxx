
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
        std::string str = Formatter::format(record);

        // Log an event
        TraceLoggingWrite(g_hplogTraceLoggingProvider,      // handle to my provider
            "HelloWorldTestEvent",                          // Event Name that should uniquely identify your event.
            TraceLoggingValue(str.c_str(), "TestMessage")); // Field for your event in the form of (value, field name).
    }
};

} // namespace plog

#endif // PLOG_TRACE_LOGGING_HXX
