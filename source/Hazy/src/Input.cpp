#include "Hazy/Renderer/Context.h"
#include "Hazy/Input.h"
#include "Hazy/Application.h"

namespace Hazy {

    bool Input::isKeyPressed(Key key) {
        Window* currentFocused = Application::getCurrentFocused();
        if (currentFocused == nullptr) {
            return false;
        }
        KeyAction state = currentFocused->getRenderContext().getKeyState(key);
        return state == KeyAction::Press || state == KeyAction::Repeat;
    }

    bool Input::isMouseButtonPressed(MouseButton button) {
        Window* currentFocused = Application::getCurrentFocused();
        if (currentFocused == nullptr) {
            return false;
        }
        MouseButtonAction state = currentFocused->getRenderContext().getMouseButtonState(button);
        return state == MouseButtonAction::Press;
    }

    const glm::vec2 Input::getMousePosition() {
        Window* currentFocused = Application::getCurrentFocused();
        if (currentFocused == nullptr) {
            return { 0.0f, 0.0f };
        }
        return currentFocused->getRenderContext().getMousePosition();
    }

}