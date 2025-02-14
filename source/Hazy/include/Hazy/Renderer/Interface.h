#pragma once
#include <hazy_pch.h>

namespace Hazy
{

    template<typename T>
    concept HasUnbind = requires(T t) {
        { t.unbind() } -> std::same_as<void>;
    };

    /**
     * @brief 绑定锁，在析构时自动解绑它管理的资源
     * @tparam T 它管理的资源类型，需要提供 unbind() 方法
     */
    template<HasUnbind T>
    class BindLock {
    public:
        template<typename... Args>
        BindLock(T& obj, Args&&... args) : m_obj(obj) { m_obj.bind(std::forward<Args>(args)...); }

        template<typename... Args>
        BindLock(T* obj, Args&&... args) : m_obj(*obj) { m_obj.bind(std::forward<Args>(args)...); }

        template<typename... Args>
        BindLock(const Ref<T>& obj, Args&&... args) : m_obj(*obj) { m_obj.bind(std::forward<Args>(args)...); }

        template<typename... Args>
        BindLock(const UniqueRef<T>& obj, Args&&... args) : m_obj(*obj) { m_obj.bind(std::forward<Args>(args)...); }

        ~BindLock() { m_obj.unbind(); }
    private:
        T& m_obj;
    };

}