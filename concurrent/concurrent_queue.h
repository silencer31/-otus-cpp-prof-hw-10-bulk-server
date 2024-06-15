#pragma once

#include <tuple>
#include <queue>
#include <mutex>

/**
* @brief  Потокобезопасная очередь с коллекцией строк для записи в файлы или вывода на консоль.
*/

template<typename T>
class ConcurrentQueue {
public:
    using hid_time_data = std::tuple<std::uint32_t, std::uint64_t, T>;

    bool empty() const {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.empty();
    }

    void push(const std::uint32_t& hid, const std::uint64_t& time, const T& data) {
        std::lock_guard<std::mutex> guard(queue_mtx);
        collection.push( std::make_tuple(hid, time, data));
    }

    hid_time_data front() {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.front();
    }

    void pop() {
        std::lock_guard<std::mutex> guard(queue_mtx);
        collection.pop();
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.size();
    }

    void clear() {
        std::lock_guard<std::mutex> guard(queue_mtx);
        return collection.clear();
    }
private:
    std::queue<hid_time_data> collection;
    mutable std::mutex queue_mtx;
};