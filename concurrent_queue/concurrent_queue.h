#include <mutex>
#include <queue>

template<typename T> class ConcurrentQueue
{
public:
    void Push(T t) { queue_.push(t); }
    T Pop()
    {
        auto t = queue_.front();
        queue_.pop();
        return t;
    }
    bool Empty() { return queue_.empty(); }

private:
    mutable std::mutex mutex_;
    mutable std::queue<T> queue_;
};