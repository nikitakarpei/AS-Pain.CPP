#pragma once

#include <Mediator/StreamReader.hpp>
#include <boost/asio/async_result.hpp>
#include <boost/asio/bind_allocator.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/recycling_allocator.hpp>
#include <memory>

namespace mediator::asio
{
    template <class TResponse> class StreamReader
    {
    private:
        mediator::StreamReader<TResponse> baseReader_;

    public:
        explicit StreamReader(mediator::StreamReader<TResponse> &&baseReader)
            : baseReader_(std::move(baseReader))
        {
        }

        OptionalRef<const TResponse> getCurrentOrNull() { return baseReader_.getCurrentOrNull(); }

        bool hasMore() { return baseReader_.hasMore(); }

        template <boost::asio::completion_token_for<void()> TCompletionToken>
        auto getNext(TCompletionToken &&token)
        {
            auto init = [this](auto handler)
            {
                auto work = boost::asio::make_work_guard(handler);

                auto handlerPtr = std::make_shared<decltype(handler)>(std::move(handler));
                auto workPtr = std::make_shared<decltype(work)>(std::move(work));

                baseReader_.getNext(
                    [handlerPtr{std::move(handlerPtr)}, workPtr{std::move(workPtr)}]() mutable
                    {
                        auto alloc = boost::asio::get_associated_allocator(
                            *handlerPtr, boost::asio::recycling_allocator<void>());

                        boost::asio::dispatch(workPtr->get_executor(),
                                              boost::asio::bind_allocator(
                                                  alloc,
                                                  [handlerPtr{std::move(handlerPtr)}]() mutable
                                                  { std::move (*handlerPtr)(); }));
                    });
            };

            return boost::asio::async_initiate<TCompletionToken, void()>(init, token);
        }
    };
}