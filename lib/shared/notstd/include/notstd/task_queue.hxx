
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
private:
    using PackagedRunnable = std::packaged_task<void()>;
public:
    using Runnable = std::function<void()>;

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
        postBack(TaskQueue::Runnable aRunnable);

        /**
         * @brief posts a task to the front of the queue
         *
         * @param aRunnable The Runnable to be run immediately
         *
         */
        std::future<void>
        postFront(TaskQueue::Runnable aRunnable);

    protected:
        Dispatcher(TaskQueue &aLooper);

    private:
        TaskQueue &m_assignedLooper;
    };

public:
    TaskQueue();

    ~TaskQueue();

    /**
     * @brief returns if the queue is running
     */
    bool
    isRunning() const noexcept;

    /**
     * @brief stops the worker thread
     *
     */
    void
    stop();

    std::shared_ptr<Dispatcher>
    getDispatcher();

private:
    /**
     * @brief the worker thread pulling the runnables from the queue and running them
     *
     */
    void
    runFunc();

    /**
     * @brief tells the worker thread to stop and join
     *
     */
    void
    abortAndJoin();

    /**
     * @brief the worker thread calls this function to get the next task.
     *        the mutex m_runnablesMutex MUST be acquired prior to calling this function
     *
     */
    PackagedRunnable
    next();

    /**
     * @brief the Dispatcher calls this function to post a runnable onto the back of the queue
     *
     */
    std::future<void>
    postBack(Runnable aRunnable);
    /**
     * @brief the Dispatcher calls this function to post a runnable onto the front of the queue
     *
     */
    std::future<void>
    postFront(Runnable aRunnable);

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