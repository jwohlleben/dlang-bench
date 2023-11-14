#include "taskpool.hpp"

#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include <vector>
#include <condition_variable>

#include "../sort/sort.hpp"
#include "qs_task.hpp"

taskpool::taskpool(unsigned int thread_count)
    : thread_count_(thread_count), working_threads_(thread_count)
{
    threads_.resize(thread_count_);
}

void taskpool::start()
{
    for (unsigned int i = 0; i < thread_count_; ++i)
    {
        threads_[i] = std::thread(&taskpool::worker_thread, this);
    }
}

void taskpool::stop()
{
    /* Scope for locking and unlocking mutex */
    {
        std::unique_lock<std::mutex> lock(mutex_);
        should_terminate_ = true;
    }

    condition_.notify_all();

    for (std::thread& thread : threads_)
    {
        thread.join();
    }

    threads_.clear();
}

void taskpool::wait_until_finished()
{
    /* Scope for locking and unlocking mutex */
    {
        std::unique_lock<std::mutex> lock(mutex_);
        should_finish_ = true;
    }

    condition_.notify_all();

    for (std::thread& thread : threads_)
    {
        thread.join();
    }

    threads_.clear();
}

void taskpool::work_until_finished()
{
    /* Scope for locking and unlocking mutex */
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ++thread_count_;
        ++working_threads_;
        should_finish_ = true;
    }

    condition_.notify_all();

    worker_thread(); // Caller also works

    for (std::thread& thread : threads_)
    {
        thread.join();
    }

    threads_.clear();
}

void taskpool::put(const std::shared_ptr<qs_task> task)
{
    /* Scope for locking and unlocking mutex */
    {
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push(task);
    }
    
    condition_.notify_one();
}

void taskpool::put_and_work_until_finished(const std::shared_ptr<qs_task> task)
{
    /* Scope for locking and unlocking mutex */
    {
        std::unique_lock<std::mutex> lock(mutex_);
        ++thread_count_;
        ++working_threads_;
        should_finish_ = true;
        tasks_.push(task);
    }

    condition_.notify_all();

    worker_thread();

    for (std::thread& thread : threads_)
    {
        thread.join();
    }

    threads_.clear();
}

const unsigned int& taskpool::thread_count() noexcept
{
    return thread_count_;
}

bool taskpool::should_do_something() const
{
    return !tasks_.empty() || should_terminate_
        || (should_finish_ && working_threads_ == 0);
}

bool taskpool::should_finish_work() const
{
    return should_finish_ && working_threads_ == 0 && tasks_.empty();
}

void taskpool::worker_thread()
{
    while (true)
    {
        std::shared_ptr<qs_task> task;
        unsigned int tid;

        /* Scope for locking and unlocking mutex */
        {
            std::unique_lock<std::mutex> lock(mutex_);
            --working_threads_;

            if (should_finish_work())
            {
                // Other threads can terminate
                should_terminate_ = true;

                lock.unlock();

                // Notify threads, that might wait for last thread
                condition_.notify_all();

                return;
            }

            condition_.wait(lock, [this]{return should_do_something();});

            if (should_terminate_)
            {
                return;
            }

            ++working_threads_;

            task = tasks_.front();

            std::unique_lock<std::mutex> task_lock(task->mutex);

            tid = task->assigned_threads;
            ++task->assigned_threads;

            if (task->assigned_threads == task->necessary_threads)
            {
                tasks_.pop();
                lock.unlock();
                condition_.notify_one(); // There might be more work available
                task_lock.unlock();
                task->condition.notify_all();
            }
            else
            {
                lock.unlock();
                condition_.notify_one();  // There might be more work available
                task->condition.wait(task_lock, [&]{return task->ready();});
            }
        }

        sort_parallel(this, task.get(), tid);
    }
}
