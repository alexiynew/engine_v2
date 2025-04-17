#include <game_engine/core/logger.hpp>

#include <queue>
#include <iostream>
#include <sstream>
#include <thread>

namespace game_engine::core{
// Singletone
class IOdeviceHelper
{
private:
    logger::customStreamHandler m_handler;
    std::queue<logger::uniqueStream> m_bufferPool;
    std::queue<logger::uniqueStream> m_buffersToProcess;
    std::unique_ptr<std::thread> m_processingThread;
    std::condition_variable m_bufferAwaitsCondition;
    std::condition_variable m_bufferReadyCondition;
    std::mutex m_mutexToProcess;
    std::mutex m_mutexToShare;
    bool m_isRunning = false;

    
    IOdeviceHelper()
    {
        for (size_t i = 0; i < 20;++i)
        {
            m_bufferPool.push(std::make_unique<std::ostringstream>());
        }
        m_isRunning = true;
        m_processingThread = std::make_unique<std::thread>(&IOdeviceHelper::loggerLoop, this);
    }

    void loggerLoop()
    {
        while (m_isRunning)
        {
            logger::uniqueStream stream;
            {
                std::unique_lock<std::mutex> lock(m_mutexToProcess);
                m_bufferAwaitsCondition.wait(lock, [this] 
                    {
                        return !m_isRunning || !m_buffersToProcess.empty();
                    });

                if (!m_isRunning) break;

                stream = std::move(m_buffersToProcess.front());
                m_buffersToProcess.pop();
            }

            if (m_handler) 
            {
                m_handler(std::move(stream));
            }
            else 
            {
                std::cout << stream->str() << std::endl;
            }

            {
                std::lock_guard<std::mutex> lock(m_mutexToProcess);
                m_bufferPool.push(std::make_unique<std::ostringstream>());
                m_bufferReadyCondition.notify_one();
            }
        }
    }

public:
    static IOdeviceHelper& instance()
    {
        static IOdeviceHelper self;
        return self;
    }

    logger::uniqueStream requestStreamDevice()
    {
        logger::uniqueStream device;

        {
            std::unique_lock<std::mutex> lock(m_mutexToProcess);
            m_bufferReadyCondition.wait(lock, [this] 
                {
                    return !m_bufferPool.empty();
                });
            
            device = std::move(m_bufferPool.front());
            m_bufferPool.pop();
        }

        return device;
    }

    void releaseStreamDevice(logger::uniqueStream device)
    {
        m_buffersToProcess.emplace(std::move(device));
        m_bufferAwaitsCondition.notify_one();
    }

    void setHandler(logger::customStreamHandler& handler)
    {
        m_handler = handler;
    }
};



Logger::Logger()
    :m_streamingDevice(IOdeviceHelper::instance().requestStreamDevice())
{}

Logger::~Logger()
{
    std::string str = m_streamingDevice->str();
    IOdeviceHelper::instance().releaseStreamDevice(std::move(m_streamingDevice));
}

void Logger::init()
{
    IOdeviceHelper::instance();
}

void Logger::init(logger::customStreamHandler& handler)
{
    IOdeviceHelper::instance().setHandler(handler);
}

}// end namespace expansion (game_engine::core)
