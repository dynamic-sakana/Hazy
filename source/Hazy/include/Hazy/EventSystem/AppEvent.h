#pragma once
#include "Hazy/EventSystem/Event.h"

namespace Hazy {

    class HAZY_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent(Window* window) : Event(window) { }
        
        inline static EventType getStaticType() { return EventType::WindowClose; }
        inline EventType getType() const override { return EventType::WindowClose; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "WindowCloseEvent -- Window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    };

    class HAZY_API WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height, Window* window)
            : Event(window), m_width(width), m_height(height) { }

        inline unsigned int getWidth() const { return m_width; }
        inline unsigned int getHeight() const { return m_height; }
        inline float getAspectRatio() const { return (float)m_width / (float)m_height; }

        inline static EventType getStaticType() { return EventType::WindowResize; }
        inline EventType getType() const override { return EventType::WindowResize; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent -- Window: " << Event::getWindow()->getProps().title << " width: " << m_width << " height: " << m_height;
            return ss.str();
        }
    private:
        unsigned int m_width, m_height;
    };

    class HAZY_API WindowFocusEvent : public Event {
    public:
        WindowFocusEvent(Window* window) : Event(window) { }
        inline static EventType getStaticType() { return EventType::WindowFocus; }
        inline EventType getType() const override { return EventType::WindowFocus; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "WindowFocusEvent -- Window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    };

    class HAZY_API WindowLostFocusEvent : public Event {
    public:
        WindowLostFocusEvent(Window* window) : Event(window) { }
        inline static EventType getStaticType() { return EventType::WindowLostFocus; }
        inline EventType getType() const override { return EventType::WindowLostFocus; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "WindowLostFocusEvent -- Window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    };

    class HAZY_API WindowMovedEvent : public Event {
    public:
        WindowMovedEvent(int x, int y, Window* window) : Event(window), x(x), y(y) { }

        inline int getX() const { return x; }
        inline int getY() const { return y; }
        
        inline static EventType getStaticType() { return EventType::WindowMoved; }
        inline EventType getType() const override { return EventType::WindowMoved; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "WindowMovedEvent -- Window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    private:
        int x, y;
    };

    class HAZY_API AppTickEvent : public Event {
    public:
        inline static EventType getStaticType() { return EventType::AppTick; }
        inline EventType getType() const override { return EventType::AppTick; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override { return "AppTickEvent"; }
    };

    class HAZY_API AppUpdateEvent : public Event {
    public:
        inline static EventType getStaticType() { return EventType::AppUpdate; }
        inline EventType getType() const override { return EventType::AppUpdate; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override { return "AppUpdateEvent"; }
    };

    class HAZY_API AppRenderEvent : public Event {
    public:
        inline static EventType getStaticType() { return EventType::AppRender; }
        inline EventType getType() const override { return EventType::AppRender; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Application; }
        inline std::string_view toString() const override { return "AppRenderEvent"; }
    };
}