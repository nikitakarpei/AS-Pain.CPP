#pragma once

#include <coroutine>

namespace coro
{
    class ICoroResumer
    {
    public:
        virtual ~ICoroResumer() = default;

        virtual void resume(std::coroutine_handle<> handle) = 0;
    };
}