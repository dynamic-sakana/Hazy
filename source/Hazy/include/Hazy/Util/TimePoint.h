#pragma once
#include <hazy_pch.h>

namespace Hazy {

    class HAZY_API TimePoint {
    public:
        TimePoint() = default;
        ~TimePoint() = default;

        /**
         * @brief 记录当前时间，并返回距离上次调用MoveOn的时间差，单位为秒
         * @tparam precision 时间差的精度
         * @return 此次距离上次的时间差
         */
        template <typename precision = float>
        inline precision MoveOn() {
            using namespace std::chrono;
            auto current = steady_clock::now();
            duration<precision> duration = current - m_current;
            m_current = current;
            return duration.count();
        }

        /**
         * @brief 获取当前时间，单位为秒
         * @tparam precision 时间精度
         * @return precision 类型的时间
         */
        template <typename precision = float>
        inline static precision Now() {
            using namespace std::chrono;
            auto now = steady_clock::now();
            duration<precision> duration = now - s_begin;
            return duration.count();
        }

    private:
        // 以微秒为单位
        std::chrono::steady_clock::time_point m_current;

        static std::chrono::steady_clock::time_point s_begin;
    };

}