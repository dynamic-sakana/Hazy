#include <Hazy.h>
#include <gtest/gtest.h>

TEST(EventQueueTest, EmplaceEventTest)
{
    Hazy::EventQueue::emplaceEvent<Hazy::WindowResizeEvent>(10, 10, nullptr);
    Hazy::EventQueue::emplaceEvent<Hazy::KeyPressedEvent>(Hazy::Key::A, 0, Hazy::ModifierKey::None, nullptr);
    while (true) {
        auto event = Hazy::EventQueue::pollEvent();
        if (event.has_value()) {
            Hazy::Logger::LogInfo("{}", static_cast<int>(event.value()->getType()));
        }
        else {
            break;
        }
    }
}

TEST(EventQueueTest, MultiThreadTest)
{
    std::thread t1(
        [] {
            for (int i = 0; i < 10; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                Hazy::EventQueue::emplaceEvent<Hazy::WindowLostFocusEvent>(nullptr);
            }
        });

    std::thread t2(
        [] {
            for (int i = 0; i < 20; i++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                Hazy::EventQueue::emplaceEvent<Hazy::MouseMovedEvent>(1, 1, nullptr);
            }
        }
    );

    t1.join();
    t2.join();

    int windowLostFocusCount = 0;
    int mouseMoveCount = 0;
    while (true) {
        auto event = Hazy::EventQueue::pollEvent();
        if (event.has_value()) {
            if (event.value()->getType() == Hazy::EventType::WindowLostFocus) {
                windowLostFocusCount++;
            }
            else if (event.value()->getType() == Hazy::EventType::MouseMoved) {
                mouseMoveCount++;
            }
        }
        else {
            break;
        }
    }
    EXPECT_EQ(windowLostFocusCount, 10);
    EXPECT_EQ(mouseMoveCount, 20);
}

TEST(EventQueueTest, ThreadPoolAndEventQueueTest) {
    Hazy::ThreadPool pool;

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 10; i++) {
        futures.push_back(pool.Execute(
            [] {
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
                Hazy::EventQueue::emplaceEvent<Hazy::WindowLostFocusEvent>(nullptr);
            }));
    }


    for (int i = 0; i < 20; i++) {
        futures.push_back(pool.Execute(
            [] {
                std::this_thread::sleep_for(std::chrono::milliseconds(2));
                Hazy::EventQueue::emplaceEvent<Hazy::MouseMovedEvent>(1, 1, nullptr);
            }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    EXPECT_EQ(futures.size(), 30);
    futures.clear();

    std::mutex window_mutex;
    int windowLostFocusCount = 0;
    std::mutex mouse_mutex;
    int mouseMoveCount = 0;

    for (int i = 0; i < 30; i++) {
        pool.Execute(
            [&]() {
                auto event = Hazy::EventQueue::pollEvent();
                if (event.has_value()) {
                    if (event.value()->getType() == Hazy::EventType::WindowLostFocus) {
                        std::lock_guard<std::mutex> lock(window_mutex);
                        windowLostFocusCount++;
                    }
                    else if (event.value()->getType() == Hazy::EventType::MouseMoved) {
                        std::lock_guard<std::mutex> lock(mouse_mutex);
                        mouseMoveCount++;
                    }
                }
            }
        );
    }

    Hazy::Logger::LogCritical("{}", windowLostFocusCount);
    Hazy::Logger::LogCritical("{}", mouseMoveCount);

    EXPECT_EQ(windowLostFocusCount, 10);
    EXPECT_EQ(mouseMoveCount, 20);
}