//
// Created by zakaria on 16/02/24.
//

#ifndef HPX_EXAMPLE_PROGRAM_CONCURRENTVECTOR_H
#define HPX_EXAMPLE_PROGRAM_CONCURRENTVECTOR_H

#include "hpx/synchronization/mutex.hpp"
#include "hpx/synchronization/counting_semaphore.hpp"
#include "hpx/synchronization/condition_variable.hpp"

#include <atomic>
#include <vector>
#include <cstdint>
#include <utility>
#include <mutex>

template <typename T>
class con_vector {
public:
    con_vector() = default;
    con_vector(const std::vector<T>& v) : vec(v), _size(v.size()) {}
    con_vector(const con_vector& other) : vec(other.vec), _size(other._size.load(std::memory_order_relaxed)) {}
    con_vector(con_vector&& other) : vec(std::move(other.vec)), _size(other._size.exchange(0, std::memory_order_relaxed)) {}
    con_vector(const std::initializer_list<T>& list) : vec(list), _size(list.size()) {};

    const std::atomic<std::size_t>& size() const { return _size; }
    const std::atomic<bool> empty() const { return _size == 0; }

    void push_back(const T& t) {
        std::lock_guard<hpx::mutex> lock(m);
        vec.push_back(t);
        _size.fetch_add(1, std::memory_order_relaxed);
    }

    template <typename... Args>
    void emplace_back(Args&&... args) {
        std::lock_guard<hpx::mutex> lock(m);
        vec.emplace_back(std::forward<Args>(args)...);
        _size.fetch_add(1, std::memory_order_relaxed);
    }

    //for writing at [idx]
    T& operator [](std::size_t idx) {
        std::lock_guard<hpx::mutex> lock(m);
        return vec[idx];
    }

    //for reading at [idx]
    const T& operator [](std::size_t idx) const {
        return vec[idx];
    }

    const T& back() const {
        return vec.back();
    }

private:
    std::atomic<std::size_t> _size;
    std::vector<T> vec;
    hpx::mutex m;

};

#endif //HPX_EXAMPLE_PROGRAM_CONCURRENTVECTOR_H
