
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
class CLooper
{
public:
    using Runnable = std::function<void()>;

    class CDispatcher
    {
        friend class CLooper; // Allow the looper to access the private constructor.

    public:
        bool
        post(const CLooper::Runnable &&aRunnable)
        {
            return m_assignedLooper.post(std::move(aRunnable));
        }

        bool
        post_blocking(const CLooper::Runnable &&aRunnable)
        {
            return m_assignedLooper.post_blocking(std::move(aRunnable));
        }

    private: // construction, since we want the looper to expose it's dispatcher exclusively!
        CDispatcher(CLooper &aLooper) :
            m_assignedLooper(aLooper)
        {}

    private:
        CLooper &m_assignedLooper;
    };

public:
    CLooper() :
        m_running(false), m_abortRequested(false), m_blockingTaskRequested(false), m_blockingTask(), m_runnables(), m_runnablesMutex(), m_dispatcher(std::shared_ptr<CDispatcher>(new CDispatcher(*this)))
    {
    }
    // Copy denied, Move to be implemented

    ~CLooper()
    {
        abortAndJoin();
    }

    bool
    running() const
    {
        return m_running.load();
    }

    bool
    run()
    {
        try {
            m_thread = std::thread(&CLooper::runFunc, this);
        } catch (...) {
            return false;
        }

        return true;
    }

    void
    stop()
    {
        abortAndJoin();
    }

    std::shared_ptr<CDispatcher>
    getDispatcher()
    {
        return m_dispatcher;
    }

private:
    void
    runFunc()
    {
        m_running.store(true);

        while (false == m_abortRequested.load()) {

            std::lock_guard guard(m_runnablesMutex);
            
            if (true == m_blockingTaskRequested.load()) {
                try {
                    m_blockingTask();
                } catch (...) {}
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

    void
    abortAndJoin()
    {
        m_abortRequested.store(true);
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

    // Runnables
    // this function is NOT thread safe
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

    bool
    post_blocking(const Runnable &&aRunnable){
        try{
            std::lock_guard guard(m_runnablesMutex);
            m_blockingTask = aRunnable; 
            m_blockingTaskRequested.store(true);

        } catch(...){
            return false;
        }
        while(true == m_blockingTaskRequested.load()) ;

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

    std::shared_ptr<CDispatcher> m_dispatcher;
};

} // namespace threading

#endif // TASK_QUEUE_HXX