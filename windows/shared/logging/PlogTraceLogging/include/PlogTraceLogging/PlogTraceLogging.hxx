
#ifndef PLOG_TRACE_LOGGING_HXX
#define PLOG_TRACE_LOGGING_HXX

#include <windows.h>

#include <TraceLoggingProvider.h>

#include <plog/Appenders/IAppender.h>
#include <plog/Log.h>

#include <string>

namespace plog
{

template <class Formatter>
class TraceLoggingAppender : public IAppender
{
public:
    TraceLoggingAppender(TraceLoggingHProvider providerHandle) : m_providerHandle(providerHandle)
    {
        TraceLoggingRegister(m_providerHandle);
    }

    ~TraceLoggingAppender()
    {
        TraceLoggingUnregister(m_providerHandle);
    }

    virtual void
    write(const Record& record) PLOG_OVERRIDE
    {
        std::wstring str = Formatter::format(record);

        TraceLoggingWrite(m_providerHandle,
            "PlogTraceLoggingWrapper",
            TraceLoggingValue(str.c_str(), "Message"));
    }

private:
    TraceLoggingHProvider m_providerHandle;
};

} // namespace plog

#endif // PLOG_TRACE_LOGGING_HXX
