#pragma once

#include <sstream>
#include <functional>
#include <memory>
#include <stdint.h>

namespace game_engine::core
{
class Logger {
public:
    using uniqueStream = std::unique_ptr<std::ostringstream>;
    using customStreamHandler = std::function<void(uniqueStream&&)>;

    Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
        *(m_streamingDevice.get()) << (manip);
        return *this;
    }

    template<typename T>
    Logger& operator<<(const T& data) {
       *(m_streamingDevice.get()) << data;
        return *this;
    }

    template <typename... Args>
    void print(Args&&... args) {
        *(m_streamingDevice.get())->operator<<(args...);
    }

    Logger();
    ~Logger();

    static void init(const uint64_t& bufferSize);
    static void init(customStreamHandler&, const uint64_t& bufferSize);

private:
    uniqueStream m_streamingDevice;
};
}