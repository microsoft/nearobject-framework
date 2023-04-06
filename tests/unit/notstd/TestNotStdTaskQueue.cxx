
#include <chrono>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <notstd/task_queue.hxx>

using namespace std::chrono_literals;

namespace notstd
{
namespace test
{
enum class TaskState {
    NotStarted,
    Started,
    Execute,
    Completed,
};

/**
 * @brief Helper class to verify correct execution of runnable tasks from a
 * task_queue.
 */
struct RunnableTask {
    explicit RunnableTask(int valueExpected) : 
        ValueExpected(valueExpected) 
    {}

    int ValueExpected;
    std::optional<int> ValueActual;
    std::future<void> Future{};
    TaskState State{ TaskState::NotStarted };
};
} // namespace test
} // namespace notstd

TEST_CASE("task queue can be created", "[notstd][shared][utility]")
{
    using notstd::task_queue;

    SECTION("creation doesn't cause a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<task_queue>());
    }
}

TEST_CASE("task queue can be destroyed", "[notstd][shared][utility]")
{
    using notstd::task_queue;

    SECTION("destruction doesn't cause a crash")
    {
        auto taskQueue = std::make_unique<task_queue>();
        REQUIRE_NOTHROW(std::destroy_at(taskQueue.get()));
        taskQueue.release();
    }
}

TEST_CASE("task queue dispatcher posts tasks without causing a crash", "[notstd][shared][utility]")
{
    using notstd::task_queue;

    SECTION("dispatcher can be obtained after creation and is valid")
    {
        task_queue taskQueue{};
        std::shared_ptr<task_queue::dispatcher> dispatcher = nullptr;;
        REQUIRE_NOTHROW(dispatcher = taskQueue.get_dispatcher());
        REQUIRE(dispatcher != nullptr);
    }

    SECTION("dispatcher can be obtained after posting one (1) task")
    {
        task_queue taskQueue{};
        task_queue::dispatcher *dispatcherPointer = nullptr;

        {   // Obtain dispatcher, post a task, then allow the dispatcher to be
            // destroyed at end-of-scope.
            auto dispatcher = taskQueue.get_dispatcher();
            REQUIRE(dispatcher != nullptr);
            dispatcherPointer = dispatcher.get();
            dispatcher->post([]{});
        }

        {   // Obtain the dispatcher and ensure it is the same as the original
            // dispatcher by validating its pointer is the same. This assumes
            // that the dispatcher object lives indefinitely within the task
            // queue, which doesn't have to be the case (eg. the queue could
            // create the dispatcher on-demand).
            auto dispatcher = taskQueue.get_dispatcher();
            REQUIRE(dispatcher != nullptr);
            REQUIRE(dispatcherPointer == dispatcher.get());
            dispatcher->post([]{});
        }
    }

    SECTION("dispatcher can be obtained after posting multiple tasks")
    {
        static constexpr std::size_t numPosts = 10;

        task_queue taskQueue{};
        std::shared_ptr<task_queue::dispatcher> dispatcher{};

        for (std::size_t i = 0; i < numPosts; i++) {
            REQUIRE_NOTHROW(dispatcher = taskQueue.get_dispatcher());
            REQUIRE_NOTHROW(dispatcher->post([]{}));
        }
    }

    SECTION("dispatcher can be used to post multiple tasks")
    {
        static constexpr std::size_t numPosts = 10;

        task_queue taskQueue{};
        auto dispatcher = taskQueue.get_dispatcher();

        for (std::size_t i = 0; i < numPosts; i++) {
            REQUIRE_NOTHROW(dispatcher->post([]{}));
        }
    }
}

TEST_CASE("task queue dispatcher posts tasks correctly", "[notstd][shared][utility]")
{
    using notstd::task_queue;

    SECTION("dispatcher can post single task without causing a crash")
    {
        REQUIRE_NOTHROW(task_queue{}.get_dispatcher()->post([]{}));
    }

    SECTION("post result returns a valid future")
    {
        task_queue taskQueue{};
        auto taskFuture = taskQueue.get_dispatcher()->post([]{});
        REQUIRE(taskFuture.valid());
    }

    SECTION("post result can be waited on without causing a crash")
    {
        task_queue taskQueue{};
        auto taskFuture = taskQueue.get_dispatcher()->post([]{});
        REQUIRE(taskFuture.valid());
        REQUIRE_NOTHROW(taskFuture.wait_for(100ms));
    }

    SECTION("posted task is completed without causing a crash")
    {
        task_queue taskQueue{};
        auto taskFuture = taskQueue.get_dispatcher()->post([]{});
        REQUIRE(taskFuture.wait_for(100ms) == std::future_status::ready);
        REQUIRE_NOTHROW(taskFuture.get());
    }

    SECTION("posted task is completed and provides the expected result")
    {
        static constexpr auto ValueInitial  = 50;
        static constexpr auto ValueExpected = 100;

        task_queue taskQueue{};
        auto value = ValueInitial;
        auto taskFuture = taskQueue.get_dispatcher()->post([&value]{
            value = ValueExpected;
        });

        REQUIRE_NOTHROW(taskFuture.get());
        REQUIRE(value == ValueExpected);
    }
}

TEST_CASE("task queue executes tasks serially", "[notstd][shared][utility][serial]")
{
    using notstd::task_queue;

    const std::vector<int> TaskValuesExpected{ 1, 2, 3, 4, 5 };

    task_queue taskQueue{};
    std::mutex valuesGate{};
    std::vector<int> taskValuesActual{};
    std::vector<std::future<void>> taskFutures{};

    taskValuesActual.reserve(TaskValuesExpected.size());
    taskFutures.reserve(TaskValuesExpected.size());

    // Push one task onto the queue for each value in the expected vector.
    for (const auto& value : TaskValuesExpected) {
        taskFutures.emplace_back(taskQueue.get_dispatcher()->post([&]{
            std::unique_lock valuesLock{ valuesGate };
            taskValuesActual.push_back(value);
        }));
    }

    // Wait for each task to complete.
    for (auto& taskFuture : taskFutures) {
        REQUIRE_NOTHROW(taskFuture.get());
    }

    // Validate the built-up vector matches the expected one.
    REQUIRE(taskValuesActual == TaskValuesExpected);
}

TEST_CASE("task queue executes tasks in first-in-first-out (FIFO) order", "[notstd][shared][utility][order]")
{
    using notstd::task_queue;
    using notstd::test::TaskState;
    using notstd::test::RunnableTask;

    // TODO: should be possible to initialize this with an initializer list.
    std::vector<RunnableTask> taskVerifiers{};
    taskVerifiers.emplace_back(1);
    taskVerifiers.emplace_back(2);
    taskVerifiers.emplace_back(3);

    // Condition variable and associated mutex used to orchestrate and
    // synchronize task execution.
    std::mutex taskCompletionGate;
    std::condition_variable taskCompletion;

    task_queue taskQueue{};
    auto dispatcher = taskQueue.get_dispatcher();

    // Submit tasks to the queue, saving their futures.
    for (auto& taskVerifier : taskVerifiers) {
        taskVerifier.Future = dispatcher->post([&] {
            // Notify task has started.
            std::unique_lock taskCompletionLock{ taskCompletionGate };
            taskVerifier.State = TaskState::Started;
            taskCompletion.notify_one();

            // Wait to execute body.
            taskCompletion.wait(taskCompletionLock, [&]{ 
                return (taskVerifier.State == TaskState::Execute); 
            });

            // Execute the body then notify of completion.
            taskVerifier.ValueActual = taskVerifier.ValueExpected;
            taskVerifier.State = TaskState::Completed;
            taskCompletion.notify_one();
        });
    }

    // Wait for the submitted tasks to complete, and validate that they ran in order.
    for (auto taskVerifierIterator = std::begin(taskVerifiers); taskVerifierIterator != std::end(taskVerifiers); std::advance(taskVerifierIterator, 1)) {
        auto& taskVerifier = *taskVerifierIterator;

        // Wait for the task to start.
        std::unique_lock taskCompletionLock{ taskCompletionGate };
        REQUIRE(taskCompletion.wait_for(taskCompletionLock, 100ms, [&]{ 
            return (taskVerifier.State == TaskState::Started); 
        }));

        // Verify the next task to execute hasn't completed yet and hasn't updated the actual value.
        REQUIRE(!taskVerifier.ValueActual.has_value());
        REQUIRE(taskVerifier.State == TaskState::Started);

        // Signal the task to resume execution.
        taskVerifier.State = TaskState::Execute;
        taskCompletion.notify_one();

        // Wait for the task to complete execution.
        taskCompletion.wait(taskCompletionLock, [&]{ 
            return (taskVerifier.State == TaskState::Completed); 
        });

        // Obtain the future result and verify it updated the actual value.
        taskVerifier.Future.get();
        REQUIRE(taskVerifier.ValueActual == taskVerifier.ValueExpected);

        // Verify that all following task futures haven't run yet, and the task hasn't updated the respective actual value.
        for (auto taskVerifierIteratorRest = std::next(taskVerifierIterator); taskVerifierIteratorRest != std::end(taskVerifiers); taskVerifierIteratorRest = std::next(taskVerifierIteratorRest)) {
            auto& taskVerifierNext = *taskVerifierIteratorRest;
            REQUIRE(taskVerifierNext.Future.valid());
            REQUIRE(!taskVerifierNext.ValueActual.has_value());
        }
    }
}

TEST_CASE("task queue can be explicitly stopped", "[notstd][shared][utility]")
{
    using notstd::task_queue;

    static constexpr std::size_t NumTasksToRun = 5; 

    std::size_t numTasksRan = 0;
    std::vector<std::future<void>> taskFutures{};
    taskFutures.reserve(NumTasksToRun);

    SECTION("stopping queue prevents additional tasks from being enqueued")
    {
        for (auto pendingTaskAction : { task_queue::pending_task_action::run, task_queue::pending_task_action::cancel }) {
            task_queue taskQueue{};
            taskQueue.stop(pendingTaskAction);
            REQUIRE_THROWS(taskQueue.get_dispatcher()->post([]{}));
        }
    }

    SECTION("stopping queue executes all pending tasks by default")
    {
        task_queue taskQueue{};

        // Push one task onto the queue for each value in the expected vector.
        for (std::size_t i = 0; i < NumTasksToRun; i++) {
            taskFutures.emplace_back(taskQueue.get_dispatcher()->post([&numTasksRan]{
                numTasksRan++;
            }));
        }

        taskQueue.stop();

        // Ensure each task has completed.
        for (auto& taskFuture : taskFutures) {
            REQUIRE_NOTHROW(taskFuture.get());
        }

        // Validate the expected number of tasks ran.
        REQUIRE(numTasksRan == NumTasksToRun);
    }

    SECTION("stopping queue cancels all pending tasks when requested")
    {
        {
            task_queue taskQueue{};
            auto dispatcher = taskQueue.get_dispatcher();

            taskFutures.emplace_back(dispatcher->post([&]{
                numTasksRan++;

                // Recursively enqueue a bunch of new tasks. Since this is
                // executing on the queue worker thread and the runnables mutex
                // isn't locked during task execution, this is safe to do. These
                // tasks will be pushed onto the queue but not yet processed. 
                //
                // This assumes knowledge of queue implementation details,
                // however, it is currently the best method available to test
                // this functionality.
                for ([[maybe_unused]] const auto _ : {1,2,3,4,5,6,7,8,9}) {
                    taskFutures.emplace_back(dispatcher->post([&] {
                        numTasksRan++;
                    }));
                }

                // Now that a bunch of tasks have been enqueued, call Stop() to
                // cancel those pending tasks.
                taskQueue.stop(task_queue::pending_task_action::cancel);
            }));

            // Give the queue a small amount of time to execute the first task.
            std::this_thread::sleep_for(20ms);

            // Ensure the first task was enqueued, and wait for it to complete.
            REQUIRE(taskFutures.size() >= 1);
            REQUIRE(taskFutures[0].valid());
            REQUIRE_NOTHROW(taskFutures[0].get());

            // Ensure multiple tasks were enqueued.
            REQUIRE(taskFutures.size() > 1);
        }

        // The queue is now destroyed. Ensure only the first task executed.
        REQUIRE(numTasksRan == 1);
    }
}
