#pragma once
#include "Hazy/Window.h"
#include "Hazy/Util/Log.h"
#include "Hazy/Util/TimePoint.h"
#include "Hazy/LayerStack/LayerStack.h"
#include "Hazy/Renderer/Context.h"
#include "Hazy/Renderer/Renderer.h"

namespace Hazy {
    class Application;
    class Window;
    class Event;

    /**
     * @brief 窗口属性，用于初始化窗口，包括了窗口标题，窗口宽度，窗口高度，父窗口以及子窗口，禁用拷贝构造，允许移动构造
     */
    struct WindowProps {
    public:
        WindowProps(const std::string& title = "Hazy", unsigned int width = 1280, unsigned int height = 720, Window* parrentWindow = nullptr)
            : title(title), width(width), height(height), parrentWindow(parrentWindow) {
        }

        WindowProps(WindowProps&) = delete;
        WindowProps& operator=(WindowProps&) = delete;

        WindowProps(WindowProps&& other) {
            title = std::move(other.title);
            childWindows = std::move(other.childWindows);
            width = other.width;
            height = other.height;
            parrentWindow = other.parrentWindow;
        }

        WindowProps& operator=(WindowProps&& other) {
            title = std::move(other.title);
            childWindows = std::move(other.childWindows);
            width = other.width;
            height = other.height;
            parrentWindow = other.parrentWindow;
            return *this;
        }

    public:

        std::string title;
        unsigned int width;
        unsigned int height;

        Window* parrentWindow;
        std::unordered_set<Window*> childWindows;

    };

    /**
     * @brief 窗口类，注意有类持有或者继承自此类且有Render资源的时候，
     * 请务必在析构函数中调用m_context->bind()来获取到正确的上下文，在上下文中进行资源释放
     */
    class HAZY_API Window {
        friend class Application;
    public:
        /**
         * @brief 构造函数，这会往消息队列中添加此窗口成为焦点的事件
         * 如果指定了父窗口，那么新创建的窗口会成为父窗口的子窗口，
         * 构造窗口后会自动成为焦点（往事件队列中添加WindowFocusEvent）
         * @param props 窗口属性
         * @param api 渲染API
         */
        Window(WindowProps& props, API api = API::OpenGL);

        /**
         * @brief 构造函数，这会往消息队列中添加此窗口成为焦点的事件
         * 如果指定了父窗口，那么新创建的窗口会成为父窗口的子窗口，
         * 构造窗口后会自动成为焦点（往事件队列中添加WindowFocusEvent）
         * @param props 窗口属性
         * @param api 渲染API
         */
        Window(WindowProps&& props, API api = API::OpenGL);

        virtual ~Window();

        /**
         * @brief 获取此窗口的所有属性
         * @return const Window& 窗口属性
         */
        virtual inline const WindowProps& getProps() const { return m_props; }

        /**
         * @brief 现在是否为垂直同步状态
         * @return true  垂直同步
         * @return false 非垂直同步
         */
        virtual inline bool isVSync() const { return m_isVSync; }

        inline LayerStack& getLayerStack() { return m_layerStack; }
        inline Context& getRenderContext() const { return *m_context; }

        inline unsigned int getWidth() const { return m_props.width; }
        inline unsigned int getHeight() const { return m_props.height; }


        inline bool isChildWindowOf(Window* other) const {
            if (other != nullptr)
                return other->m_props.childWindows.contains(const_cast<Window*>(this));
            return false;
        }

        inline bool isParrentWindowOf(Window* other) const {
            return this->m_props.childWindows.contains(other);
        }

        inline void becomeChildOf(Window* other) {
            if (other != nullptr) {
                this->m_props.parrentWindow = other;
                other->m_props.childWindows.insert(this);
            }
        }

        inline void becomeParrentOf(Window* other) {
            if (other != nullptr) {
                this->m_props.childWindows.insert(other);
                other->m_props.parrentWindow = this;
            }
        }

        inline bool hasChildWindow() const { return !m_props.childWindows.empty(); }
        inline std::unordered_set<Window*>& getChildWindows() { return m_props.childWindows; }

    protected:

        /**
         *@brief 这个窗口在绘制每一帧的时候调用的函数，包括切换上下文，交换双缓冲，清除颜色等
         */
        virtual void update();

        /**
         * @brief 当窗口收到事件时调用，就是将事件转发给层，也可以在这个函数中处理事件
         * @param e
         */
        virtual void onEvent(Event& e);

        /**
         * @brief 设置垂直同步是否启用
         * @param enabled
         */
        virtual inline void setVSync(bool enabled) { m_context->enableVSync(enabled); }

        /**
         * @brief 设置渲染函数，这个渲染函数不用担心上下文问题，因为在调用这个渲染函数的时候一定在上下文中
         * @param func 渲染函数
         */
        virtual inline void setRenderFunc(const std::function<void()>& func) { m_renderFunc = std::move(func); }

    protected:
        
        virtual void registerEventCallback();

    protected:
        WindowProps m_props;
        API m_api;

        // 渲染上下文
        Ref<Context> m_context;
        
        // 渲染器
        Ref<Renderer> m_renderer;

        // 用于更新，指定更新的时候要干什么，注意此函数调用的时候不会有上下文
        std::function<void()> m_updateFunc;

        // 用于渲染，指定渲染的时候要干什么，在调用这个函数的时候无需担心上下文问题，因为在调用这个渲染函数的时候一定在上下文中
        std::function<void()> m_renderFunc;

        // 用于指定有上下文的时候额外干什么？比如说调整视口大小，在调用这里面保存的函数的时候一定有上下文
        std::queue<std::function<void()>> m_contentUpdateQueue;

        // 距离上一帧的时间
        float m_deltaTime = 0.0f;

        LayerStack m_layerStack;
        bool m_isVSync = true;

    private:

        TimePoint m_lastFrameTime;
    };
}

namespace std {

    // 特化std::hash<UniqueRef<Hazy::Window>>
    template <>
    struct hash<unique_ptr<Hazy::Window>> {
        size_t operator()(const unique_ptr<Hazy::Window>& window) const {
            return reinterpret_cast<size_t>(window.get());
        }
    };

    // 特化std::equal_to<UniqueRef<Hazy::Window>>
    template<>
    struct equal_to<unique_ptr<Hazy::Window>> {
        bool operator()(const unique_ptr<Hazy::Window>& lhs, const unique_ptr<Hazy::Window>& rhs) const {
            return lhs.get() == rhs.get();
        }
    };
};
