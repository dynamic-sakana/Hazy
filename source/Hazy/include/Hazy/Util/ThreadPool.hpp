#pragma once
#include <hazy_pch.h>
#include "Hazy/Util/Log.h"
namespace Hazy {
    /**
     * @brief 线程池，根据指定的线程数，将任务分配给不同的线程，注意数据竞争问题
     */
    class HAZY_API ThreadPool {
    public:
        /**
         * @brief 构造一个线程池
         * @param threadCount 这个线程池拥有的线程数量，默认为std::thread::hardware_concurrency()
         */
        ThreadPool(int threadCount = std::thread::hardware_concurrency());
        ~ThreadPool();
        template <typename Func, typename... Args>
        std::future<std::invoke_result_t<Func, Args...>> Execute(Func&& func, Args&&... args);

    private:
        void Work();

        std::condition_variable m_cv;
        std::vector<std::thread> m_threads;
        std::mutex m_queueMutex;
        std::queue<std::function<void()>> m_taskQueue;
        bool m_stopped;
    };


    /**
     * @brief 给这个线程池分配任务，注意在调用之前，一定检查Func函数的线程安全
     * @tparam Func 调用函数的类型
     * @tparam Args 调用函数的参数类型
     * @param func 调用的函数
     * @param args 要传递给函数的参数
     * @return std::future<std::invoke_result_t<Func, Args...>> 函数返回值
     */
    template <typename Func, typename... Args>
    std::future<std::invoke_result_t<Func, Args...>> ThreadPool::Execute(Func&& func, Args&&... args) {
        using return_type = std::invoke_result_t<Func, Args...>;

        // 包装任务为一个函数对象
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
        );

        std::future<return_type> future = task->get_future();

        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            if (m_stopped)
                throw std::runtime_error("ThreadPool has been stopped.");
            m_taskQueue.emplace([task]() { (*task)(); });
        }

        m_cv.notify_one();
        return future;
    }

}
