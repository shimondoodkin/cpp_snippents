
template<typename T>
class ThreadQueue : public std::thread {

    std::deque<T> queue;
    std::condition_variable signal;
    mutable std::mutex mutex_for_signal;
public:
    void push_back(T& value)
    {
        std::lock_guard<std::mutex> lock(mutex_for_signal);
        queue.push_back(std::move(value));
        signal.notify_one();
    }
    ThreadQueue(std::function <void(T&)> process_value, bool& running) : std::thread([this, &running, process_value] {
        while (running)
        {
            auto lock = std::unique_lock<std::mutex>(mutex_for_signal);
            signal.wait(lock);
            if (!queue.empty()) {
                T i = std::move(queue.front());
                queue.pop_front();
                lock.unlock();
                try {
                    process_value(i);
                }
                catch (const std::exception& e) {
                    std::cerr << " a standard exception was caught, with message '"
                        << e.what() << "'\n";
                }
            }
        }
        }) {}
};
