export module concurrent_queue;

import <mutex>;
export import <queue>;

export template <typename T>
class ConcurrentQueue {
 public:
  ConcurrentQueue(size_t capacity) : threshold_(capacity) {}

  void Push(T t) {
    std::unique_lock<std::mutex> lock(mutex_);
    not_full_cv_.wait(lock, [this] { return queue_.size() < threshold_; });
    queue_.push(t);
    lock.unlock();
    not_empty_cv_.notify_one();
  }

  T Pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    not_empty_cv_.wait(lock, [this] { return !queue_.empty(); });
    auto t = queue_.front();
    queue_.pop();
    lock.unlock();
    not_full_cv_.notify_one();
    return t;
  }

  bool Empty() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.empty();
  }

  bool Full() {
    std::unique_lock<std::mutex> lock(mutex_);
    return queue_.size() >= threshold_;
  }

 private:
  std::mutex mutex_;
  std::condition_variable not_full_cv_;
  std::condition_variable not_empty_cv_;
  std::queue<T> queue_;
  size_t threshold_;
};