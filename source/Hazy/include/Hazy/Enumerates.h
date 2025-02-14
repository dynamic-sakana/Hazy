#pragma once
#include <hazy_pch.h>

namespace Hazy {

    enum class HAZY_API BufferUsage : uint8_t {
        StreamDraw,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy
    };

    enum class HAZY_API DataType : uint8_t {
        Int,
        Float,
        Bool
    };

    inline bool operator== (DataType lhs, DataType rhs) {
        return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
    }

    enum class HAZY_API TextureFilter : uint8_t {
        Nearest,
        Linear,
        NearestMipmapNearest,
        LinearMipmapNearest,
        NearestMipmapLinear,
        LinearMipmapLinear
    };

    enum class HAZY_API TextureType : uint8_t {
        Ambient,
        Diffuse,
        Specular,
        Normal
    };

    enum class HAZY_API TextureWrap : uint8_t {
        ClampToEdge,
        ClampToBorder,
        MirroredRepeat,
        Repeat
    };

    enum class MouseButtonAction : uint8_t {
        Release,
        Press
    };

    enum class KeyAction : uint8_t {
        Release,
        Press,
        Repeat
    };

    enum class HAZY_API Key : uint8_t {
        Unknown,
        Space,
        Apostrophe,   /* '   单引号 */
        Comma,        /* ,   逗号   */
        Minus,        /* -   减号   */
        Period,       /* .   句号   */
        Slash,        /* /   斜线   */
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,                     // Numbers
        Semicolon,    /* ;   分号   */
        Equal,        /* =   等号   */
        A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,   // Letters
        LeftBracket,  /* [ 左方括号 */
        Backslash,    /* \ 反斜线   */
        RightBracket, /* ] 右括号   */
        GraveAccent,  /* ` 反引号   */
        World1,
        World2,
        Esc, Enter, Tab, Backspace, Insert, Delete,
        Right, Left, Down, Up,
        PageUp, PageDown, Home, End,
        CapsLock, ScrollLock, NumLock, PrintScreen, Pause,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24, F25,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        NumpadDecimal, NumpadDivide, NumpadMultiply, NumpadSubtract, NumpadAdd, NumpadEnter, NumpadEqual,
        LeftShift, LeftControl, LeftAlt, LeftSuper,
        RightShift, RightControl, RightAlt, RightSuper,
        Menu
    };

    enum class MouseButton : uint8_t {
        Left,               // 主要的三个按键
        Right,
        Middle,

        SideButton1, SideButton2, SideButton3, SideButton4, SideButton5
    };

    enum class ModifierKey : uint8_t {
        None,
        Shift,
        Control,
        Alt,
        Super,
        CapsLock,
        NumLock
    };

    int BufferUsageToOpenGL(BufferUsage usage);
    int DataTypeToOpenGL(DataType type);
    int DataTypeSize(DataType type);
    int FilterToOpenGL(TextureFilter filter);
    int WrapToOpenGL(TextureWrap wrap);

    int KeyToGLFW(Key key);
    Key GLFWToKey(int key);
    std::string KeyToString(Key key);

    int MouseButtonToGLFW(MouseButton button);
    MouseButton GLFWToMouseButton(int button);
    std::string MouseButtonToString(MouseButton button);

    int ModifierKeyToGLFW(ModifierKey key);
    ModifierKey GLFWToModifierKey(int key);
    std::string ModifierKeyToString(ModifierKey key);

    int KeyActionToGLFW(KeyAction action);
    KeyAction GLFWToKeyAction(int action);

    int MouseButtonActionToGLFW(MouseButtonAction action);
    MouseButtonAction GLFWToMouseButtonAction(int action);
}