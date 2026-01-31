#pragma once

#include <coroutine>

namespace coro
{
    class BasicCoroutine
    {
    public:
        struct Promise
        {
            BasicCoroutine get_return_object() { return BasicCoroutine{}; }

            void unhandled_exception() noexcept {}

            void return_void() noexcept {}

            std::suspend_never initial_suspend() noexcept { return {}; }
            std::suspend_never final_suspend() noexcept { return {}; }

            auto await_transform(auto &&awaitable) { return awaitable.operator co_await(); }
        };

        using promise_type = Promise;
    };

} // namespace example