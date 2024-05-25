#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

class AudioBuffer {
public:
    AudioBuffer() : finished(false) {}

    void push(const std::vector<float>& data) {
        std::unique_lock<std::mutex> lock(mutex_);
        buffer_.push(data);
        condition_.notify_one();
    }

    bool pop(std::vector<float>& data) {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return !buffer_.empty() || finished; });

        if (buffer_.empty() && finished) {
            return false; // No more data to read
        }

        data = buffer_.front();
        buffer_.pop();
        return true;
    }

    void setFinished() {
        std::unique_lock<std::mutex> lock(mutex_);
        finished = true;
        condition_.notify_all();
    }

    bool isFinished() const {
        return finished;
    }

private:
    std::queue<std::vector<float>> buffer_;
    std::mutex mutex_;
    std::condition_variable condition_;
    bool finished;
};

#endif // AUDIOBUFFER_H
