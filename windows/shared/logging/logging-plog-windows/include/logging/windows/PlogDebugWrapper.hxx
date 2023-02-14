
#ifndef PLOG_DEBUG_WRAPPER_HXX
#define PLOG_DEBUG_WRAPPER_HXX

#include <windows.h>

#include <plog/Appenders/DebugOutputAppender.h>
#include <plog/Appenders/DynamicAppender.h>
#include <plog/Log.h>

#include <string>

namespace logging
{
namespace plog
{

template <typename Formatter>
class DebugWrapperAppender : public ::plog::DynamicAppender
{
public:
    /**
     * @brief Construct a new DebugWrapperAppender object
     * This class adds a DebugOutputApender iff the DEBUG macro is set and IsDebuggerPresent() is true
     *  otherwise this class just does the same as the input Appender
     * @param providerHandle
     */
    DebugWrapperAppender(::plog::IAppender *appender)
    {
        ::plog::DynamicAppender::addAppender(appender);
        ::plog::DynamicAppender::addAppender(&m_debugAppender);
    }

private:
    ::plog::DebugOutputAppender<Formatter> m_debugAppender;
};

} // namespace plog
} // namespace logging

#endif // PLOG_DEBUG_WRAPPER_HXX
