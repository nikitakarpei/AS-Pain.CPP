#pragma once

#include <Coro/IAwaiter.hpp>
#include <Coro/ICoroResumer.hpp>
#include <Coro/ICoroResumerFactory.hpp>
#include <Coro/IThreadAwaitableExecutorContext.hpp>
#include <coroutine>
#include <type_traits>
#include <variant>

namespace coro
{
    template <typename TResult> class [[nodiscard]] Awaitable
    {
    private:
        struct FinalAwaiter
        {
            bool await_ready() const noexcept { return false; }

            template <typename P> auto await_suspend(std::coroutine_handle<P> handle) noexcept
            {
                return handle.promise().continuation;
            }

            void await_resume() const noexcept {}
        };

        struct PromiseBase
        {
            std::coroutine_handle<> continuation;

            void unhandled_exception() noexcept {}

            std::suspend_always initial_suspend() noexcept { return {}; }
            FinalAwaiter final_suspend() noexcept { return {}; }

            template <typename T> auto await_transform(Awaitable<T> &&awaitable)
            {
                return awaitable.operator co_await();
            }

            template <typename TAwaiter> struct CustomAwaiterWrapper
            {
                TAwaiter awaiter;

                bool await_ready() const noexcept { return awaiter.isCompleted(); }

                auto await_suspend(std::coroutine_handle<> handle) noexcept
                {
                    std::shared_ptr<ICoroResumer> resumerPtr{
                        IThreadAwaitableExecutorContext::instance().coroResumerFactory().create()};

                    awaiter.onCompleted(
                        [handle{std::move(handle)}, resumerPtr{std::move(resumerPtr)}]()
                        { resumerPtr->resume(std::move(handle)); });
                }

                template <class T = typename TAwaiter::ResultType>
                void await_resume()
                    requires(std::is_void_v<T>)
                {
                }

                template <class T = typename TAwaiter::ResultType>
                auto await_resume()
                    requires(!std::is_void_v<T>)
                {
                    return std::move(awaiter.getResult());
                }
            };

            template <typename TAwaiter>
                requires(std::is_base_of_v<IAwaiterBase, TAwaiter>)
            auto await_transform(TAwaiter &&awaiter)
            {
                return CustomAwaiterWrapper{std::move(awaiter)};
            }
        };

    public:
        template <class Enable = void> struct Promise : PromiseBase
        {
            std::variant<std::monostate, TResult> result;

            Awaitable get_return_object()
            {
                return Awaitable{std::coroutine_handle<Promise<>>::from_promise(*this)};
            }

            void return_value(TResult &&res) noexcept { result = std::move(res); }
        };

        template <> struct Promise<typename std::enable_if_t<std::is_void_v<TResult>>> : PromiseBase
        {
            Awaitable get_return_object()
            {
                return Awaitable{std::coroutine_handle<Promise<>>::from_promise(*this)};
            }

            void return_void() noexcept {}
        };

        using promise_type = Promise<>;

        Awaitable() = default;

        ~Awaitable()
        {
            if (handle_)
            {
                handle_.destroy();
            }
        }

        struct Awaiter
        {
            std::coroutine_handle<Promise<>> handle;

            bool await_ready() const noexcept { return !handle || handle.done(); }

            auto await_suspend(std::coroutine_handle<> calling) noexcept
            {
                handle.promise().continuation = calling;
                return handle;
            }

            template <typename T = TResult>
                requires(std::is_same_v<T, void>)
            void await_resume() noexcept
            {
            }

            template <typename T = TResult>
                requires(!std::is_same_v<T, void>)
            T await_resume() noexcept
            {
                return std::move(handle.promise().result);
            }
        };

        auto operator co_await() noexcept { return Awaiter{handle_}; }

    private:
        explicit Awaitable(std::coroutine_handle<Promise<>> handle) : handle_(handle) {}

        std::coroutine_handle<Promise<>> handle_;
    };
} // namespace coro
