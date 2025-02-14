#pragma once
#include <hazy_pch.h>
#include "Hazy/EventSystem/EventEnum.h"
#include "Hazy/Definition.h"
#include "Hazy/Window.h"

namespace Hazy {

    class Event;

    /**
     * @brief 事件队列，单例类，这个类线程安全
     */
    class HAZY_API EventQueue {
    public:
        EventQueue(EventQueue&) = delete;
        EventQueue(EventQueue&&) = delete;
        EventQueue& operator=(EventQueue&) = delete;
        EventQueue& operator=(EventQueue&&) = delete;

        ~EventQueue() {
            Logger::LogTrace("EventQueue destroyed");
        }

        /**
         * @brief 从事件队列中拉取事件
         * @return std::optional<Ref<Event>> 一个指向事件的智能指针
         */
        inline static std::optional<Ref<Event>> pollEvent() {
            init();
            if (s_toDoList.empty())
                return { std::nullopt };
            else {
                {
                    std::lock_guard<std::mutex> lock(s_queueMutex);
                    Ref<Event> front = s_toDoList.front();
                    s_toDoList.pop();
                    return { front };
                }
            }
        }

        inline bool isEmpty() const { return s_toDoList.empty(); }

        /**
         * @brief 将事件放置于队列，如果这个事件没有被处理
         * @tparam T 事件类型（自动推导）
         * @param event 事件
         */
        template<extendsFrom<Event> T>
        inline static void pushEvent(T& event) {
            init();
            {
                std::lock_guard<std::mutex> lock(s_queueMutex);
                s_toDoList.emplace(std::move(event));
            }
        }

        /**
         * @brief 将事件放置于队列
         * @tparam T 事件类型（指定）
         * @param[in] args 事件构造函数参数
         */
        template<extendsFrom<Event> T, typename... Args>
        inline static void emplaceEvent(Args&&... args) {
            init();
            Ref<Event> event = Ref<T>(new T(std::forward<Args>(args)...));
            {
                std::lock_guard<std::mutex> lock(s_queueMutex);
                s_toDoList.emplace(event);
            }
        }

    private:
        
        inline static void init() {
            std::call_once(s_initialzed,
                [] {
                    s_eventQueue.reset(new EventQueue());
                    std::atexit([] { s_eventQueue.reset(); });
                });
        }
        
        EventQueue() {
            Logger::LogTrace("EventQueue created");
        }

        static std::once_flag s_initialzed;
        static UniqueRef<EventQueue> s_eventQueue;

        static std::mutex s_queueMutex;                        // 这个代办事件的互斥锁
        static std::queue<Ref<Event>> s_toDoList;  // 代办事件清单
    };


    /**
     * @brief 继承此类来创建自定义事件，注意需要创建函数：getStaticType()，返回事件的类型
     * 需要重写函数：getType()、getCategoryFlags()
     * 可以重写函数：toString()
     */
    class HAZY_API Event {
    public:
        Event(Window* window) : s_windows(window) { }

        /**
         * @brief 获取事件的类型（小类）
         * @return EventType 
         */
        virtual EventType getType() const = 0;

        /**
         * @brief 获取事件的类型（大类）
         * @return EventCategory 
         */
        virtual EventCategory getCategoryFlags() const = 0;

        /**
         * @brief 将事件转换成字符串，注意这个函数会访问Window中的属性（需要打印出产生此事件的窗口名称，所以在调用这个函数之前一定要确保产生此事件的窗口没有被销毁）
         * @return std::string_view 转换出来的新的字符串视图，为什么是视图，因为视图更加高效，虽然也没快到那里去
         */
        inline virtual std::string_view toString() const { return "Event -- Base"; }
        inline bool isHandled() const { return m_handled; }
        inline void markDone() { m_handled = true; }
        inline bool isInCategory(EventCategory category) const { return static_cast<bool>(getCategoryFlags() & category); }

        /**
         * @brief 获取产生这个事件的窗口
         * @return Window* 产生事件的窗口的指针
         */
        inline Window* getWindow() const { return s_windows; }

        /**
         * @brief 过滤事件，如果传入的事件的类型不是这个事件的类型，则标记为已经处理（逻辑上就是过滤）
         * @param event 需要判断的事件，如果这个事件是后面的事件类型，那么就放过它
         * @param eventType 需要这个事件的类型，（小类）
         */
        inline static void filter(Event& event, EventType eventType) {
            if (event.getType() != eventType)
                event.markDone();
        }

        /**
         * @brief 过滤事件，如果传入的事件的类型不是这个事件的类型，则标记为已经处理（逻辑上就是过滤）
         * @param event 需要判断的事件类型，如果这个事件是后面的事件类型，那么就放过它
         * @param eventCategory 需要这个时间的事件类型，（大类）
         */
        inline static void filter(Event& event, EventCategory eventCategory) {
            if (!event.isInCategory(eventCategory))
                event.markDone();
        }

    protected:
        bool m_handled = false;
        Window* s_windows;
    };
    
    /**
     * @brief 事件分发器，用于分发事件，使用装饰器模式
     * 
     */
    class HAZY_API EventDispatcher {
    public:
        EventDispatcher(Event& event) : m_event(event) { }

        /**
         * @brief 事件分发
         * @tparam T 这个事件按照什么类型处理
         * @param Fn 处理方法
         * @return true 处理成功
         * @return false 处理失败
         */
        template<class T>
        inline bool dispatch(std::function<bool(T&)> Fn) {
            if (!m_event.isHandled()) {
                // 如果此事件没有被处理过，则处理此事件
                if (m_event.getType() == T::getStaticType() && Fn(static_cast<T&>(m_event))) {
                    // 如果处理成功，标记事件为已处理
                    m_event.markDone();
                    return true;
                }
                return false;
            }
            return true;
        }

    private:
        
        Event& m_event;
    };

    
}