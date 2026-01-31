#include <Ioc/IServiceProvider.hpp>
#include <Mediator/Contracts/IResponse.hpp>
#include <Mediator/IRequestExceptionHandler.hpp>
#include <Mediator/IRequestHandler.hpp>
#include <Mediator/IRequestSender.hpp>
#include <Mediator/IResponseHandler.hpp>
#include <Task/ITask.hpp>
#include <Task/ITaskExecutor.hpp>
#include <exception>
#include <memory>
#include <typeindex>
#include <utility>
#include <vector>

namespace mediator::detail
{
    class RequestSender final : public IRequestSender
    {
    private:
        ioc::IServiceProvider &serviceProvider_;
        task::ITaskExecutor &taskExecutor_;

    public:
        explicit RequestSender(ioc::IServiceProvider &serviceProvider,
                               task::ITaskExecutor &taskExecutor)
            : serviceProvider_(serviceProvider), taskExecutor_(taskExecutor)
        {
        }

        void send(const std::type_index &requestHandlerTypeIndex,
                  const std::type_index &requestExceptionHandlerTypeIndex,
                  std::unique_ptr<IBaseRequest> requestPtr,
                  std::unique_ptr<IBaseResponseHandler> responseHandlerPtr) override
        {
            std::shared_ptr<IBaseRequestHandler> requestHandlerPtr =
                serviceProvider_.getRequiredService<IBaseRequestHandler>(requestHandlerTypeIndex);

            std::vector<std::shared_ptr<IBaseRequestExceptionHandler>> requestExceptionHandlerPtrs =
                serviceProvider_.getServices<IBaseRequestExceptionHandler>(
                    requestExceptionHandlerTypeIndex);

            taskExecutor_.execute(std::make_unique<HandleTask>(
                std::move(requestHandlerPtr), std::move(requestExceptionHandlerPtrs),
                std::move(requestPtr), std::move(responseHandlerPtr)));
        }

        class HandleTask final : public task::ITask
        {
        private:
            std::shared_ptr<IBaseRequestHandler> requestHandlerPtr_;
            std::vector<std::shared_ptr<IBaseRequestExceptionHandler>> requestExceptionHandlerPtrs_;
            std::unique_ptr<IBaseRequest> requestPtr_;
            std::unique_ptr<IBaseResponseHandler> responseHandlerPtr_;

        public:
            explicit HandleTask(std::shared_ptr<IBaseRequestHandler> requestHandlerPtr,
                                std::vector<std::shared_ptr<IBaseRequestExceptionHandler>>
                                    requestExceptionHandlerPtrs,
                                std::unique_ptr<IBaseRequest> requestPtr,
                                std::unique_ptr<IBaseResponseHandler> responseHandlerPtr)
                : requestHandlerPtr_(std::move(requestHandlerPtr)),
                  requestExceptionHandlerPtrs_(std::move(requestExceptionHandlerPtrs)),
                  requestPtr_(std::move(requestPtr)),
                  responseHandlerPtr_(std::move(responseHandlerPtr))
            {
            }

            coro::Awaitable<void> operator()() override
            {
                std::unique_ptr<IResponse> responsePtr;

                try
                {
                    responsePtr = requestHandlerPtr_->baseHandle(*requestPtr_);
                }
                catch (...)
                {
                    const std::exception_ptr &exceptionPtr = std::current_exception();

                    for (const auto &requestExceptionHandlerPtr : requestExceptionHandlerPtrs_)
                    {
                        if (requestExceptionHandlerPtr->baseTryHandle(*requestPtr_, exceptionPtr))
                        {
                            co_return;
                        }
                    }

                    co_return;
                }

                responseHandlerPtr_->baseHandle(std::move(responsePtr));
            }
        };
    };
}

namespace mediator
{
    std::unique_ptr<IRequestSender> makeRequestSender(ioc::IServiceProvider &serviceProvider,
                                                      task::ITaskExecutor &taskExecutor)
    {
        return std::make_unique<detail::RequestSender>(serviceProvider, taskExecutor);
    }
}