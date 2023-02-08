
#ifndef PLOG_TRACE_LOGGING_HXX
#define PLOG_TRACE_LOGGING_HXX

#include <windows.h>

#include <TraceLoggingProvider.h>

#include <string>

#include <plog/Log.h>

TRACELOGGING_DECLARE_PROVIDER(g_hplogTraceLoggingProvider);

namespace logging
{

/**
 * @brief Returns the name of the log file to be used
 * Format is <date>-LogNearObject-<executableName>.txt
 * For example, if the date was Jan 2, 2001 and the executableName was nocli, then the name would be
 * 20010102-LogNearObject-nocli.txt
 *
 * @param executableName
 * @return std::string
 */
std::string
GetLogName2(const std::string& executableName);

// template <class Formatter>
// class PLOG_LINKAGE_HIDDEN TraceLoggingAppender : public IAppender
// {
// public:
//     TraceLoggingAppender()
//     {
//     }

//     ~TraceLoggingAppender()
//     {
//         DeregisterEventSource(m_eventSource);
//     }

//     virtual void write(const Record& record) PLOG_OVERRIDE
//     {
//         std::wstring str = Formatter::format(record);
//         const wchar_t* logMessagePtr[] = { str.c_str() };

        
//     }

// private:

// private:

// };

} // namespace logging

#endif // PLOG_TRACE_LOGGING_HXX
