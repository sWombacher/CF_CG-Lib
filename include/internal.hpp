#pragma once

#include <functional>
#include <mutex>

namespace cf {
namespace internal {

template <typename _ReturnType, typename... _Args> struct _ProtectedFunction;
template <typename _ReturnType, typename... _Args> struct _ProtectedFunction<_ReturnType(_Args...)> {
    template <typename _PT> void set(_PT&& forwardRef) {
        std::lock_guard<std::mutex> lg(this->m_Mutex);
        this->m_ProtectType = std::forward<_PT>(forwardRef);
    }
    template <typename... _FunctionArgs> _ReturnType operator()(_FunctionArgs&&... args) {
        std::lock_guard<std::mutex> lg(this->m_Mutex);
        if (this->m_ProtectType)
            return this->m_ProtectType(std::forward<_FunctionArgs>(args)...);

        // return default values if no function has been defined
        return _ReturnType();
    }

  private:
    std::mutex m_Mutex;
    std::function<_ReturnType(_Args...)> m_ProtectType;
};

} // namespace internal
} // namespace cf
