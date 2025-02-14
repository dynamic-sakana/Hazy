#pragma once
#include <hazy_pch.h>
#include "Hazy/Definition.h"
#include "Hazy/Enumerates.h"

namespace Hazy {

    class HAZY_API Input {
    public:
        static bool isKeyPressed(Key key);
        static bool isMouseButtonPressed(MouseButton button);
        static const glm::vec2 getMousePosition();
    };

}