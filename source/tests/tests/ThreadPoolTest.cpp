#include <Hazy.h>
#include <gtest/gtest.h>

int func(int& a) {
    a++;
    return a;
}

TEST(ThreadPoolTest, ThreadPoolTest_CommonFunction) {
    Hazy::ThreadPool pool;
    int a = 0;
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 1000; i++) {
        auto res = pool.Execute(func, std::ref(a));
        futures.push_back(std::move(res));
    }
    for (auto& future : futures) {
        future.wait();
    }
}


TEST(ThreadPoolTest, ThreadPoolTest_LambdaObject) {
    Hazy::ThreadPool pool;
    int a = 0;
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 1000; i++) {
        auto res = pool.Execute(
            [](int& a) {
                return ++a;
            }, std::ref(a));
        futures.push_back(std::move(res));
    }
    for (auto& future : futures) {
        future.wait();
    }
}

TEST(ThreadPoolTest, ThreadPoolTest_SecurityOfThreadPool) {
    Hazy::ThreadPool pool;
    int a = 0;
    std::mutex m;
    std::vector<std::future<int>> futures;

    std::thread t1([&]() {
        for (int i = 0; i < 500; i++) {
            auto res = pool.Execute(func, std::ref(a));
            {
                std::lock_guard<std::mutex> lock(m);
                futures.push_back(std::move(res));
            }
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 500; i++) {
            auto res = pool.Execute(func, std::ref(a));
            {
                std::lock_guard<std::mutex> lock(m);
                futures.push_back(std::move(res));
            }
        }
    });

    t1.join();
    t2.join();

    for (auto& future : futures) {
        future.wait();
    };

    EXPECT_EQ(futures.size(), 1000);
}