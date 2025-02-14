#include "Hazy/Util/ThreadPool.hpp"

namespace Hazy {

    ThreadPool::ThreadPool(int threadCount)
        : m_stopped(false) {
        Logger::LogTrace("Creating thread pool with {} threads", threadCount);
        for (int i = 0; i < threadCount; i++) {
            m_threads.emplace_back(std::thread(&ThreadPool::Work, this));
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stopped = true;
        }
        m_cv.notify_all();
        for (std::thread& thread : m_threads) {
            thread.join();
        }
        Logger::LogTrace("Destroyed thread pool");
    }


    void ThreadPool::Work() {
        while (true) {
            // 声明一个函数类型的变量
            std::function<void()> task;
            // 从任务队列中获取任务
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                // 如果现在没下班且任务队列为空，摸鱼等待任务
                // 如果现在没下班且任务队列不为空，取任务
                // 如果现在下班了，往下执行，直接下班了
                m_cv.wait(lock,
                    [this] {
                        return m_stopped || !m_taskQueue.empty();
                    });
                // 如果任务队列为空，且线程池停止，返回
                if (m_stopped && m_taskQueue.empty())
                    return;
                task = std::move(m_taskQueue.front());
                m_taskQueue.pop();
            }
            // 上班
            task();
        }
    }

}