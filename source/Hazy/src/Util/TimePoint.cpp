#include "Hazy/Util/TimePoint.h"

namespace Hazy {
    std::chrono::steady_clock::time_point TimePoint::s_begin = std::chrono::steady_clock::now();
}