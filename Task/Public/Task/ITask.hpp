#pragma once

#include <Coro/Awaitable.hpp>

namespace task
{
    class ITask
    {
    public:
        virtual ~ITask() = default;

        virtual coro::Awaitable<void> operator()() = 0;
    };
}
