#include "Hazy/Window.h"
#include <hazy_pch.h>
#include "Hazy/Util/Log.h"
#include "Hazy/EventSystem.h"

namespace Hazy {

    Window::Window(WindowProps& props, API api)
        : m_props(std::move(props)), m_api(api), m_updateFunc([] { }), m_renderFunc([this] { m_renderer->clear(); }) {
        if (api == API::OpenGL) {
            {
                m_context = Context::create<API::OpenGL>(this, m_props.title, m_props.width, m_props.height);
                setVSync(true);
                if (m_props.parrentWindow != nullptr) {
                    m_props.parrentWindow->m_props.childWindows.insert(this);
                }
            }

            {
                ContextLock lock(*m_context);
                m_renderer = Renderer::create<API::OpenGL>();
            }
        }

        EventQueue::emplaceEvent<WindowFocusEvent>(this);

        registerEventCallback();
    }

    Window::Window(WindowProps&& props, API api)
        : m_props(std::move(props)), m_api(api), m_updateFunc([] { }), m_renderFunc([this] { m_renderer->clear();}) {
        if (api == API::OpenGL) {
            {
                m_context = Context::create<API::OpenGL>(this, m_props.title, m_props.width, m_props.height);
                setVSync(true);
                if (m_props.parrentWindow != nullptr) {
                    m_props.parrentWindow->m_props.childWindows.insert(this);
                }
            }

            {
                ContextLock lock(*m_context);
                m_renderer = Renderer::create<API::OpenGL>();
            }
        }

        EventQueue::emplaceEvent<WindowFocusEvent>(this);

        registerEventCallback();
    }

    Window::~Window() {
        // 通知父子窗口，我被销毁了，你们自由了
        if (this->m_props.parrentWindow != nullptr) {
            this->m_props.parrentWindow->m_props.childWindows.erase(this);
        }
        if (!this->m_props.childWindows.empty()) {
            for (Window* child : this->m_props.childWindows) {
                child->m_props.parrentWindow = nullptr;
            }
        }
        Logger::LogTrace("Window destroyed: {} ", m_props.title);
    }

    void Window::update() {
        m_deltaTime = m_lastFrameTime.MoveOn();
        m_updateFunc();

        // 上下文锁，保证在调用上下文相关的函数时，上下文是有效的
        ContextLock contentLock(*m_context);

        while (!m_contentUpdateQueue.empty()) {
            m_contentUpdateQueue.front()(); // 调用添加的需要上下文的函数
            m_contentUpdateQueue.pop();     // 移除已经调用的函数
        }

        m_renderFunc();

        for (auto layer : m_layerStack) {
            layer->update();
        }

        m_context->SwapBuffers();
    }

    void Window::onEvent(Event& e) {
        m_layerStack.onEvent(e);
        switch (e.getType()) {
        case EventType::WindowResize:
            m_props.width = static_cast<WindowResizeEvent&>(e).getWidth();
            m_props.height = static_cast<WindowResizeEvent&>(e).getHeight();
            m_contentUpdateQueue.emplace([this] { m_renderer->resize(m_props.width, m_props.height); });
            break;
        default:
            break;
        }
    }

    // 所有的回调函数都在这里注册
    void Window::registerEventCallback() {

        m_context->callback.whenWindowResized = 
            [](Window* window, int width, int height) {
                EventQueue::emplaceEvent<WindowResizeEvent>(width, height, window);
            };

        m_context->callback.whenWindowFocusChanged = 
            [](Window* window, bool focused) {
                if (focused)
                    EventQueue::emplaceEvent<WindowFocusEvent>(window);
                else
                    EventQueue::emplaceEvent<WindowLostFocusEvent>(window);
            };

        m_context->callback.whenWindowClosed = 
            [](Window* window) {
                EventQueue::emplaceEvent<WindowCloseEvent>(window);
            };

        m_context->callback.whenWindowMoved = 
            [](Window* window, int xpos, int ypos) {
                EventQueue::emplaceEvent<WindowMovedEvent>(xpos, ypos, window);
            };

        m_context->callback.whenKeyTriggered = 
            [](Window* window, Key key, int /* scancode */, KeyAction action, ModifierKey mods) {
                switch (action) {
                case KeyAction::Press:
                    EventQueue::emplaceEvent<KeyPressedEvent>(key, 0, mods, window);
                    break;
                case KeyAction::Release:
                    EventQueue::emplaceEvent<KeyReleasedEvent>(key, mods, window);
                    break;
                case KeyAction::Repeat:
                    EventQueue::emplaceEvent<KeyPressedEvent>(key, 1, mods, window);
                    break;
                }
            };

        m_context->callback.whenMouseClicked = 
            [](Window* window, MouseButton button, MouseButtonAction action, ModifierKey mods) {
                switch (action) {
                case MouseButtonAction::Press:
                    EventQueue::emplaceEvent<MouseButtonPressedEvent>(button, mods, window);
                    break;
                case MouseButtonAction::Release:
                    EventQueue::emplaceEvent<MouseButtonReleasedEvent>(button, mods, window);
                    break;
                }
            };

        m_context->callback.whenMouseScrolled = 
            [](Window* window, float xoffset, float yoffset) {
                EventQueue::emplaceEvent<MouseScrolledEvent>(xoffset, yoffset, window);
            };

        m_context->callback.whenMouseMoved = 
            [](Window* window, float x, float y) {
                EventQueue::emplaceEvent<MouseMovedEvent>(x, y, window);
            };
    }
}
