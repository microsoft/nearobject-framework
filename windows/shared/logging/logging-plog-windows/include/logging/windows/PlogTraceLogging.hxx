
#ifndef PLOG_TRACE_LOGGING_HXX
#define PLOG_TRACE_LOGGING_HXX

#include <windows.h>

#include <TraceLoggingProvider.h>

#include <plog/Appenders/IAppender.h>
#include <plog/Log.h>

#include <iostream>
#include <string>

namespace logging
{
namespace plog
{

template <typename Formatter>
class TraceLoggingAppender : public ::plog::IAppender
{
public:
    /**
     * @brief Construct a new TraceLoggingAppender object
     * This class is a wrapper around the trace logging calls, exposing them through the plog interface.
     * You must provide your own providerHandle, which needs to be defined elsewhere via the TRACELOGGING_DEFINE_PROVIDER macro
     *
     * @param providerHandle
     */
    TraceLoggingAppender(TraceLoggingHProvider providerHandle) :
        m_providerHandle(providerHandle)
    {
        HRESULT hr = TraceLoggingRegister(m_providerHandle);
        if (!SUCCEEDED(hr)) {
            std::cerr << "could not register TraceLoggingProvider!";
        }
    }

    ~TraceLoggingAppender()
    {
        TraceLoggingUnregister(m_providerHandle);
    }

    virtual void
    write(const ::plog::Record& record) override
    {
        std::wstring str = Formatter::format(record);

        TraceLoggingWrite(m_providerHandle,
            "PlogTraceLoggingWrapper",
            TraceLoggingWideString(str.c_str(), "Message"));
    }

private:
    TraceLoggingHProvider m_providerHandle;
};

} // namespace plog
} // namespace logging

#endif // PLOG_TRACE_LOGGING_HXX
