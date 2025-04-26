#include <game_engine/core/logger.hpp>

#include <queue>
#include <iostream>
#include <sstream>
#include <thread>
#include <cassert>

namespace game_engine::core{
// Singletone
class IOdeviceHelper
{
private:
    Logger::customStreamHandler m_handler;
    std::queue<Logger::uniqueStream> m_bufferPool;
    std::queue<Logger::uniqueStream> m_buffersToProcess;
    std::unique_ptr<std::thread> m_processingThread;
    std::condition_variable m_bufferAwaitsCondition;
    std::condition_variable m_bufferReadyCondition;
    std::mutex m_mutexToProcess;
    std::mutex m_mutexToShare;
    std::atomic_bool m_isRunning = false;
    std::atomic_uint64_t m_poolSize;

    void InitWithPoolSize(const uint64_t& streamingDeviceCount)
    {
        m_poolSize = streamingDeviceCount;
        assert("Pool size must be set and be greater than zero" && 0 != m_poolSize);
        for (size_t i = 0; i < m_poolSize;++i)
        {
            m_bufferPool.push(std::make_unique<std::ostringstream>());
        }
        m_isRunning = true;
        m_processingThread = std::make_unique<std::thread>(&IOdeviceHelper::loggerLoop, this);
    }
    
    IOdeviceHelper()
    {}

    void loggerLoop()
    {
        while (m_isRunning)
        {
            Logger::uniqueStream stream;
            {
                std::unique_lock<std::mutex> lock(m_mutexToProcess);
                m_bufferAwaitsCondition.wait(lock, [this] 
                    {
                        return !m_isRunning || !m_buffersToProcess.empty();
                    });
                
                stream = std::move(m_buffersToProcess.front());
                m_buffersToProcess.pop();
            }
            PrintStream(stream);
            {
                std::lock_guard<std::mutex> lock(m_mutexToShare);
                m_bufferPool.push(std::make_unique<std::ostringstream>());
            }
            m_bufferReadyCondition.notify_one();
            if (!m_isRunning) break;
        }
        std::lock_guard<std::mutex> lockProcess(m_mutexToProcess);
        std::lock_guard<std::mutex> lockShare(m_mutexToShare);

        while(!m_buffersToProcess.empty())
        {
            PrintStream(m_buffersToProcess.front());
            m_buffersToProcess.pop();
        }
        while (!m_bufferPool.empty())
        {
            m_bufferPool.pop();
        }
    }

    void PrintStream(Logger::uniqueStream& stream)
    {
        if (m_handler) 
        {
            m_handler(std::move(stream));
        }
        else 
        {
            std::cout << stream->str() << std::endl;
        }
    }

public:
    static IOdeviceHelper& instance()
    {
        static IOdeviceHelper self;
        return self;
    }

    Logger::uniqueStream requestStreamDevice()
    {
        Logger::uniqueStream device;
        {
            std::unique_lock<std::mutex> lock(m_mutexToShare);
            m_bufferReadyCondition.wait(lock, [this]
                {
                    return !m_bufferPool.empty();
                });
            device = std::move(m_bufferPool.front());
            m_bufferPool.pop();
        }

        return device;
    }

    void releaseStreamDevice(Logger::uniqueStream device)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutexToProcess);
            m_buffersToProcess.emplace(std::move(device));
        }
        m_bufferAwaitsCondition.notify_one();
    }

    void setHandler(Logger::customStreamHandler& handler)
    {
        m_handler = handler;
    }

    void StartWithBufferSize(const uint64_t& bufferPoolSize = 0)
    {
        instance().InitWithPoolSize(bufferPoolSize);
    }

    static bool isWorking()
    {
        return instance().m_poolSize > 0 &&
            instance().m_processingThread.get();
    }

    ~IOdeviceHelper()
    {
        m_isRunning = false;
        m_processingThread->join();
    }
};



Logger::Logger()
    :m_streamingDevice(IOdeviceHelper::instance().requestStreamDevice())
{}

Logger::~Logger()
{
    IOdeviceHelper::instance().releaseStreamDevice(std::move(m_streamingDevice));
}

void Logger::init(const uint64_t& bufferSize)
{
    if (IOdeviceHelper::isWorking())
    {
        Logger() << "Trying to reinit working logger from thread " 
                 << std::this_thread::get_id();
        return;
    }
    IOdeviceHelper::instance().StartWithBufferSize(bufferSize);
}

void Logger::init(customStreamHandler& handler, const uint64_t& bufferSize)
{
    if (IOdeviceHelper::isWorking())
    {
        Logger() << "Trying to reinit working logger with handler from thread "
                 << std::this_thread::get_id();
        return;
    }
    IOdeviceHelper::instance().StartWithBufferSize(bufferSize);
    IOdeviceHelper::instance().setHandler(handler);
}

}// end namespace expansion (game_engine::core)
