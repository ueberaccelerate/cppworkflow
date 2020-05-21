#ifndef THREADPOOL_THREADPOOL_HPP
#define THREADPOOL_THREADPOOL_HPP

#include "threadsafe_queue.hpp"

// thread libraries
#include <thread>
#include <atomic>
#include <mutex>

// utils libraries
#include <functional>
#include <vector>
#include <exception>

class threadpool final {
private:
    class invalid_task : public std::exception {
    public:
    };
    using VoidFunction = std::function<void ()>;
    std::atomic_bool done_;
    std::vector<std::thread> threads_;
    threadsafe_queue<VoidFunction> queue_;
    void work_thread() {
        while (!done_) {
            try {
                VoidFunction task;
                if ( queue_.try_pop(task) ) {
                    task();
                } else {
                    std::this_thread::yield();
                }
            } catch ( ... ) {
            }
        }
    }
    threadpool() :
    done_{false},
    threads_{[&](){
        const auto thread_number = std::thread::hardware_concurrency();
        std::vector<std::thread> workers;
        try {
            for(size_t i = 0; i < thread_number; ++i) {
                workers.emplace_back(std::thread(&threadpool::work_thread, this));
            }
        } catch(...) {
            done_ = true;
            throw;
        }
        return workers;
    }()},
    queue_{}
    {
    }
public:
    template<typename FunctionType, typename ...Args>
    void submit(FunctionType func, Args ...args) {
        queue_.push([&](){
            func(args...);
        });
    }

    ~threadpool() {
        done_ = true;
        std::for_each(std::begin(threads_), std::end(threads_), [](auto &thread) {
            if (thread.joinable()) {
                thread.join();
            }
        });
    }

    static threadpool &get() {
      static threadpool instance;
      return instance;
    }
};
#endif // THREADPOOL_THREADPOOL_HPP
