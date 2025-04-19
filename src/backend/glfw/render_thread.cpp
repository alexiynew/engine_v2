#include "render_thread.hpp"

#define LOG_ERROR std::cerr
#include <iostream>

namespace game_engine::backend
{

RenderThread::RenderThread(GLFWwindow* window)
    : m_window(window)
{
    m_running.store(true, std::memory_order_release);
    m_thread = std::thread(&RenderThread::renderLoop, this);

    // Wait for tread to start
    m_initPromise.get_future().get();
}

RenderThread::~RenderThread()
{
    shutdown();
}

#pragma region RenderThread

void RenderThread::shutdown() noexcept
{
    try {
        m_running.store(false, std::memory_order_release);

        m_cv.notify_all();

        if (m_thread.joinable()) {
            m_thread.join();
        }
    } catch (...) {
        LOG_ERROR << "Render thread shutdown failed" << std::endl;
    }
}

void RenderThread::submit(Task task)
{
    std::packaged_task<void()> packagedTask(std::move(task));

    {
        std::lock_guard lock(m_mutex);
        m_tasks.push_back(std::move(packagedTask));
    }

    m_cv.notify_one();
}

std::future<void> RenderThread::submitSync(Task task)
{
    std::packaged_task<void()> packagedTask(std::move(task));
    auto future = packagedTask.get_future();

    {
        std::lock_guard lock(m_mutex);
        m_tasks.push_back(std::move(packagedTask));
    }

    m_cv.notify_one();
    return future;
}

std::thread::id RenderThread::getId() const noexcept
{
    return m_thread.get_id();
}

#pragma endregion

#pragma region RenderThread private

void RenderThread::renderLoop()
{
    glfwMakeContextCurrent(m_window);

    // Signal tread start
    m_initPromise.set_value();

    while (m_running.load(std::memory_order_acquire)) {
        std::deque<std::packaged_task<void()>> tasks;

        {
            std::unique_lock lock(m_mutex);
            m_cv.wait(lock, [this] { return !m_running.load(std::memory_order_acquire) || !m_tasks.empty(); });

            if (!m_running) {
                break;
            }

            tasks = std::move(m_tasks);
            m_tasks.clear();
        }

        try {
            for (auto& task : tasks) {
                task();
            }

        } catch (...) {
            // TODO report error to backend;
            // report error
        }
    }

    glfwMakeContextCurrent(nullptr);
}

#pragma endregion

} // namespace game_engine::backend

