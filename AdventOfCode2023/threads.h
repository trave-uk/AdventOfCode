// ThreadSafeQueue is from this answer: https://codereview.stackexchange.com/a/267859

#include <exception>
#include <mutex>
#include <queue>
#include <thread>
#include <stack>

class finishedThread : public std::exception {};

template <typename T>
class ThreadSafeQueue {
    std::mutex mutex;
    std::condition_variable cond_var;
    std::queue<T> queue;

public:
    void push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            queue.push(item);
        }

        cond_var.notify_one();
    }

    T& front() {
        std::unique_lock lock(mutex);
        cond_var.wait(lock, [&] { return !queue.empty(); });
        return queue.front();
    }

    void pop() {
        std::lock_guard<std::mutex> lock(mutex);
        queue.pop();
    }

    T pop_front() {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty())
            throw finishedThread();
        T item = queue.front();
        queue.pop();
        return item;
    }

    bool empty()
    {
        return queue.empty();
    }
};

template <typename T>
class ThreadSafeStack {
    std::mutex mutex;
    std::condition_variable cond_var;
    std::stack<T> stack;

public:
    void push(T&& item) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stack.push(item);
        }

        cond_var.notify_one();
    }

    T& back() {
        std::unique_lock lock(mutex);
        cond_var.wait(lock, [&] { return !stack.empty(); });
        return stack.back();
    }

    void pop() {
        std::lock_guard lock(mutex);
        stack.pop();
    }

    T pop_top() {
        std::lock_guard<std::mutex> lock(mutex);
        if (stack.empty())
            throw finishedThread();
        T item = stack.top();
        stack.pop();
        return item;
    }
};
