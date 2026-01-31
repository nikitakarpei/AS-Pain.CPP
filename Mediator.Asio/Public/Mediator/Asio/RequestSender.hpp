#pragma once

#include <Mediator/Contracts/IRequest.hpp>
#include <Mediator/IRequestExceptionHandler.hpp>
#include <Mediator/IRequestHandler.hpp>
#include <Mediator/IRequestSender.hpp>
#include <Mediator/IResponseHandler.hpp>
#include <boost/asio/async_result.hpp>
#include <boost/asio/bind_allocator.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/recycling_allocator.hpp>
#include <memory>

namespace mediator::asio
{
    class RequestSender
    {
    private:
        template <class TResponse, class THandler, class TWork>
        class ResponseHandler final : public IResponseHandler<TResponse>
        {
        private:
            THandler handler_;
            TWork work_;

        public:
            explicit ResponseHandler(THandler &&handler, TWork &&work)
                : handler_(std::move(handler)), work_(std::move(work))
            {
            }

            void handle(std::unique_ptr<TResponse> responsePtr) override
            {
                auto alloc = boost::asio::get_associated_allocator(
                    handler_, boost::asio::recycling_allocator<void>());

                boost::asio::dispatch(work_.get_executor(),
                                      boost::asio::bind_allocator(
                                          alloc, [handler{std::move(handler_)},
                                                  responsePtr{std::move(responsePtr)}]() mutable
                                          { std::move(handler)(std::move(responsePtr)); }));
            }
        };

        IRequestSender &sender_;

    public:
        explicit RequestSender(IRequestSender &sender) : sender_(sender) {}

        template <class TRequest, boost::asio::completion_token_for<
                                      void(std::unique_ptr<typename TRequest::ResponseType>)>
                                      TCompletionToken>
        auto send(std::unique_ptr<TRequest> requestPtr, TCompletionToken &&token)
        {
            auto init = [this](auto handler, std::unique_ptr<TRequest> requestPtr)
            {
                auto work = boost::asio::make_work_guard(handler);

                auto responseHandlerPtr =
                    std::make_unique<ResponseHandler<typename TRequest::ResponseType,
                                                     decltype(handler), decltype(work)>>(
                        std::move(handler), std::move(work));

                sender_.send(std::move(requestPtr), std::move(responseHandlerPtr));
            };

            return boost::asio::async_initiate<
                TCompletionToken, void(std::unique_ptr<typename TRequest::ResponseType>)>(
                init, token, std::move(requestPtr));
        }
    };
}