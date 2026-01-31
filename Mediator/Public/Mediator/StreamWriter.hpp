#pragma once

#include <Coro/Awaitable.hpp>
#include <memory>

namespace mediator
{
    class IStreamResponse;

    class IBaseStreamWriter
    {
    public:
        virtual ~IBaseStreamWriter() = default;

        virtual coro::Awaitable<void> baseWrite(std::unique_ptr<IStreamResponse> responsePtr) = 0;
    };

    template <class TResponse> class StreamWriter
    {
    private:
        IBaseStreamWriter &baseWriter_;

    public:
        explicit StreamWriter(IBaseStreamWriter &baseWriter) : baseWriter_(baseWriter) {}

        coro::Awaitable<void> write(std::unique_ptr<TResponse> responsePtr)
        {
            co_await baseWriter_.baseWrite(std::move(responsePtr));
        }
    };
}