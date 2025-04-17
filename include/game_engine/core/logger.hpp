#pragma once

#include <sstream>
#include <functional>

namespace game_engine::core
{
namespace logger
{
typedef std::unique_ptr<std::ostringstream> uniqueStream;
typedef std::function<void(uniqueStream&&)> customStreamHandler;
}

class Logger {
public:
/*    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
        m_streamingDevice->operator<<(manip);
        return *this;
    }
*/
    template<typename T>
    Logger& operator<<(const T& data) {
       *(m_streamingDevice.get()) << data;
        return *this;
    }
    Logger();
    ~Logger();

    static void init();
    static void init(logger::customStreamHandler&);

private:
    logger::uniqueStream m_streamingDevice;
};
}