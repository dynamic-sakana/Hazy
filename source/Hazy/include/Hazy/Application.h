#pragma once
#include <hazy_pch.h>
#include "Hazy/Window.h"
#include "Hazy/Util/ThreadPool.hpp"

namespace Hazy {

    class WindowCloseEvent;

    /**
     * @brief 一个程序只能有一个Application，且这个类无法被移动构造，实际上这是一个静态的类，所有属性方法都是静态的
     */
    class HAZY_API Application {
    public:
        virtual ~Application();
        static void Run();

        /**
         * @brief 添加一个窗口
         * @param prop 新的窗口的属性
         */
        template <API api>
        inline static void addWindow(WindowProps& props) {
            init();
            if constexpr (api == API::OpenGL) {
                s_windows.insert(UniqueRef(new Window(props, API::OpenGL)));
            }
        }

        template <API api>
        inline static void addWindow(WindowProps&& props) {
            init();
            if constexpr (api == API::OpenGL){
                s_windows.insert(UniqueRef(new Window(std::move(props), API::OpenGL)));
            }
        }

        inline static void addWindow(Window* window) {
            init();
            s_windows.insert(UniqueRef<Window>(window));
        }

        /**
         * @brief 对每一个窗口执行某一个行为
         * @tparam Func 函数类型
         * @tparam Args 参数类型
         * @param func 可调用对象（第一个参数必须是Window*类型的）
         * @param args 可调用对象的其他参数
         */
        template <typename Func, typename... Args>
        requires std::invocable<Func, Window*, Args...> && std::same_as<std::type_identity_t<Window*>, std::type_identity_t<typename std::function<decltype(std::declval<Func>()(std::declval<Window*>(), std::declval<Args>()...))(Window*, Args...)>::argument_type>>
        inline static void forEachWindow(Func&& func, Args&&... args) {
            if (!s_running)
                Logger::LogWarn("You can not loop through windows before you add one, right?");
            for (auto& window : s_windows) {
                func(window.get(), std::forward<Args>(args)...);
            }
        }

        /**
         * @brief 按照名称查找窗口
         * @param name 窗口的名称
         * @return Window* 窗口的指针
         */
        inline static Window* getWindow(std::string&& name) {
            if (!s_running)
                Logger::LogWarn("You can not get window before you add one, right?");
            for (auto& window : s_windows) {
                if (window->getProps().title == name) {
                    return window.get();
                }
            }
            return nullptr;
        }

        /**
         * @brief 获取窗口的指针，裸指针，注意一定不能将它释放，不要调用Window的非const成员函数
         * @param pred 谓词
         * @return Window* 满足谓词条件的窗口
         */
        [[deprecated("Do not use this function, use getWindow(std::string&&) instead")]]
        inline static Window* getWindow(std::function<bool(Window*)> pred) {
            if (!s_running)
                Logger::LogWarn("You can not get window before you add one, right?");
            for (auto& window : s_windows) {
                if (pred(window.get())) {
                    return window.get();
                }
            }
            return nullptr;
        }
        
        inline static Window* getCurrentFocused() {
            if (!s_running)
                Logger::LogWarn("You can not get current focused window before you add one, right?");
            return s_currentFocused;
        }

        inline static bool isAlive() { return s_running; }

        inline static Application& get() {
            init();
            return *(s_instance.get());
        }

        inline static ThreadPool& getThreadPool() { return s_threadPool; }

        inline static void addShutDownHook(std::function<void()> func) {
            s_shutDownHooks.push(func);
        }

    protected:
        
        Application();

        /**
         * @brief 从消息队列中获取事件，然后分发事件给各个窗口
         */
        static void CheckEvents();
        static bool OnWindowClose(WindowCloseEvent& e);

    private:
        inline static void init() {
            std::call_once(s_initializedFlag,
                [] {
                    s_instance.reset(new Application());
                    s_running = true;
                    std::atexit([]() {
                        s_running = false;
                        while (!s_shutDownHooks.empty()) {
                            s_shutDownHooks.top()();
                            s_shutDownHooks.pop();
                        }
                        s_instance.reset();
                    });
                });
        }

        static ThreadPool s_threadPool;
        static std::unordered_set<UniqueRef<Window>> s_windows;
        static std::shared_mutex s_windowMutex;
        static bool s_running;

        static Window* s_currentFocused;

        static UniqueRef<Application> s_instance;
        static std::once_flag s_initializedFlag;

        static std::stack<std::function<void()>> s_shutDownHooks;
    };

}