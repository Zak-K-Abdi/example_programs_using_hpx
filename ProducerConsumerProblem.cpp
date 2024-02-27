//
// Created by zakaria on 09/02/24.
/*
 * Using HPX synchronization objects
 * to give an example of how one could approach
 * the producer consumer problem
*/

#include "hpx/hpx_init.hpp"
#include "hpx/hpx_finalize.hpp"
#include "hpx/synchronization/mutex.hpp"
#include "hpx/synchronization/condition_variable.hpp"
#include "hpx/thread.hpp"
#include "hpx/parallel/algorithms/for_each.hpp"
#include "hpx/algorithm.hpp"
#include "hpx/execution.hpp"

#include "ConcurrentVector.h"

#include <iostream>
#include <queue>
#include <atomic>
#include <utility>
#include <thread>

hpx::condition_variable emptyBuffer;
hpx::condition_variable fullBuffer;
hpx::mutex m;

std::queue<int> buffer;
std::atomic<int> max_size = 5;
std::atomic<int> cnt = 1;


// Each thread will insert ten items into the q
// while the size of the q is <= 5.
void producer (int id) {
    for (int i = 0; i < 10; i++) {
        std::unique_lock<hpx::mutex> lock(m);
        fullBuffer.wait(lock, [] { return buffer.size() < max_size; });
        int val = rand() % 100;
        if (buffer.size() < max_size)
            buffer.push(val);
        std::cout << "Count: " << cnt++ << " Producer thread id: " << id << " produces: " << val << '\n';
        lock.unlock();
        emptyBuffer.notify_one();
    }
}

// Each thread will remove ten items from
// the queue while the queue is not empty.
void consumer (int id) {
    for (int i = 0; i < 10; i++) {
        std::unique_lock<hpx::mutex> lock(m);
        emptyBuffer.wait(lock, [] { return !buffer.empty(); });
        int val = buffer.front();
        buffer.pop();
        std::cout << "Count: " << cnt++ << " Consumer thread id: " << id << " consumed: " << val << '\n';
        lock.unlock();
        fullBuffer.notify_one();
    }
}


int hpx_main()
{
    hpx::thread prod1(producer, 1);
    hpx::thread prod2(producer, 2);
    hpx::thread cons1(consumer, 1);
    hpx::thread cons2(consumer, 2);

    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}