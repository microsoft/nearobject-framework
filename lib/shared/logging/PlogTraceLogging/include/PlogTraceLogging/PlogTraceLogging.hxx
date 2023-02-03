
#ifndef PLOG_TRACE_UTILS_HXX
#define PLOG_TRACE_UTILS_HXX

#include <string>

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

} // namespace logging

#endif // PLOG_TRACE_UTILS_HXX
