#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <condition_variable>
#include <future>
#include <chrono>
#include <utility>
#include <type_traits>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <imgui.h>

template<typename __Derived, typename __Base>
concept extendsFrom = std::derived_from<__Derived, __Base>;

template <typename Func, typename FirstArg, typename... Args>
using firstArg_t = typename std::function<
    decltype(
        std::declval<Func>()(std::declval<FirstArg>(), std::declval<Args>()...)
        )
>::argument_type;

#include "Hazy/Definition.h"