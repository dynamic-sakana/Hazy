#pragma once
#include "Hazy/Enumerates.h"
#include "Hazy/EventSystem/Event.h"
namespace Hazy {
    
    /**
     * @brief 键盘事件的基类，只重写了getCategoryFlags，还有三个函数：getStaticType()、getType()、toString()
     * 
     */
    class HAZY_API KeyEvent : public Event {
    public:
        inline Key getKey() const { return m_key; }
        inline ModifierKey getMod() const { return m_mod; }

        inline EventCategory getCategoryFlags() const override { return EventCategory::Keyboard | EventCategory::Input; }
    protected:
        KeyEvent(Key key, ModifierKey mod, Window* window) : Event(window), m_key(key), m_mod(mod) { }
        
    private:
        Key m_key;
        ModifierKey m_mod;
    };

    /**
     * @brief 按下键盘的事件的类，构造函数参数为：键、重复次数、修饰键、窗口指针
     */
    class HAZY_API KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(Key key, int repeatCount, ModifierKey mod, Window* window)
            : KeyEvent(key, mod, window), m_repeatCount(repeatCount) { }
        inline int getRepeatCount() const { return m_repeatCount; }

        inline static EventType getStaticType() { return EventType::KeyPressed; }
        inline EventType getType() const override { return EventType::KeyPressed; }

        inline std::string_view toString() const override {
            std::stringstream ss;
            if (m_repeatCount == 0)
                ss << "KeyPressedEvent -- " << " Key: " << KeyToString(KeyEvent::getKey()) << " Mod: " << ModifierKeyToString(KeyEvent::getMod()) << " window: " << Event::getWindow()->getProps().title;
            else
                ss << "KeyPressedEvent -- RepeatCount: " << m_repeatCount << " Key: " << KeyToString(KeyEvent::getKey()) << " Mod: " << ModifierKeyToString(KeyEvent::getMod()) << " window: " << Event::getWindow()->getProps().title;
            return ss.str();
        }

    private:
        int m_repeatCount;
    };

    /**
     *@brief 释放键盘的事件的类，构造函数参数为：键、修饰键、窗口指针
     */
    class HAZY_API KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(Key key, ModifierKey mod, Window* window) : KeyEvent(key, mod, window) { }

        inline static EventType getStaticType() { return EventType::KeyReleased; }
        inline EventType getType() const override { return EventType::KeyReleased; }
        inline std::string_view toString() const override {
            std::stringstream ss;
            ss << "KeyReleasedEvent -- Key: " << KeyToString(KeyEvent::getKey()) << " window: "
                << Event::getWindow()->getProps().title;
            return ss.str();
        }
    };

}