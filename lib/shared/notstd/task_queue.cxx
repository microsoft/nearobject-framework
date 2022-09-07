
#include <cassert>
#include <ciso646>
#include <notstd/task_queue.hxx>

using namespace notstd;

std::future<void>
TaskQueue::Dispatcher::Post(std::function<void()> runnable)
{
    return m_taskQueue.Post(std::move(runnable));
}

TaskQueue::Dispatcher::Dispatcher(TaskQueue &taskQueue) :
    m_taskQueue(taskQueue)
{}

TaskQueue::TaskQueue() :
    m_dispatcher(std::make_shared<notstd::enable_make_protected<Dispatcher>>(*this))
{
    try {
        m_thread = std::thread(&TaskQueue::ProcessQueue, this);
        m_state = State::Running;
    } catch (...) {
        throw TaskQueue::CreationException();
    }
}

TaskQueue::~TaskQueue()
{
    try {
        StopAndWaitForTaskCompletion();
    } catch (...) {
        // TODO: this object is being destroyed, but an exception could have
        // propagated from the worker thread. The caller may want to know about
        // this. A targeted exception may need to be thrown for that; need to
        // figure this out.
    }
}

std::shared_ptr<TaskQueue::Dispatcher>
TaskQueue::GetDispatcher() const noexcept
{
    return m_dispatcher;
}

void
TaskQueue::Stop(PendingTaskAction pendingTaskAction) noexcept
{
    {
        std::scoped_lock runnablesChangedLock{ m_runnablesChangedGate };

        switch (pendingTaskAction) {
        case PendingTaskAction::Cancel:
            m_state = State::Canceling;
            break;
        default:
            m_state = State::Stopping;
            break;
        }
    }

    m_runnablesChanged.notify_all();
}

void
TaskQueue::StopAndWaitForTaskCompletion()
{
    Stop();

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
TaskQueue::ShouldQueueExit() const noexcept
{
    bool shouldExit = false;

    // Exit if abort is requested, or if stopping and the queue has been drained.
    switch (m_state) {
    case State::Stopping:
        shouldExit = m_runnables.empty();
        break;
    case State::Canceling:
        shouldExit = true;
        break;
    default:
        shouldExit = false;
        break;
    }

    return shouldExit;
}

bool
TaskQueue::IsStopPending() const noexcept
{
    bool isStopPending = false;

    switch (m_state) {
    case State::Stopping:
    case State::Canceling:
        isStopPending = true;
        break;
    default:
        isStopPending = false;
        break;
    }

    return isStopPending;
}
 
void
TaskQueue::ProcessQueue()
{
    for (;;) {
        std::unique_lock runnablesChangedLock{ m_runnablesChangedGate };

        // Wait for non-empty queue or if exit requested.
        m_runnablesChanged.wait(runnablesChangedLock, [&]() {
            return !m_runnables.empty() || ShouldQueueExit();
        });

        // Check exit condition in case requested while waiting.
        if (ShouldQueueExit()) {
            break;
        }

        // The queue is non-empty, remove all tasks from the queue while holding
        // the lock to run them later.
        assert(m_runnables.size() > 0);
        auto tasks = std::move(m_runnables);

        // Now that all pending tasks have been popped from the queue, release
        // the lock. This allows clients to continue pushing onto the queue
        // while the existing tasks are executed, and eliminates the need to
        // acquire the lock once for each task. This sacrifices latency in
        // aborting the queue, which is assumed to be a rare scenario that can
        // sustain the performance penalty.
        runnablesChangedLock.unlock();

        // Notify waiters since the queue has been modified (now empty).
        m_runnablesChanged.notify_all();

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
TaskQueue::Post(std::function<void()> runnable)
{
    auto task = std::packaged_task<void()>(std::move(runnable));
    auto future = task.get_future();
    bool wasEmpty = false;

    {
        std::lock_guard runnablesChangedLock{ m_runnablesChangedGate };
        if (IsStopPending()) {
            throw std::runtime_error("attempt to post to queue in stopping state, this is a bug!");
        }

        wasEmpty = m_runnables.empty();
        m_runnables.push(std::move(task));
    }

    if (wasEmpty) {
        m_runnablesChanged.notify_all();
    }

    return future;
}
