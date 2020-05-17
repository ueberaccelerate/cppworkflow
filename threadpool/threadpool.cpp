
// thread libraries
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <condition_variable>

// utils libraries
#include <functional>
#include <vector>
#include <exception>
#include <queue>

// output libraries
#include <iostream>

#define NODISCARD [[nodiscard]]

template<typename T, typename Container = std::queue<T>>
class threadsafe_queue {
    class threadsafe_empty_error: public std::exception {
        
    };
public:
    using value_type      = typename Container::value_type;
    using value_type_ptr  = std::shared_ptr<value_type>;
    using size_type       = typename Container::size_type;
    using reference       = typename Container::reference;
    using const_reference = typename Container::const_reference;

    threadsafe_queue &operator=(const threadsafe_queue&) = delete;
    
    void push(const value_type &value) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        queue_.push(value);
        queue_cv_.notify_one();
    }
    
    void push(value_type &&value) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        queue_.push(value);
        queue_cv_.notify_one();
    }
    
    
    NODISCARD bool try_pop(T &element)  {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        if(queue_.empty()) {
            return false;
        }
        element = queue_.front();
        queue_.pop();
        return true;
    }

    NODISCARD value_type_ptr try_pop()  {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        if(queue_.empty()) {
            return value_type_ptr();
        }
        auto result = std::make_shared<value_type>(queue_.front());
        queue_.pop();
        return result;
    }
    void wait_and_pop(T &element) {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        queue_cv_.wait(lock, [this](){return !queue_.empty();});
        element = queue_.front();
        queue_.pop();
    }
    
    NODISCARD value_type_ptr wait_and_pop() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        queue_cv_.wait(lock, [this](){return !queue_.empty();});
        auto result = std::make_shared<value_type>(queue_.front());
        queue_.pop();
        return  result;
    }
private:
    std::mutex    queue_mutex_;
    std::condition_variable queue_cv_;
    Container queue_;
};

class threadpool final {
private:
    
    class invalid_task : public std::exception {
    public:
        const char * what() const throw() override {
            return "treadpool::invalid_task";
        }
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
public:
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
};

int main() {
    std::cout << std::thread::hardware_concurrency() << "\n";
    threadpool pool;
    pool.submit([](int a, int b){
        return a + b;
    }, 42, 32);

    return 0;
}
