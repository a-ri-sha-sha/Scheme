#include "sema.h"

#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

#include <gtest/gtest.h>

void RunTest(int threads_count, int concurrency_level) {
    Semaphore semaphore(concurrency_level);
    for (int i = 0; i < concurrency_level; ++i) {
        semaphore.Enter();
    }

    int time = 0;
    std::vector<std::thread> threads;
    threads.reserve(threads_count);
    for (int i = 0; i < threads_count; ++i) {
        threads.emplace_back([&time, &semaphore, i]() {
            semaphore.Enter([&time, i](int& value) {
                auto cur_time = time++;
                ASSERT_EQ(i, cur_time);
                --value;
            });
            semaphore.Leave();
        });
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
    }

    for (int i = 0; i < concurrency_level; ++i) {
        semaphore.Leave();
        std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(200));
    }

    for (int i = 0; i < threads_count; ++i) {
        threads[i].join();
    }
}

TEST(Order, NewTest) {
    for (int i = 0; i < 10; ++i) {
        Semaphore semaphore(1);
        int time = 0;
        std::vector<std::thread> threads;
        threads.reserve(3);
        std::atomic<bool> flag = false;

        threads.emplace_back([&time, &semaphore, &flag]() {
            semaphore.Enter([&time](int& value) {
                auto cur_time = time++;
                ASSERT_EQ(0, cur_time);
                --value;
                ASSERT_TRUE(value >= 0);
            });
            while (!flag) {
            }
            semaphore.Leave();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        threads.emplace_back([&time, &semaphore]() {
            semaphore.Enter([&time](int& value) {
                auto cur_time = time++;
                ASSERT_EQ(1, cur_time);
                --value;
                ASSERT_TRUE(value >= 0);
            });
            semaphore.Leave();
        });

        threads.emplace_back([&time, &semaphore, &flag]() {
            while (!flag) {
            }
            semaphore.Enter([&time](int& value) {
                auto cur_time = time++;
                ASSERT_EQ(2, cur_time);
                --value;
                ASSERT_TRUE(value >= 0);
            });
            semaphore.Leave();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        flag = true;

        for (auto& cur : threads) {
            cur.join();
        }
    }
}

TEST(Order, Mutex) {
    RunTest(4, 1);
}

TEST(Order, Semaphore) {
    RunTest(4, 3);
}
