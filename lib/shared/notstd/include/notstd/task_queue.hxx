
#ifndef TASK_QUEUE_HXX
#define TASK_QUEUE_HXX

#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>

namespace threading
{
class Looper
{
public:
    using Runnable = std::function<void()>;

    class Dispatcher
    {
        friend class Looper; // Allow the looper to access the private constructor.

    public:
        /**
         * @brief posts a non blocking task onto the queue
         *
         * @param aRunnable The Runnable to be posted onto the queue
         *
         */
        bool
        post(const Looper::Runnable &&aRunnable)
        {
            return m_assignedLooper.post(std::move(aRunnable));
        }

        /**
         * @brief posts a blocking task onto the queue. When any thread posts
         *        a blocking task using this function, they will block until
         *        the task completes
         *
         * @param aRunnable The Runnable to be posted onto the queue
         *
         */
        bool
        postBlocking(const Looper::Runnable &&aRunnable)
        {
            return m_assignedLooper.postBlocking(std::move(aRunnable));
        }

    private: // construction, since we want the looper to expose it's dispatcher exclusively!
        Dispatcher(Looper &aLooper) :
            m_assignedLooper(aLooper)
        {}

    private:
        Looper &m_assignedLooper;
    };

public:
    Looper() :
        m_running(false), m_abortRequested(false), m_blockingTaskRequested(false), m_blockingTask(), m_runnables(), m_runnablesMutex(), m_dispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
    {
    }

    ~Looper()
    {
        abortAndJoin();
    }


    /**
     * @brief returns if the queue is running
     */
    bool
    running() const
    {
        return m_running.load();
    }

    /**
     * @brief starts the worker thread running the tasks on the queue
     *
     */
    bool
    run()
    {
        try {
            m_thread = std::thread(&Looper::runFunc, this);
        } catch (...) {
            return false;
        }

        return true;
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
        m_running.store(true);

        while (false == m_abortRequested.load()) {
            std::lock_guard guard(m_runnablesMutex);

            if (true == m_blockingTaskRequested.load()) {
                try {
                    m_blockingTask();
                } catch (...) {
                }
                m_blockingTaskRequested.store(false);
            } else {
                try {
                    Runnable r = next();
                    if (nullptr != r) {
                        r();
                    }
                } catch (...) {
                }
            }
        }

        m_running.store(false);
    }

    /**
     * @brief tells the worker thread to stop and join
     *
     */
    void
    abortAndJoin()
    {
        m_abortRequested.store(true);
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    /**
     * @brief the worker thread calls this function to get the next task.
     *        the mutex m_runnablesMutex MUST be acquired prior to calling this function
     *
     */
    Runnable
    next()
    {
        if (m_runnables.empty()) {
            return nullptr;
        }
        Runnable runnable = m_runnables.front();
        m_runnables.pop();

        return runnable;
    }

    /**
     * @brief the Dispatcher calls this function to post a runnable onto the queue
     *
     */
    bool
    post(const Runnable &&aRunnable)
    {
        try {
            std::lock_guard guard(m_runnablesMutex);
            m_runnables.push(std::move(aRunnable));
        } catch (...) {
            return false;
        }

        return true;
    }

    /**
     * @brief the Dispatcher calls this function to post a blocking task
     *
     */
    bool
    postBlocking(const Runnable &&aRunnable)
    {
        try {
            std::lock_guard guard(m_runnablesMutex);
            m_blockingTask = aRunnable;
            m_blockingTaskRequested.store(true);
        } catch (...) {
            return false;
        }

        while (true == m_blockingTaskRequested.load())
            ;

        return true;
    }

private:
    std::thread m_thread;
    std::atomic_bool m_running;
    std::atomic_bool m_abortRequested;
    std::atomic_bool m_blockingTaskRequested;

    std::queue<Runnable> m_runnables;
    Runnable m_blockingTask;
    std::recursive_mutex m_runnablesMutex;

    std::shared_ptr<Dispatcher> m_dispatcher;
};

} // namespace threading

#endif // TASK_QUEUE_HXX