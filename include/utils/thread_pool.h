#pragma once

#include <queue>
#include <functional>
#include <condition_variable>
#include <vector>
#include <thread>

class ThreadPool {

    public:
        ThreadPool(const size_t& num_threads);
        ~ThreadPool();

        void queue(std::function<void()> work);

    private:
        std::vector<std::jthread> workers_;
        std::condition_variable cv_;
        std::queue<std::function<void()>> queue_;
        std::mutex queue_mutex_;

};
