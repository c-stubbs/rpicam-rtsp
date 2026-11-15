#include "thread_pool.h"

// Basic thread pool class

ThreadPool::ThreadPool(const size_t& num_threads) {

    for (size_t i = 0; i < num_threads; ++i)
    {
        workers_.emplace_back(std::jthread([this](std::stop_token st){
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

                work();
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
