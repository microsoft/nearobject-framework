
#include <chrono>
#include <iomanip>
#include <sstream>

#include <PlogTraceLogging/PlogTraceLogging.hxx>

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
