
#ifndef TASK_QUEUE_HXX
#define TASK_QUEUE_HXX

#include <ciso646>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <notstd/memory.hxx>
#include <queue>
#include <stdexcept>
#include <thread>

namespace notstd 
{
/**
 * @brief A thread-safe, serialized task queue supporting cancelation.
 */
class TaskQueue
{
public:
    /**
     * @brief Class which handles the details of dispatching tasks to a
     * TaskQueue.
     * 
     * This default implementation submits tasks to the attached queue in
     * first-in-first-out (FIFO) order.
     */
    class Dispatcher
    {
        /**
         * @brief Allow the task queue to access the private constructor.
         */
        friend class TaskQueue;

    public:
        /**
         * @brief Posts a task onto the queue.
         *
         * If the queue is being destroyed and this function is called there is
         * NO guarantee that the requested task will be ran. However, if this
         * function returns before the queue has been requested to be destroyed
         * then the requested task will run.
         *        
         * @param runnable The runnable task to be posted onto the queue.
         */
        std::future<void>
        Post(std::function<void()> runnable);

    protected:
        /**
         * @brief Construct a new Dispatcher object.
         * 
         * @param taskQueue 
         */
        Dispatcher(TaskQueue &taskQueue);

    private:
        TaskQueue &m_taskQueue;
    };

public:
    /**
     * @brief Signifies that the worker thread could not be started.
     */
    struct CreationException : public std::exception {};

    /**
     * @brief Constructs the task queue and starts the worker thread. 
     * Throws a CreationException if the thread couldn't be started.
     */
    TaskQueue();

    /**
     * @brief Destroy the Task Queue object.
     */
    ~TaskQueue();

    /**
     * @brief The action to apply to pending tasks.
     */
    enum class PendingTaskAction {
        Run,
        Cancel,
    };

    /**
     * @brief Halts operation of the queue.
     * 
     * @param pendingTaskAction The desired action to take for pending tasks.
     * Prior to destruction, all pending tasks will by run by default ('Run'),
     * or will be canceled ('Cancel').
     */
    void
    Stop(PendingTaskAction pendingTaskAction = PendingTaskAction::Run) noexcept;

    /**
     * @brief Stops the worker thread and waits for completion of all pending
     * queue tasks.
     */
    void
    StopAndWaitForTaskCompletion();

    /**
     * @brief Get the Dispatcher object.
     * 
     * @return std::shared_ptr<Dispatcher> 
     */
    std::shared_ptr<Dispatcher>
    GetDispatcher() const noexcept;

private:
    /**
     * @brief Handler function run by the queue processing thread.
     */
    void
    ProcessQueue();

    /**
     * @brief Indicates whether the queue processing thread should exit.
     * 
     * The m_runnablesChangedGate mutex must be held when calling this function.
     * 
     * @return true 
     * @return false 
     */
    bool
    ShouldQueueExit() const noexcept;

    /**
     * @brief Indicates if stopping the queue is pending.
     * 
     * This does *not* indicate whether the worker thread should stop. For that,
     * call @ref ShouldQueueExit().
     * 
     * The m_runnablesChangedGate mutex must be held when calling this function.
     * 
     * @return true 
     * @return false 
     */
    bool
    IsStopPending() const noexcept;

    /**
     * @brief The Dispatcher calls this function to post a std::function<void()>
     * onto the back of the queue.
     * 
     * @param runnable
     * @return std::future<void> 
     */
    std::future<void>
    Post(std::function<void()> runnable);

    /**
     * @brief Helper to track state of the queue.
     */
    enum class State {
        Running,
        Stopping,
        Canceling,
        Stopped,
    };

private:
    std::thread m_thread;
    std::shared_ptr<Dispatcher> m_dispatcher;

    std::mutex m_runnablesChangedGate;
    // Access to the below variables must be synchronized with m_runnablesChangedGate.
    std::queue<std::packaged_task<void()>> m_runnables;
    std::condition_variable m_runnablesChanged;
    State m_state{ State::Stopped };
};

} // namespace notstd

#endif // TASK_QUEUE_HXX