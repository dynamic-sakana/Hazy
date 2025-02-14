#pragma once
#include "Hazy/Definition.h"

namespace Hazy {

    class Event;
    class MouseButtonPressedEvent;
    class MouseButtonReleasedEvent;
    class MouseMovedEvent;
    class MouseScrolledEvent;
    class KeyPressedEvent;
    class KeyReleasedEvent;
    class WindowResizeEvent;
    class Window;

    /**
     * @brief 层的基类，每一个层都属于某一个窗口
     */
    class HAZY_API Layer {
    public:
        explicit Layer(Window* window) : m_window(window) { }
        virtual ~Layer() = default;
        virtual void update() { }
        virtual void onEvent(Event&) { }

        inline bool isEnabled() const { return m_enabled; }
        inline void enable()  { m_enabled = true; }
        inline void disable() { m_enabled = false; }

    protected:
        bool m_enabled = true;
        Window* m_window;
    };

    class HAZY_API ImGuiLayer : public Layer {
    public:
        /**
         * @brief 构造函数
         * @param window 指定附加的窗口
         * @param renderFunc 指定渲染函数
         */
        ImGuiLayer(Window* window, const std::function<void()>& renderFunc);
        ImGuiLayer(Window* window);
        ~ImGuiLayer();
        void update() override;

        inline void setRenderFunc(const std::function<void()>& renderFunc) { m_renderFunc = renderFunc; }
        
    private:
        
        float m_time = 0.0f;
        static std::atomic<bool> s_ImGuiInited;
        std::function<void()> m_renderFunc;
    };

}