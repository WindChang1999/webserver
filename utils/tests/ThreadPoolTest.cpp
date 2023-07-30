#include<gtest/gtest.h>
#include<utils/ThreadPool.hpp>
#include<atomic>

TEST(ThreadPoolTest, submitVoidFuntion) {
    ThreadPool pool;
    int a = 0;
    std::future<void> future = pool.submit([&a] {
        a = 1;
    });
    future.wait();
    EXPECT_EQ(a, 1);
}

TEST(ThreadPoolTest, submitNonVoidFuntion) {
    ThreadPool pool;
    std::future<int> future = pool.submit([] {
        return 1;
    });
    int ret = future.get();
    EXPECT_EQ(ret, 1);
}

TEST(ThreadPoolTest, fetchAndIncPerThread) {
    int threadCount = 32;
    ThreadPool pool(threadCount);
    std::atomic_int a = 0;
    for (int i = 0; i < threadCount; ++i) {
        pool.submit([&a] {
            a.fetch_add(1);
        });
    }
    pool.waitForAllTasks();
    EXPECT_EQ(a.load(), threadCount);
}