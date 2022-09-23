
#ifndef TASK_QUEUE_HXX
#define TASK_QUEUE_HXX

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <notstd/memory.hxx>
#include <queue>
#include <stdexcept>
#include <thread>
#include <version>

namespace notstd 
{
/**
 * @brief A thread-safe, serialized task queue supporting cancelation.
 */
class task_queue
{
public:
    /**
     * @brief Class which handles the details of dispatching tasks to a
     * task_queue.
     * 
     * This default implementation submits tasks to the attached queue in
     * first-in-first-out (FIFO) order.
     */
    class dispatcher
    {
        /**
         * @brief Allow the task queue to access the private constructor.
         */
        friend class task_queue;

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
        post(std::function<void()> runnable);

    protected:
        /**
         * @brief Construct a new dispatcher object.
         * 
         * @param taskQueue 
         */
        dispatcher(task_queue &taskQueue);

    private:
        task_queue &m_task_queue;
    };

public:
    /**
     * @brief Signifies that the worker thread could not be started.
     */
    struct creation_exception : public std::exception {};

    /**
     * @brief Constructs the task queue and starts the worker thread. 
     * Throws a creation_exception if the thread couldn't be started.
     */
    task_queue();

    /**
     * @brief Destroy the Task Queue object.
     */
    ~task_queue();

    /**
     * @brief The action to apply to pending tasks.
     */
    enum class pending_task_action {
        run,
        cancel,
    };

    /**
     * @brief Halts operation of the queue.
     * 
     * @param pending_task_action The desired action to take for pending tasks.
     * Prior to destruction, all pending tasks will by run by default ('Run'),
     * or will be canceled ('Cancel').
     */
    void
    stop(pending_task_action pending_task_action = pending_task_action::run) noexcept;

    /**
     * @brief Stops the worker thread and waits for completion of all pending
     * queue tasks.
     */
    void
    stop_and_wait_for_task_completion();

    /**
     * @brief Get the dispatcher object.
     * 
     * @return std::shared_ptr<dispatcher> 
     */
    std::shared_ptr<dispatcher>
    get_dispatcher() const noexcept;

private:
    /**
     * @brief Handler function run by the queue processing thread.
     */
    void
    process_queue();

    /**
     * @brief Indicates whether the queue processing thread should exit.
     * 
     * The m_runnables_changed_gate mutex must be held when calling this function.
     * 
     * @return true 
     * @return false 
     */
    bool
    should_queue_exit() const noexcept;

    /**
     * @brief Indicates if stopping the queue is pending.
     * 
     * This does *not* indicate whether the worker thread should stop. For that,
     * call @ref should_queue_exit().
     * 
     * The m_runnables_changed_gate mutex must be held when calling this function.
     * 
     * @return true 
     * @return false 
     */
    bool
    is_stop_pending() const noexcept;

    /**
     * @brief The dispatcher calls this function to post a std::function<void()>
     * onto the back of the queue.
     * 
     * @param runnable
     * @return std::future<void> 
     */
    std::future<void>
    post(std::function<void()> runnable);

    /**
     * @brief Helper to track state of the queue.
     */
    enum class state {
        running,
        stopping,
        canceling,
        stopped,
    };

private:
    std::thread m_thread;
    std::shared_ptr<dispatcher> m_dispatcher;

    std::mutex m_runnables_changed_gate;
    // Access to the below variables must be synchronized with m_runnables_changed_gate.
    std::queue<std::packaged_task<void()>> m_runnables;
    std::condition_variable m_runnables_changed;
    state m_state{ state::stopped };
};

} // namespace notstd

#endif // TASK_QUEUE_HXX
