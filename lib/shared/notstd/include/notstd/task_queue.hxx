
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
    
    struct CreationException : public std::exception {}; // signifies that the worker thread could not be started
    struct GetNextTaskException : public std::exception {}; // signifies that the worker thread could not get the next task to run
    struct PushTaskException : public std::exception {}; // signifies that the Dispatcher could not push a task onto the queue
    
    class Dispatcher
    {
        friend class TaskQueue; // Allow the looper to access the private constructor.

    public:
        /**
         * @brief posts a task to the back of the queue
         *        If the queue is being destroyed and this function is called there is NO guarantee that the requested task will be ran
         *        But if this function returns before the queue has been requested to be destroyed then the requested task will run.
         *        throws PushTaskException if the task couldn't be pushed properly
         *        
         * @param aRunnable The Runnable to be posted onto the queue
         *
         */
        std::future<void>
        postBack(TaskQueue::Runnable aRunnable);

    protected:
        Dispatcher(TaskQueue &aLooper);

    private:
        TaskQueue &m_assignedLooper;
    };

public:
    /**
     * @brief constructs the task queue and starts the worker thread. 
     *        throws a CreationException if the thread couldn't be started
     *
     */
    TaskQueue();

    ~TaskQueue();

    /**
     * @brief returns if the queue is running
     */
    bool
    isRunning() const noexcept;

    /**
     * @brief stops the worker thread and completes the task queue
     *
     */
    void
    stopWorkerAndCompleteTasks();

    std::shared_ptr<Dispatcher>
    getDispatcher();

private:
    /**
     * @brief the worker thread pulling the runnables from the queue and running them
     *        throws GetNextTaskException if the next task couldn't be obtained
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
     */
    PackagedRunnable
    next();

    /**
     * @brief the Dispatcher calls this function to post a runnable onto the back of the queue
     *
     */
    std::future<void>
    postBack(Runnable aRunnable);

private:
    std::thread m_thread;
    std::atomic_bool m_running;
    std::atomic_bool m_abortRequested;

    std::queue<PackagedRunnable> m_runnables;
    std::mutex m_runnablesMutex;

    std::shared_ptr<Dispatcher> m_dispatcher;
};

} // namespace threading

#endif // TASK_QUEUE_HXX