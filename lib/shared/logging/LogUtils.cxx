
#include <chrono>
#include <format>
#include <iomanip>

#include <logging/LogUtils.hxx>

std::string
logging::GetLogName(const std::string& executableName)
{
    auto now = std::chrono::system_clock::now();
    auto dateString = std::vformat("{0:%Y%M%d}", std::make_format_args(now));

    return dateString + "-LogNearObject-" + executableName + ".txt";
}
