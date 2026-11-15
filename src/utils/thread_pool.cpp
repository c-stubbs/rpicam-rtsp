#include "thread_pool.h"

#include <chrono>

std::string formatDuration(std::chrono::steady_clock::duration duration)
{
    using namespace std::chrono;

    auto ns = duration_cast<nanoseconds>(duration);

    auto s = duration_cast<seconds>(ns);
    ns -= s;

    auto ms = duration_cast<milliseconds>(ns);
    ns -= ms;

    auto us = duration_cast<microseconds>(ns);
    ns -= us;

    return fmt::format("{}s {}ms {}us {}ns", s.count(), ms.count(), us.count(), ns.count());
}

// Basic thread pool class

ThreadPool::ThreadPool(const size_t& num_threads, Logger& logger)
    : log_(logger)
{

    for (size_t i = 0; i < num_threads; ++i)
    {
        workers_.emplace_back(std::jthread([this, thread_id = i](std::stop_token st){
            while (!st.stop_requested())    
            {
                std::function<void()> work;

                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);
                    cv_.wait(lock, [this, &st](){return !queue_.empty() || st.stop_requested();});

                    if (st.stop_requested())
                        return;

                    work = std::move(queue_.front());
                    queue_.pop();
                }
                
                auto start = std::chrono::steady_clock::now();
                work();
                auto duration = std::chrono::steady_clock::now() - start;

                log_.debug("thread #{} worked {}", thread_id, formatDuration(duration)); 

            }
        }));
    }
};

ThreadPool::~ThreadPool()
{
   for (auto& worker : workers_)
   {
       worker.request_stop();
   }

   cv_.notify_all();

   for (auto& worker : workers_)
   {
       worker.join();
   }
};

void ThreadPool::queue(std::function<void()> work)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        queue_.push(std::move(work));
    }

    cv_.notify_all();
};
