#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <thread>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace game_engine::backend
{

class RenderThread final
{
public:
    using Task = std::function<void()>;

    explicit RenderThread(GLFWwindow* window);
    ~RenderThread();

    void shutdown() noexcept;

    void submit(Task task);

    [[nodiscard]]
    std::future<void> submitSync(Task task);

    std::thread::id getId() const noexcept;

private:

    void renderLoop();

    GLFWwindow* m_window = nullptr;
    std::atomic<bool> m_running{false};
    std::thread m_thread;

    std::deque<std::packaged_task<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    std::promise<void> m_initPromise;
};

} // namespace game_engine::backend
