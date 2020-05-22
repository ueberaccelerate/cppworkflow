#ifndef UTILS_TIMER_HPP
#define UTILS_TIMER_HPP

#include <chrono>
#include <functional>
#include <future>

namespace utils {

class Timer {
 public:
  using TimerInterval = std::chrono::seconds;
  using Block = std::function<void(Timer &)>;
  Timer(const TimerInterval seconds, bool repeat, Block b)
      : repeat_{repeat},
        started_{false},
        seconds_{seconds},
        fireBlock_{b},
        resultBlock_{try_async_block()} {}

  void start() {
    block_cv_.notify_one();
    started_ = true;
    std::this_thread::yield();
  }
  
  void invalidate() {
    block_cv_.notify_one();
    started_ = true;
  }

  ~Timer() {
    invalidate();
    resultBlock_.wait();
  }

 private:
  std::function<std::future<int>()> try_async_block = [&]() {
    try {
      auto future = std::async([&]() {
        std::unique_lock<std::mutex> lock{block_mutex_};
        block_cv_.wait(lock, [&]() { return started_ == true; });
        fireBlock_(*this);
        return 0;
      });
      return future;
    } catch (std::system_error &e) {
      throw;
    } catch (...) {
      throw;
    }
  };

  std::atomic_bool repeat_;
  std::atomic_bool started_;
  std::mutex block_mutex_;
  std::condition_variable block_cv_;
  TimerInterval seconds_;
  Block fireBlock_;
  std::future<int> resultBlock_;
};

}  // namespace utils
#endif  // UTILS_TIMER_HPP
