
#include <cassert>
#include <version>

#include <notstd/task_queue.hxx>

using namespace notstd;

std::future<void>
task_queue::dispatcher::post(std::function<void()> runnable)
{
    return m_task_queue.post(std::move(runnable));
}

task_queue::dispatcher::dispatcher(task_queue &task_queue) :
    m_task_queue(task_queue)
{}

task_queue::task_queue() :
    m_dispatcher(std::make_shared<notstd::enable_make_protected<dispatcher>>(*this))
{
    try {
        m_thread = std::thread(&task_queue::process_queue, this);
        m_state = state::running;
    } catch (...) {
        throw task_queue::creation_exception();
    }
}

task_queue::~task_queue()
{
    try {
        stop_and_wait_for_task_completion();
    } catch (...) {
        // TODO: this object is being destroyed, but an exception could have
        // propagated from the worker thread. The caller may want to know about
        // this. A targeted exception may need to be thrown for that; need to
        // figure this out.
    }
}

std::shared_ptr<task_queue::dispatcher>
task_queue::get_dispatcher() const noexcept
{
    return m_dispatcher;
}

void
task_queue::stop(pending_task_action pending_task_action) noexcept
{
    {
        std::scoped_lock runnables_changed_lock{ m_runnables_changed_gate };

        switch (pending_task_action) {
        case pending_task_action::cancel:
            m_state = state::canceling;
            break;
        default:
            m_state = state::stopping;
            break;
        }
    }

    m_runnables_changed.notify_all();
}

void
task_queue::stop_and_wait_for_task_completion()
{
    stop();

    if (m_thread.joinable()) {
        try {
            m_thread.join();
        } catch (...) {
            // TODO: need to handle any exceptions thrown from the worker
            // thread (not the task executiuon) and propagate them to the caller.
        }
    }
}

bool
task_queue::should_queue_exit() const noexcept
{
    bool should_exit = false;

    // Exit if abort is requested, or if stopping and the queue has been drained.
    switch (m_state) {
    case state::stopping:
        should_exit = m_runnables.empty();
        break;
    case state::canceling:
        should_exit = true;
        break;
    default:
        should_exit = false;
        break;
    }

    return should_exit;
}

bool
task_queue::is_stop_pending() const noexcept
{
    bool is_stop_pending = false;

    switch (m_state) {
    case state::stopping:
    case state::canceling:
        is_stop_pending = true;
        break;
    default:
        is_stop_pending = false;
        break;
    }

    return is_stop_pending;
}
 
void
task_queue::process_queue()
{
    for (;;) {
        std::unique_lock runnables_changed_lock{ m_runnables_changed_gate };

        // Wait for non-empty queue or if exit requested.
        m_runnables_changed.wait(runnables_changed_lock, [&]() {
            return !m_runnables.empty() || should_queue_exit();
        });

        // Check exit condition in case requested while waiting.
        if (should_queue_exit()) {
            break;
        }

        // The queue is non-empty, remove all tasks from the queue while holding
        // the lock to run them later.
        assert(m_runnables.size() > 0);
        auto tasks = std::move(m_runnables);
        m_runnables = {};

        // Now that all pending tasks have been popped from the queue, release
        // the lock. This allows clients to continue pushing onto the queue
        // while the existing tasks are executed, and eliminates the need to
        // acquire the lock once for each task. This sacrifices latency in
        // aborting the queue, which is assumed to be a rare scenario that can
        // sustain the performance penalty.
        runnables_changed_lock.unlock();

        // Notify waiters since the queue has been modified (now empty).
        m_runnables_changed.notify_all();

        // Run all the pending tasks.
        while (!tasks.empty()) {
            auto taskToRun = std::move(tasks.front());
            tasks.pop();
            if (taskToRun.valid()) {
                taskToRun();
            }
        }
    } // for (;;)
}

std::future<void>
task_queue::post(std::function<void()> runnable)
{
    auto task = std::packaged_task<void()>(std::move(runnable));
    auto future = task.get_future();
    bool was_empty = false;

    {
        std::lock_guard runnables_changed_lock{ m_runnables_changed_gate };
        if (is_stop_pending()) {
            throw std::runtime_error("attempt to post to queue in stopping state, this is a bug!");
        }

        was_empty = m_runnables.empty();
        m_runnables.push(std::move(task));
    }

    if (was_empty) {
        m_runnables_changed.notify_all();
    }

    return future;
}
