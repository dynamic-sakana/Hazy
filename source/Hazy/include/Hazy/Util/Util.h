#include <hazy_pch.h>

namespace Hazy {

    template<typename NumType>
    NumType NormalizeAngle(NumType x, NumType period) {
        // 处理特殊情况
        if (period == NumType(0)) return x;
        if (x == NumType(0)) return NumType(0);

        // 使用较大的中间类型来存储计算结果，
        // 如果 NumType 是float，那么使用double存储中间值，
        // 如果是其他数值类型，使用long double从储存中间值
        using CalcType = typename std::conditional<std::is_same_v<NumType, float>, double, long double>::type;

        CalcType cx = static_cast<CalcType>(x);
        CalcType cperiod = static_cast<CalcType>(period);

        // 首先用整除快速减小x的值域
        CalcType quotient = std::floor(cx / cperiod);
        CalcType remainder = cx - quotient * cperiod;

        // 确保结果在[0, period)范围内
        if (remainder < CalcType(0)) {
            remainder += cperiod;
        }

        // 如果结果接近period，则返回0
        if (std::abs(remainder - cperiod) < std::numeric_limits<NumType>::epsilon() * cperiod) {
            return NumType(0);
        }

        // 找到最接近0的等价值
        if (remainder > cperiod / CalcType(2)) {
            remainder -= cperiod;
        }

        return static_cast<NumType>(remainder);
    }

}