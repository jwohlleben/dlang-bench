#ifndef TASKPOOL_HPP
#define TASKPOOL_HPP

#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include <vector>
#include <condition_variable>

#include "qs_task.hpp"

/**
 * This class represents a simple taskpool
 */
class taskpool
{
public:
    /**
     * Initializes the taskpool with the number of worker threads
     * @param thread_count The thread count
     */
    taskpool(unsigned int thread_count);

    /**
     * Starts the taskpool. Worker threads are waiting for new tasks
     */
    void start();

    /**
     * Stops the taskpool. Worker threads stop working
     */
    void stop();

    /**
     * Wait for threads to finish the work
     */
    void wait_until_finished();

    /**
     * Work also on tasks until finished
     */
    void work_until_finished();

    /**
     * Inserts a new task into the task queue
     *
     * @param task Task to insert into queue
     */
    void put(const std::shared_ptr<qs_task> task);

    /**
     * Inserts a new task into the task queue and works until finished
     *
     * @param task Task to insert into queue
     */
    void put_and_work_until_finished(const std::shared_ptr<qs_task> task);

    /**
     * Returns the number of threads
     * @return The thread count
     */
    const unsigned int& thread_count() noexcept;

private:
    /**
     * Number of worker threads
     */
    unsigned int thread_count_;

    /**
     * Number of currently working threads
     */
    unsigned int working_threads_;

    /**
     * Threads should terminate
     */
    bool should_terminate_ = false;

    /**
     * Threads should finish the work (queue), then terminate
     */
    bool should_finish_ = false;

    /**
     * Mutex for the queue
     */
    std::mutex mutex_;

    /**
     * Conditonal variable for the mutex
     */
    std::condition_variable condition_;

    /**
     * Worker threads
     */
    std::vector<std::thread> threads_;

    /**
     * Queue of tasks for the worker threads
     */
    std::queue<std::shared_ptr<qs_task>> tasks_;

    /**
     * Checks, if a thread should do something
     * Assumes, that mutex_ is locked by caller
     */
    bool should_do_something() const;

    /**
     * Checks, if the thread should exit when should_finish_ is true
     * Assumes, that mutex_ is locked by caller
     */
    bool should_finish_work() const;

    /**
     * Waits for new tasks and executes them.
     * Exits, if should_terminate_ is true.
     * Finishes the work and exits, if should_finish_ is true
     */
    void worker_thread();
};

#endif
