#include <ciso646>
#include <notstd/task_queue.hxx>

using namespace threading;

std::future<void>
TaskQueue::Dispatcher::postBack(TaskQueue::Runnable aRunnable)
{
    return m_assignedLooper.postBack(std::move(aRunnable));
}

std::future<void>
TaskQueue::Dispatcher::postFront(TaskQueue::Runnable aRunnable)
{
    return m_assignedLooper.postFront(std::move(aRunnable));
}

TaskQueue::Dispatcher::Dispatcher(TaskQueue &aLooper) :
    m_assignedLooper(aLooper)
{}

TaskQueue::TaskQueue() :
    m_running(false),
    m_abortRequested(false),
    m_dispatcher(std::make_shared<notstd::enable_make_protected<Dispatcher>>(*this))
{
    try {
        m_thread = std::thread(&TaskQueue::runFunc, this);
    } catch (...) {
        throw TaskQueue::CreationException();
    }
}

TaskQueue::~TaskQueue()
{
    stop();
}

bool
TaskQueue::isRunning() const noexcept
{
    return m_running;
}

void
TaskQueue::stop()
{
    abortAndJoin();
}

std::shared_ptr<TaskQueue::Dispatcher>
TaskQueue::getDispatcher()
{
    return m_dispatcher;
}

void
TaskQueue::runFunc()
{
    m_running = true;

    while (not m_abortRequested) {
        TaskQueue::PackagedRunnable taskToRun;
        try {
            std::lock_guard guard(m_runnablesMutex);
            taskToRun = next();
        } catch (...) {
            throw TaskQueue::GetNextTaskException();
        }

        if (taskToRun.valid()) {
            taskToRun();
        }
    }

    m_running = false;
}

void
TaskQueue::abortAndJoin()
{
    m_abortRequested = true;
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

TaskQueue::PackagedRunnable
TaskQueue::next()
{
    if (m_runnables.empty()) {
        return PackagedRunnable();
    }
    TaskQueue::PackagedRunnable runnable = std::move(m_runnables.front());
    m_runnables.pop_front();
    return runnable;
}

std::future<void>
TaskQueue::postBack(TaskQueue::Runnable aRunnable)
{
    PackagedRunnable task(aRunnable);
    auto future = task.get_future();
    try {
        std::lock_guard guard(m_runnablesMutex);
        m_runnables.push_back(std::move(task));
    } catch (...) {
        throw TaskQueue::PushTaskException();
    }

    return future;
}

std::future<void>
TaskQueue::postFront(TaskQueue::Runnable aRunnable)
{
    TaskQueue::PackagedRunnable task(aRunnable);
    auto future = task.get_future();
    try {
        std::lock_guard guard(m_runnablesMutex);
        m_runnables.push_front(std::move(task));
    } catch (...) {
        throw TaskQueue::PushTaskException();
    }
    return future;
}
