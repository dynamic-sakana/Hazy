#pragma once

namespace Hazy
{
    enum class EventType : uint8_t
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,   // Window events
        AppTick, AppUpdate, AppRender,                                          // App events
        KeyPressed, KeyReleased,                                                // Keyboard events
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled      // Mouse events
    };

    enum class EventCategory : uint8_t
    {
        None = 0,
        Application = 1 << 0,
        Input       = 1 << 1,
        Keyboard    = 1 << 2,
        Mouse       = 1 << 3,
        MouseButton = 1 << 4
    };

    inline EventCategory operator | (EventCategory lhs, EventCategory rhs) {
        return static_cast<EventCategory>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
    }

    inline EventCategory operator & (EventCategory lhs, EventCategory rhs) {
        return static_cast<EventCategory>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }
}