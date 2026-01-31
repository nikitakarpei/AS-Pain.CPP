#pragma once

#include <Coro/Awaitable.hpp>
#include <Mediator/Contracts/IStreamRequest.hpp>
#include <Mediator/StreamWriter.hpp>

namespace mediator
{
    class IBaseStreamRequestHandler
    {
    public:
        virtual ~IBaseStreamRequestHandler() = default;

        virtual coro::Awaitable<void> baseHandle(const IBaseStreamRequest &request,
                                                 IBaseStreamWriter &writer) = 0;
    };

    template <class TRequest> class IStreamRequestHandler : public IBaseStreamRequestHandler
    {
    public:
        ~IStreamRequestHandler() = default;

        virtual coro::Awaitable<void>
        handle(const TRequest &request, StreamWriter<typename TRequest::ResponseType> &&writer) = 0;

    private:
        coro::Awaitable<void> baseHandle(const IBaseStreamRequest &request,
                                         IBaseStreamWriter &writer) final
        {
            co_await handle(dynamic_cast<const TRequest &>(request),
                            StreamWriter<typename TRequest::ResponseType>(writer));
        }
    };
}