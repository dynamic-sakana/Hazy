#pragma once
#define HAZY_API
#include <memory>

namespace Hazy {

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T>
    using WeakRef = std::weak_ptr<T>;

    template <typename T>
    using UniqueRef = std::unique_ptr<T>;

    enum class API : uint8_t {
        OpenGL
    };
    
}