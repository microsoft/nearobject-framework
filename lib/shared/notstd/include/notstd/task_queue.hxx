
#ifndef TASK_QUEUE_HXX
#define TASK_QUEUE_HXX

#include <atomic>
#include <ciso646>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <notstd/memory.hxx>
#include <queue>
#include <stdexcept>
#include <thread>

namespace threading
{
class TaskQueue
{
public:
    using Runnable = std::function<void()>;
    using PackagedRunnable = std::packaged_task<void()>;

    class Dispatcher
    {
        friend class TaskQueue; // Allow the looper to access the private constructor.

    public:
        /**
         * @brief posts a task to the back of the queue
         *
         * @param aRunnable The Runnable to be posted onto the queue
         *
         */
        std::future<void>
        postBack(TaskQueue::Runnable aRunnable)
        {
            return m_assignedLooper.postBack(std::move(aRunnable));
        }

        /**
         * @brief posts a task to the front of the queue
         *
         * @param aRunnable The Runnable to be run immediately
         *
         */
        std::future<void>
        postFront(TaskQueue::Runnable aRunnable)
        {
            return m_assignedLooper.postFront(std::move(aRunnable));
        }

    protected:
        Dispatcher(TaskQueue &aLooper) :
            m_assignedLooper(aLooper)
        {}

    private:
        TaskQueue &m_assignedLooper;
    };

public:
    TaskQueue() :
        m_running(false),
        m_abortRequested(false),
        m_dispatcher(std::make_shared<notstd::enable_make_protected<Dispatcher>>(*this))
    {
        try {
            m_thread = std::thread(&TaskQueue::runFunc, this);
        } catch (...) {
        }
    }

    ~TaskQueue()
    {
        stop();
    }

    /**
     * @brief returns if the queue is running
     */
    bool
    isRunning() const noexcept
    {
        return m_running;
    }

    /**
     * @brief stops the worker thread
     *
     */
    void
    stop()
    {
        abortAndJoin();
    }

    std::shared_ptr<Dispatcher>
    getDispatcher()
    {
        return m_dispatcher;
    }

private:
    /**
     * @brief the worker thread pulling the runnables from the queue and running them
     *
     */
    void
    runFunc()
    {
        m_running = true;

        while (not m_abortRequested) {
            PackagedRunnable taskToRun;
            {
                std::lock_guard guard(m_runnablesMutex);
                taskToRun = next();
            }

            if (taskToRun.valid()) {
                taskToRun();
            }
        }

        m_running = false;
    }

    /**
     * @brief tells the worker thread to stop and join
     *
     */
    void
    abortAndJoin()
    {
        m_abortRequested = true;
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    /**
     * @brief the worker thread calls this function to get the next task.
     *        the mutex m_runnablesMutex MUST be acquired prior to calling this function
     *
     */
    PackagedRunnable
    next()
    {
        if (m_runnables.empty()) {
            return PackagedRunnable();
        }
        PackagedRunnable runnable = std::move(m_runnables.front());
        m_runnables.pop_front();
        return runnable;
    }

    /**
     * @brief the Dispatcher calls this function to post a runnable onto the back of the queue
     *
     */
    std::future<void>
    postBack(Runnable aRunnable)
    {
        PackagedRunnable task(aRunnable);
        auto fut = task.get_future();
        try {
            std::lock_guard guard(m_runnablesMutex);
            m_runnables.push_back(std::move(task));
        } catch (...) {
        }

        return fut;
    }

    /**
     * @brief the Dispatcher calls this function to post a runnable onto the front of the queue
     *
     */
    std::future<void>
    postFront(Runnable aRunnable)
    {
        PackagedRunnable task(aRunnable);
        auto fut = task.get_future();
        try {
            std::lock_guard guard(m_runnablesMutex);
            m_runnables.push_front(std::move(task));
        } catch (...) {
        }
        return fut;
    }

private:
    std::thread m_thread;
    std::atomic_bool m_running;
    std::atomic_bool m_abortRequested;

    std::deque<PackagedRunnable> m_runnables;
    std::mutex m_runnablesMutex;

    std::shared_ptr<Dispatcher> m_dispatcher;
};

} // namespace threading

#endif // TASK_QUEUE_HXX