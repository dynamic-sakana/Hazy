#pragma once
#include "Hazy/Enumerates.h"
#include "Hazy/EventSystem/Event.h"

namespace Hazy {
    class HAZY_API MouseButtonPressedEvent : public Event {
    public:
        MouseButtonPressedEvent(MouseButton button, ModifierKey mod, Window* window) : Event(window), m_button(button), m_mod(mod) { }

        inline MouseButton getMouseButton() const { return m_button; }
        inline ModifierKey getMod() const { return m_mod; }

        inline static EventType getStaticType() { return EventType::MouseButtonPressed; }
        inline EventType getType() const override { return EventType::MouseButtonPressed; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Mouse | EventCategory::Input; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent -- Button: " << MouseButtonToString(m_button) << " Mod: " << ModifierKeyToString(m_mod) << " window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    private:
        MouseButton m_button;
        ModifierKey m_mod;
    };

    class HAZY_API MouseButtonReleasedEvent : public Event {
    public:
        MouseButtonReleasedEvent(MouseButton button, ModifierKey mod, Window* window) : Event(window), m_button(button), m_mod(mod) { }

        inline MouseButton getMouseButton() const { return m_button; }
        inline ModifierKey getMod() const { return m_mod; }

        inline static EventType getStaticType() { return EventType::MouseButtonReleased; }
        inline EventType getType() const override { return EventType::MouseButtonReleased; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Mouse | EventCategory::Input; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent -- Button: " << MouseButtonToString(m_button) << " Mod: " << ModifierKeyToString(m_mod) << " window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    private:
        MouseButton m_button;
        ModifierKey m_mod;
    };
    
    class HAZY_API MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(float x, float y, Window* window) : Event(window), m_mouseX(x), m_mouseY(y) { }

        inline float getX() const { return m_mouseX; }
        inline float getY() const { return m_mouseY; }

        inline static EventType getStaticType() { return EventType::MouseMoved; }
        inline EventType getType() const override { return EventType::MouseMoved; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Mouse | EventCategory::Input; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent -- X: " << m_mouseX << " Y: " << m_mouseY << " window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    private:
        float m_mouseX, m_mouseY;
    };

    class HAZY_API MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float x, float y, Window* window) : Event(window), m_xOffset(x), m_yOffset(y) { }

        inline float getXOffset() const { return m_xOffset; }
        inline float getYOffset() const { return m_yOffset; }
        
        inline static EventType getStaticType() { return EventType::MouseScrolled; }
        inline EventType getType() const override { return EventType::MouseScrolled; }
        inline EventCategory getCategoryFlags() const override { return EventCategory::Mouse | EventCategory::Input; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent -- XOffset: " << m_xOffset << " YOffset: " << m_yOffset << " window:  " << Event::getWindow()->getProps().title;
            return ss.str();
        }
    private:
        float m_xOffset, m_yOffset;
    };
}