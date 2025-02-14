#include <hazy_pch.h>
#include "Hazy/Application.h"
#include "Hazy/Util/Log.h"
#include "Hazy/EventSystem.h"
#include "Hazy/Input.h"

namespace Hazy {
    ThreadPool Application::s_threadPool;
    std::unordered_set<UniqueRef<Window>> Application::s_windows;
    std::shared_mutex Application::s_windowMutex;
    Window* Application::s_currentFocused = nullptr;

    bool Application::s_running = false;

    UniqueRef<Application> Application::s_instance;
    std::once_flag Application::s_initializedFlag;
    std::stack<std::function<void()>> Application::s_shutDownHooks;

    Application::Application() {
        if (s_instance.get() != nullptr) {
            Logger::LogCritical("Application already exists");
            std::exit(EXIT_FAILURE);
        }
        Logger::LogTrace("Application created");
    }

    Application::~Application() {
        s_windows.clear();
        Logger::LogTrace("Application terminated");
    }

    void Application::Run() {
        if (!s_running) {
            Logger::LogWarn("Did you add a window or something? This function will exit later, check your code, please");
            return;
        }
        Hazy::Logger::LogInfo("================= Application running =====================");
        while (s_running) {
            if (s_windows.size() == 0) {
                s_running = false;
                break;
            }

            // 在首次进入主循环的时候，检查消息队列，
            // 因为此时有可能已经有窗口了，创建了窗口之后就会有消息进入消息队列
            // 所以在窗口更新状态之前就应该处理已经进入消息队列的事件
            CheckEvents();

            for (auto& window : s_windows) {
                window->update();
            }
        }
        Hazy::Logger::LogInfo("================== Application exited =====================");
    }

    void Application::CheckEvents() {
        while (true) {
            auto e = EventQueue::pollEvent();
            if (!e.has_value()) {
                break;
            }
            else {
                Event& event = *e.value();
                event.getWindow()->onEvent(event);  // 先将此事件转发给产生此事件的窗口，让窗口知道他们有什么事件
                switch (event.getType()) {
                case EventType::WindowClose:
                    Application::OnWindowClose(static_cast<WindowCloseEvent&>(event));
                    break;
                case EventType::WindowFocus:
                    s_currentFocused = static_cast<WindowFocusEvent&>(event).getWindow();
                    break;
                case EventType::WindowLostFocus:
                    if (s_currentFocused != nullptr || s_currentFocused == static_cast<WindowLostFocusEvent&>(event).getWindow())
                        s_currentFocused = nullptr;
                    break;
                default:
                    break;
                }
            }
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {

        auto it = std::find_if(s_windows.begin(), s_windows.end(),
            [&](const UniqueRef<Window>& window) {
                return window.get() == e.getWindow();
            });

        if (it != s_windows.end()) {
            // 获取到要关闭的窗口
            UniqueRef<Window>& window = const_cast<UniqueRef<Window>&>(*it);
            if (window->m_props.childWindows.empty()) {
                // 如果没有子窗口，直接销毁
                if (s_currentFocused == window.get()) {
                    s_currentFocused = nullptr;
                }
                s_windows.erase(it);
            }
            else {
                // 如果有子窗口，那么先销毁子窗口，然后再销毁父窗口
                std::queue<Window*> queue;    // 用于实现广度优先遍历
                std::stack<Window*> stack;    // 用于实现逆序删除
                queue.push(window.get());
                while (!queue.empty()) {
                    Window* temp = queue.front();
                    queue.pop();
                    for (Window* child : temp->m_props.childWindows) {
                        queue.push(child);
                    }
                    stack.push(temp);
                }
                while (!stack.empty()) {
                    EventQueue::emplaceEvent<WindowCloseEvent>(stack.top());
                    stack.pop();
                }
            }
        }
        return true;
    }

}