
#include <ctime>
#include <iomanip>
#include <sstream>
#include <LogUtils.hxx>

std::string
logging::GetLogName(const std::string& executableName)
{
    auto now = time(0);
    tm* ltm = localtime(&now);

    std::stringstream ss;

    ss << 1900 + ltm->tm_year
       << std::setw(2) << std::setfill('0')
       << ltm->tm_mon
       << ltm->tm_mday
       << "-LogNearObject-"
       << executableName
       << ".txt";

    return ss.str();
}
